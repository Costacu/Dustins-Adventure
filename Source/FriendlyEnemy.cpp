#include "../header/FriendlyEnemy.h"
#include <algorithm>

FriendlyEnemy::FriendlyEnemy(std::string name, int hp, float speed, std::string texturePath)
    : Entity(0.f, 0.f),
      name_(std::move(name)),
      hp_(hp),
      maxHp_(hp),
      speed_(speed),
      texturePath_(std::move(texturePath)),
      direction_(1.f, 0.f),
      distractPos_(0.f, 0.f)
{
    loadTexture();
}

FriendlyEnemy::FriendlyEnemy(const FriendlyEnemy& other)
    : Entity(other),
      name_(other.name_),
      hp_(other.hp_),
      maxHp_(other.maxHp_),
      speed_(other.speed_),
      texturePath_(other.texturePath_),
      state_(other.state_),
      direction_(other.direction_),
      distractPos_(other.distractPos_)
{
    loadTexture();
}

FriendlyEnemy& FriendlyEnemy::operator=(const FriendlyEnemy& other) {
    if (this != &other) {
        Entity::operator=(other);
        name_ = other.name_;
        hp_ = other.hp_;
        maxHp_ = other.maxHp_;
        speed_ = other.speed_;
        texturePath_ = other.texturePath_;
        state_ = other.state_;
        direction_ = other.direction_;
        distractPos_ = other.distractPos_;
        loadTexture();
    }
    return *this;
}

FriendlyEnemy::~FriendlyEnemy() {}

Entity* FriendlyEnemy::clone() const {
    return new FriendlyEnemy(*this);
}

void FriendlyEnemy::print(std::ostream& os) const {
    os << "FriendlyEnemy(name=" << name_ << ", speed=" << speed_ << ")";
}

void FriendlyEnemy::patrol(float dt, const Map& map) {
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

void FriendlyEnemy::update(float dt) {
    (void)dt;
}


void FriendlyEnemy::updateAI(float dt, const sf::Vector2f&, const Map& map) {
    patrol(dt, map);
}

void FriendlyEnemy::reset() {
    hp_ = maxHp_;
    pos_.set(0.f, 0.f);
}

void FriendlyEnemy::loadTexture() {
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
