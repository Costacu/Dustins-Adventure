#include "../header/Enemy.h"
#include "../header/TextureManager.h"
#include "../header/GameTemplates.h"
#include <cmath>
#include <algorithm>
#include <vector>

Enemy::Enemy(std::string name, int hp, float speed, std::string texturePath)
    : Entity(0.f, 0.f),
      name_(std::move(name)),
      texturePath_(std::move(texturePath)),
      hp_(hp),
      speed_(speed),
      state_(State::Patrol),
      direction_(1.f, 0.f),
      distracted_(false),
      distractTimer_(0.f),
      distractPos_(0.f, 0.f),
      pathTimer_(0.f),
      stuckTimer_(0.f),
      lastPos_(0.f, 0.f)
{
    loadTexture();
}

Enemy::Enemy(const Enemy& other)
    : Entity(other),
      name_(other.name_),
      texturePath_(other.texturePath_),
      hp_(other.hp_),
      speed_(other.speed_),
      state_(other.state_),
      direction_(other.direction_),
      distracted_(other.distracted_),
      distractTimer_(other.distractTimer_),
      distractPos_(other.distractPos_),
      path_(other.path_),
      pathTimer_(other.pathTimer_),
      stuckTimer_(other.stuckTimer_),
      lastPos_(other.lastPos_)
{
    loadTexture();
}

Enemy& Enemy::operator=(const Enemy& other) {
    if (this != &other) {
        Entity::operator=(other);
        name_ = other.name_;
        hp_ = other.hp_;
        speed_ = other.speed_;
        texturePath_ = other.texturePath_;
        state_ = other.state_;
        direction_ = other.direction_;
        distracted_ = other.distracted_;
        distractTimer_ = other.distractTimer_;
        distractPos_ = other.distractPos_;
        path_ = other.path_;
        pathTimer_ = other.pathTimer_;
        stuckTimer_ = other.stuckTimer_;
        lastPos_ = other.lastPos_;
        loadTexture();
    }
    return *this;
}

Enemy::~Enemy() {}

Entity* Enemy::clone() const {
    return new Enemy(*this);
}

void Enemy::print(std::ostream& os) const {
    os << "Enemy(name=" << name_ << ", speed=" << speed_.get() << ")";
}

sf::FloatRect Enemy::getBounds() const {
    sf::FloatRect b = sprite_.getGlobalBounds();
    float margin = 20.f;
    return sf::FloatRect(b.left + margin, b.top + margin, b.width - 2 * margin, b.height - 2 * margin);
}

void Enemy::update(float dt) {
    if (distracted_) {
        distractTimer_ -= dt;
        if (distractTimer_ <= 0.f)
            distracted_ = false;
    }
}

bool Enemy::detectPlayer(const sf::Vector2f& p) const {
    sf::FloatRect b = sprite_.getGlobalBounds();
    sf::Vector2f center(b.left + b.width / 2.f, b.top + b.height / 2.f);

    float distSq = math::getDistSq(p, center);

    return distSq < 450.f * 450.f;
}

void Enemy::patrol(float dt, const Map& map) {
    float currentSpeed = speed_.get();
    sf::Vector2f moveVec = direction_ * currentSpeed * dt;
    sf::FloatRect nextBounds = getBounds();
    nextBounds.left += moveVec.x;
    nextBounds.top += moveVec.y;

    if (map.collidesWithWall(nextBounds)) {
        int dir = std::rand() % 4;
        switch (dir) {
            case 0: direction_ = {1.f, 0.f}; break;
            case 1: direction_ = {-1.f, 0.f}; break;
            case 2: direction_ = {0.f, 1.f}; break;
            case 3: direction_ = {0.f, -1.f}; break;
        }
    } else {
        move(moveVec.x, moveVec.y);
    }
}

void Enemy::chase(float dt, const sf::Vector2f& target, const Map& map) {
    sf::FloatRect b = sprite_.getGlobalBounds();
    sf::Vector2f center(b.left + b.width / 2.f, b.top + b.height / 2.f);

    float dx = target.x - center.x;
    float dy = target.y - center.y;
    float len = std::sqrt(dx * dx + dy * dy);

    if (len > 0.f) {
        dx /= len;
        dy /= len;
    }

    float currentSpeed = speed_.get();
    float moveAmt = currentSpeed * dt * 1.3f;
    float stepX = dx * moveAmt;
    float stepY = dy * moveAmt;

    float currX = pos_.getX();
    float currY = pos_.getY();

    sf::FloatRect bounds = sprite_.getGlobalBounds();
    float margin = 20.f;
    float boxW = bounds.width - 2 * margin;
    float boxH = bounds.height - 2 * margin;

    sf::FloatRect testX(currX + stepX + margin, currY + margin + 1.f, boxW, boxH - 2.f);
    bool canMoveX = !map.collidesWithWall(testX);

    float nextX = canMoveX ? (currX + stepX) : currX;
    sf::FloatRect testY(nextX + margin + 1.f, currY + stepY + margin, boxW - 2.f, boxH);
    bool canMoveY = !map.collidesWithWall(testY);

    if (canMoveX) currX += stepX;
    if (canMoveY) currY += stepY;

    pos_.set(currX, currY);
    sprite_.setPosition(currX, currY);
}

void Enemy::updateAI(float dt, const sf::Vector2f& playerPos, const Map& map, bool isPlayerHidden) {
    sf::FloatRect b = sprite_.getGlobalBounds();
    sf::Vector2f center(b.left + b.width / 2.f, b.top + b.height / 2.f);
    sf::Vector2f currentTarget = playerPos;

    if (distracted_) {
        state_ = State::Chase;
        currentTarget = distractPos_;
    }
    else {
        if (isPlayerHidden) {
            state_ = State::Patrol;
            path_.clear();
        }
        else if (state_ == State::Patrol) {
            if (detectPlayer(playerPos)) {
                state_ = State::Chase;
            }
        }
        else if (state_ == State::Chase) {
            if (!detectPlayer(playerPos)) {
                state_ = State::Patrol;
                path_.clear();
            }
        }
    }

    if (state_ == State::Chase) {
        pathTimer_ += dt;
        if (pathTimer_ > 0.2f) {
            pathTimer_ = 0.f;
            path_ = map.findPath(center, currentTarget);
        }

        if (!path_.empty()) {
            sf::Vector2f nextPoint = path_.front();

            float distSq = math::getDistSq(nextPoint, center);

            if (distSq < 15.f * 15.f) {
                path_.erase(path_.begin());
                if (!path_.empty()) nextPoint = path_.front();
            }

            if (!path_.empty()) {
                chase(dt, nextPoint, map);
            }
        } else {
             chase(dt, currentTarget, map);
        }

    } else {
        patrol(dt, map);
    }
}

void Enemy::reset() {
    hp_.reset();
    pos_.set(0.f, 0.f);
    path_.clear();
    pathTimer_ = 0.f;
    stuckTimer_ = 0.f;
    lastPos_ = {0.f, 0.f};
    state_ = State::Patrol;
}

void Enemy::loadTexture() {
    sf::Texture& tex = TextureManager::getInstance().getTexture(texturePath_);
    sprite_.setTexture(tex, true);

    auto sz = tex.getSize();
    sprite_.setScale(48.f / sz.x, 48.f / sz.y);
    sprite_.setPosition(pos_.getX(), pos_.getY());
}

void Enemy::distractTo(const sf::Vector2f& p, float sec) {
    distractPos_ = p;
    distractTimer_ = sec;
    distracted_ = (sec > 0.f);
}