#include "../header/Enemy.h"
#include <cmath>
#include <algorithm>

Enemy::Enemy(std::string name, int hp, float speed, std::string texturePath)
    : Entity(0.f, 0.f),
      name_(std::move(name)),
      hp_(hp),
      maxHp_(hp),
      speed_(speed),
      texturePath_(std::move(texturePath)),
      state_(State::Patrol),
      direction_(1.f, 0.f),
      distracted_(false),
      distractTimer_(0.f),
      distractPos_(0.f, 0.f)
{
    loadTexture();
}

Enemy::Enemy(const Enemy& other)
    : Entity(other),
      name_(other.name_),
      hp_(other.hp_),
      maxHp_(other.maxHp_),
      speed_(other.speed_),
      texturePath_(other.texturePath_),
      state_(other.state_),
      direction_(other.direction_),
      distracted_(other.distracted_),
      distractTimer_(other.distractTimer_),
      distractPos_(other.distractPos_)
{
    loadTexture();
}

Enemy& Enemy::operator=(const Enemy& other) {
    if (this != &other) {
        Entity::operator=(other);
        name_ = other.name_;
        hp_ = other.hp_;
        maxHp_ = other.maxHp_;
        speed_ = other.speed_;
        texturePath_ = other.texturePath_;
        state_ = other.state_;
        direction_ = other.direction_;
        distracted_ = other.distracted_;
        distractTimer_ = other.distractTimer_;
        distractPos_ = other.distractPos_;
        loadTexture();
    }
    return *this;
}

Enemy::~Enemy() {}

Entity* Enemy::clone() const {
    return new Enemy(*this);
}

void Enemy::print(std::ostream& os) const {
    os << "Enemy(name=" << name_ << ", speed=" << speed_ << ")";
}

void Enemy::update(float dt) {
    if (distracted_) {
        distractTimer_ -= dt;
        if (distractTimer_ <= 0.f)
            distracted_ = false;
    }
}

bool Enemy::detectPlayer(const sf::Vector2f& p) const {
    float dx = p.x - pos_.getX();
    float dy = p.y - pos_.getY();
    return dx * dx + dy * dy < 350.f * 350.f;
}

void Enemy::patrol(float dt, const Map& map) {
    sf::Vector2f newPos(
        pos_.getX() + direction_.x * speed_ * dt,
        pos_.getY() + direction_.y * speed_ * dt
    );

    sf::FloatRect future(newPos.x, newPos.y,
                         sprite_.getGlobalBounds().width,
                         sprite_.getGlobalBounds().height);

    if (map.collidesWithWall(future)) {
        float x = direction_.x;
        direction_.x = -direction_.y;
        direction_.y = x;
        return;
    }

    pos_.set(newPos.x, newPos.y);
    sprite_.setPosition(newPos);
}

void Enemy::chase(float dt, const sf::Vector2f& p, const Map& map) {
    float dx = p.x - pos_.getX();
    float dy = p.y - pos_.getY();
    float len = std::sqrt(dx * dx + dy * dy);

    if (len > 0.f) {
        dx /= len;
        dy /= len;
    }

    sf::Vector2f newPos(
        pos_.getX() + dx * speed_ * dt * 1.25f,
        pos_.getY() + dy * speed_ * dt * 1.25f
    );

    sf::FloatRect future(newPos.x, newPos.y,
                         sprite_.getGlobalBounds().width,
                         sprite_.getGlobalBounds().height);

    if (!map.collidesWithWall(future)) {
        pos_.set(newPos.x, newPos.y);
        sprite_.setPosition(newPos);
    }
}

void Enemy::updateAI(float dt, const sf::Vector2f& playerPos, const Map& map) {
    if (detectPlayer(playerPos))
        state_ = State::Chase;
    else
        state_ = State::Patrol;

    if (state_ == State::Chase)
        chase(dt, playerPos, map);
    else
        patrol(dt, map);
}

void Enemy::reset() {
    hp_ = maxHp_;
    pos_.set(0.f, 0.f);
}

void Enemy::loadTexture() {
    std::vector<std::string> paths = {
        texturePath_,
        "textures/" + texturePath_,
        "../textures/" + texturePath_
    };

    bool loaded = false;
    for (auto& p : paths)
        if (texture_.loadFromFile(p))
            loaded = true;

    if (!loaded) {
        sf::Image img;
        img.create(48, 48, sf::Color(255, 120, 120));
        texture_.loadFromImage(img);
    }

    texture_.setSmooth(true);
    sprite_.setTexture(texture_, true);

    auto sz = texture_.getSize();
    sprite_.setScale(48.f / sz.x, 48.f / sz.y);
    sprite_.setPosition(pos_.getX(), pos_.getY());
}

void Enemy::distractTo(const sf::Vector2f& p, float sec) {
    distractPos_ = p;
    distractTimer_ = sec;
    distracted_ = (sec > 0.f);
}
