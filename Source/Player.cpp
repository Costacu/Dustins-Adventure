#include "../header/Player.h"
#include "../header/Exceptions.h"
#include <filesystem>
#include <algorithm>

Player::Player(std::string name, int hp, float speed, std::string texturePath)
    : Entity(0.f, 0.f),
      name_(std::move(name)),
      texturePath_(std::move(texturePath)),
      hp_(hp),
      maxHp_(hp),
      speed_(speed)
{
    loadTexture();
}

Player::Player(const Player& other)
    : Entity(other),
      name_(other.name_),
      texturePath_(other.texturePath_),
      hp_(other.hp_),
      maxHp_(other.maxHp_),
      speed_(other.speed_)
{
    sprite_ = other.sprite_;
}

Player& Player::operator=(const Player& other) {
    if (this != &other) {
        Entity::operator=(other);
        name_ = other.name_;
        texturePath_ = other.texturePath_;
        hp_ = other.hp_;
        maxHp_ = other.maxHp_;
        speed_ = other.speed_;
        sprite_ = other.sprite_;
    }
    return *this;
}

Player::~Player() = default;

[[maybe_unused]] Entity* Player::clone() const {
    return new Player(*this);
}

void Player::print(std::ostream& os) const {
    os << "Player(name=" << name_
       << ", hp=" << hp_ << "/" << maxHp_
       << ", speed=" << speed_;
}

void Player::update(float dt) {
    float dx = 0.f, dy = 0.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) dx -= speed_ * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) dx += speed_ * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) dy -= speed_ * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) dy += speed_ * dt;

    move(dx, dy); // folosim direct metoda din Entity
}

void Player::reset() {
    hp_ = maxHp_;
    setPosition(0.f, 0.f);
}

void Player::draw(sf::RenderWindow& window) const {
    Entity::draw(window);
}


void Player::loadTexture() {
    std::vector<std::string> paths = {
        texturePath_,
        "textures/" + texturePath_,
        "../textures/" + texturePath_
    };

    bool loaded = false;
    for (auto& p : paths) {
        if (texture_.loadFromFile(p)) {
            loaded = true;
            break;
        }
    }

    if (!loaded)
        throw FileLoadError(texturePath_);

    texture_.setSmooth(true);
    sprite_.setTexture(texture_, true);

    auto sz = texture_.getSize();
    sprite_.setScale(48.f / sz.x, 48.f / sz.y);
    sprite_.setPosition(pos_.getX(), pos_.getY());
}
