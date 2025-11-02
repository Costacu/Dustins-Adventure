#include "../header/Enemy.h"


#include "../header/Enemy.h"
#include <cmath>

Enemy::Enemy(std::string name, int hp, float speed, std::string texturePath)
    : name_(std::move(name)), texturePath_(std::move(texturePath)), hp_(hp), maxHp_(hp), speed_(speed) {
    pos_.x = 0.f;
    pos_.y = 0.f;
    loadTexture();
    sprite_.setPosition(pos_.x, pos_.y);
}

Enemy::Enemy(const Enemy& other)
    : name_(other.name_),
      texturePath_(other.texturePath_),
      hp_(other.hp_),
      maxHp_(other.maxHp_),
      speed_(other.speed_),
      pos_(other.pos_) {
    loadTexture();
    sprite_.setPosition(pos_.x, pos_.y);
}

Enemy& Enemy::operator=(const Enemy& other) {
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

Enemy::~Enemy() {}

void Enemy::update(float dt, const sf::Vector2f& playerPos, const sf::FloatRect& playArea) {
    float dx = playerPos.x - pos_.x;
    float dy = playerPos.y - pos_.y;
    float len = std::sqrt(dx * dx + dy * dy);
    if (len > 0.f) {
        dx /= len;
        dy /= len;
        pos_.x += dx * speed_ * dt;
        pos_.y += dy * speed_ * dt;
    }
    sf::FloatRect b = sprite_.getGlobalBounds();
    if (pos_.x < playArea.left) pos_.x = playArea.left;
    if (pos_.y < playArea.top) pos_.y = playArea.top;
    if (pos_.x + b.width > playArea.left + playArea.width) pos_.x = playArea.left + playArea.width - b.width;
    if (pos_.y + b.height > playArea.top + playArea.height) pos_.y = playArea.top + playArea.height - b.height;
    sprite_.setPosition(pos_.x, pos_.y);
}

void Enemy::takeDamage(int dmg) {
    if (dmg < 0) dmg = 0;
    hp_ -= dmg;
    clampHp();
}

void Enemy::reset() {
    hp_ = maxHp_;
    pos_.x = 0.f;
    pos_.y = 0.f;
    sprite_.setPosition(pos_.x, pos_.y);
}

bool Enemy::isAlive() const { return hp_ > 0; }
const std::string& Enemy::getName() const { return name_; }
const std::string& Enemy::getTexturePath() const { return texturePath_; }
int Enemy::getHp() const { return hp_; }
int Enemy::getMaxHp() const { return maxHp_; }
float Enemy::getSpeed() const { return speed_; }
const Enemy::Position& Enemy::getPosition() const { return pos_; }

// Enemy.cpp
sf::FloatRect Enemy::getBounds() const {
    sf::FloatRect b = sprite_.getGlobalBounds();
    if (b.width <= 0.f || b.height <= 0.f)
        return sf::FloatRect(pos_.x, pos_.y, 48.f, 48.f);
    return b;
}


void Enemy::setPosition(float newX, float newY) {
    pos_.x = newX;
    pos_.y = newY;
    sprite_.setPosition(pos_.x, pos_.y);
}

void Enemy::move(float dx, float dy) {
    pos_.x += dx;
    pos_.y += dy;
    sprite_.setPosition(pos_.x, pos_.y);
}

void Enemy::draw(sf::RenderWindow& window) const {
    window.draw(sprite_);
}

void Enemy::loadTexture() {
    texture_.loadFromFile(texturePath_);
    sprite_.setTexture(texture_);
}

void Enemy::clampHp() {
    if (hp_ < 0) hp_ = 0;
    if (hp_ > maxHp_) hp_ = maxHp_;
}
