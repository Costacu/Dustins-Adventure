#include "../header/Player.h"


#include <iostream>

Player::Player(std::string name, int hp, float speed, std::string texturePath)
    : name_(std::move(name)), texturePath_(std::move(texturePath)), hp_(hp), maxHp_(hp), speed_(speed) {
    pos_.x = 0.f;
    pos_.y = 0.f;
    loadTexture();
    sprite_.setPosition(pos_.x, pos_.y);
}

Player::Player(const Player& other)
    : name_(other.name_),
      texturePath_(other.texturePath_),
      hp_(other.hp_),
      maxHp_(other.maxHp_),
      speed_(other.speed_),
      pos_(other.pos_) {
    loadTexture();
    sprite_.setPosition(pos_.x, pos_.y);
}

Player& Player::operator=(const Player& other) {
    if (this != &other) {
        name_ = other.name_;
        texturePath_ = other.texturePath_;
        hp_ = other.hp_;
        maxHp_ = other.maxHp_;
        speed_ = other.speed_;
        pos_ = other.pos_;
        loadTexture();
        sprite_.setPosition(pos_.x, pos_.y);
    }
    return *this;
}

Player::~Player() {}

void Player::input() {}

void Player::update(float dt) {
    float dx = 0.f, dy = 0.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) dx -= speed_ * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) dx += speed_ * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) dy -= speed_ * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) dy += speed_ * dt;
    move(dx, dy);
}

void Player::takeDamage(int dmg) {
    if (dmg < 0) dmg = 0;
    hp_ -= dmg;
    clampHp();
}

void Player::reset() {
    hp_ = maxHp_;
    pos_.x = 0.f;
    pos_.y = 0.f;
    sprite_.setPosition(pos_.x, pos_.y);
}

bool Player::isAlive() const { return hp_ > 0; }
const std::string& Player::getName() const { return name_; }
const std::string& Player::getTexturePath() const { return texturePath_; }
int Player::getHp() const { return hp_; }
int Player::getMaxHp() const { return maxHp_; }
float Player::getSpeed() const { return speed_; }
const Player::Position& Player::getPosition() const { return pos_; }


sf::FloatRect Player::getBounds() const {
    sf::FloatRect b = sprite_.getGlobalBounds();
    if (b.width <= 0.f || b.height <= 0.f)
        return sf::FloatRect(pos_.x, pos_.y, 48.f, 48.f);
    return b;
}


void Player::setPosition(float newX, float newY) {
    pos_.x = newX;
    pos_.y = newY;
    sprite_.setPosition(pos_.x, pos_.y);
}

void Player::move(float dx, float dy) {
    pos_.x += dx;
    pos_.y += dy;
    sprite_.setPosition(pos_.x, pos_.y);
}

void Player::loadTexture() {
    texture_.loadFromFile(texturePath_);
    sprite_.setTexture(texture_);
}

void Player::clampHp() {
    if (hp_ < 0) hp_ = 0;
    if (hp_ > maxHp_) hp_ = maxHp_;
}

std::ostream& operator<<(std::ostream& os, const Player::Position& p) {
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Player& pl) {
    os << "Player{name=\"" << pl.getName() << "\", hp=" << pl.getHp() << "/" << pl.getMaxHp()
       << ", speed=" << pl.getSpeed() << ", pos=" << pl.getPosition() << ", texture=\"" << pl.getTexturePath() << "\"}";
    return os;
}
