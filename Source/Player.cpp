#include "../header/Player.h"
#include <iostream>
#include <filesystem>
using std::string;

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
    pos_.x = 0.f;
    pos_.y = 0.f;
    sprite_.setPosition(pos_.x, pos_.y);
}

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
    namespace fs = std::filesystem;

    const string name = texturePath_;                 // e.g. "Dustin.png" or "textures/Dustin.png"
    std::vector<string> candidates;

    candidates.push_back(name);                       // as provided
    if (name.find('/') == string::npos && name.find('\\') == string::npos) {
        candidates.push_back(string("textures/") + name);
        candidates.push_back(string("../textures/") + name);
    }
#ifdef GAME_ASSETS_DIR
    candidates.push_back((fs::path(GAME_ASSETS_DIR) / name).string());
#endif

    bool loaded = false;
    for (const auto& p : candidates) {
        if (texture_.loadFromFile(p)) { loaded = true; break; }
    }
    if (!loaded) {
        sf::Image img; img.create(48, 48, sf::Color(200, 200, 255));
        texture_.loadFromImage(img);
    }

    texture_.setSmooth(true);
    sprite_.setTexture(texture_);

    auto sz = texture_.getSize();
    const float W = 48.f, H = 48.f;
    if (sz.x > 0 && sz.y > 0) {
        sprite_.setScale(W / static_cast<float>(sz.x), H / static_cast<float>(sz.y));
    }
    sprite_.setPosition(pos_.x, pos_.y);
}


void Player::draw(sf::RenderWindow& window) const {
    window.draw(sprite_);
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
