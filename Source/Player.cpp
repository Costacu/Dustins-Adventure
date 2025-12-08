#include "../header/Player.h"
#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>
using std::string;

//constructor in clasa derivata
Player::Player(std::string name, int hp, float speed, std::string texturePath)
    : Entity(0.f, 0.f),
      name_(std::move(name)),
      texturePath_(std::move(texturePath)),
      hp_(hp), maxHp_(hp), speed_(speed)
{
    loadTexture();
}

Player::Player(const Player& other)
    : name_(other.name_),
      hp_(other.hp_),
      speed_(other.speed_),
      texturePath_(other.texturePath_),
      maxHp_(other.maxHp_) {
    pos_ = other.pos_;
    loadTexture();
    sprite_.setPosition(pos_.getX(), pos_.getY());
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
        sprite_.setPosition(pos_.getX(), pos_.getY());
    }
    return *this;
}

Entity* Player::clone() const {
    return new Player(*this);
}

void Player::print(std::ostream& os) const {
    os << "Player(name=" << name_
       << ", hp=" << hp_ << "/" << maxHp_
       << ", speed=" << speed_;
}


Player::~Player() = default;

void Player::update(const float dt) {
    float dx = 0.f, dy = 0.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) dx -= speed_ * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) dx += speed_ * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) dy -= speed_ * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) dy += speed_ * dt;
    move(dx, dy);
}

void Player::reset() {
    hp_ = maxHp_;
    setPosition(0.f, 0.f);
}

void Player::draw(sf::RenderWindow& window) const {
    Entity::draw(window);
}

const std::string& Player::getName() const { return name_; }
const std::string& Player::getTexturePath() const { return texturePath_; }
int Player::getHp() const { return hp_; }
int Player::getMaxHp() const { return maxHp_; }
float Player::getSpeed() const { return speed_; }
const Player::Position& Player::getPosition() const { return pos_; }

void Player::setPosition(float newX, float newY) {
    Entity::setPosition(newX, newY);
}

void Player::move(float dx, float dy) {
    Entity::move(dx, dy);
}

void Player::loadTexture() {
    namespace fs = std::filesystem;

    const string name = texturePath_;
    std::vector<string> candidates;
    candidates.push_back(name);
    if (name.find('/') == string::npos && name.find('\\') == string::npos) {
        candidates.push_back(string("textures/") + name);
        candidates.push_back(string("../textures/") + name);
    }

    // Use STL algorithm instead of a raw loop (matches cppcheck suggestion)
    const bool loaded = std::any_of(candidates.begin(), candidates.end(),
        [this](const std::string& p) {
            return texture_.loadFromFile(p);
        });

    if (!loaded) {
        sf::Image img; img.create(48, 48, sf::Color(200, 200, 255));
        texture_.loadFromImage(img);
    }

    texture_.setSmooth(true);
    sprite_.setTexture(texture_);

    const auto sz = texture_.getSize();
    const float W = 48.f, H = 48.f;
    if (sz.x > 0 && sz.y > 0) {
        sprite_.setScale(W / static_cast<float>(sz.x), H / static_cast<float>(sz.y));
    }
    sprite_.setPosition(pos_.getX(), pos_.getY());
}

std::ostream& operator<<(std::ostream& os, const Player::Position& p) {
    os << "(" << p.getX() << ", " << p.getY() << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Player& pl) {
    os << "Player{name=\"" << pl.getName() << "\", hp=" << pl.getHp() << "/" << pl.getMaxHp()
       << ", speed=" << pl.getSpeed() << ", pos=" << pl.getPosition()
       << ", texture=\"" << pl.getTexturePath() << "\"}";
    return os;
}
