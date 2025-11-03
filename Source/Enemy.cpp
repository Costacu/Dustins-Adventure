#include "../header/Enemy.h"
#include <cmath>
#include <filesystem>
using std::string;

Enemy::Enemy(std::string name, int hp, float speed, std::string texturePath)
    : name_(std::move(name)), texturePath_(std::move(texturePath)), hp_(hp), maxHp_(hp), speed_(speed) {
    pos_.set(0.f, 0.f);
    loadTexture();
    sprite_.setPosition(pos_.getX(), pos_.getY());

    distracted_ = false;
    distractTimer_ = 0.f;
    distractPos_ = sf::Vector2f(0.f,0.f);

}

Enemy::Enemy(const Enemy& other)
    : name_(other.name_),
      texturePath_(other.texturePath_),
      hp_(other.hp_),
      maxHp_(other.maxHp_),
      speed_(other.speed_),
      pos_(other.pos_) {
    loadTexture();
    sprite_.setPosition(pos_.getX(), pos_.getY());
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
        sprite_.setPosition(pos_.getX(), pos_.getY());
        distractTimer_ = other.distractTimer_;
    }
    return *this;
}

Enemy::~Enemy() = default;

void Enemy::update(float dt, const sf::Vector2f& playerPos, const sf::FloatRect& playArea) {
    if (distracted_) {
        distractTimer_ -= dt;
        if (distractTimer_ <= 0.f) {
            distracted_ = false;
        }
    }

    sf::Vector2f target = distracted_ ? distractPos_ : playerPos;

    float dx = target.x - pos_.getX();
    float dy = target.y - pos_.getY();
    float len = std::sqrt(dx * dx + dy * dy);
    if (len > 0.f) {
        dx /= len;
        dy /= len;
        pos_.set(pos_.getX() + dx * speed_ * dt, pos_.getY() + dy * speed_ * dt);
    }

    if (distracted_ && len < 8.f) {
        distracted_ = false;
        distractTimer_ = 0.f;
    }

    sf::FloatRect b = sprite_.getGlobalBounds();
    if (pos_.getX() < playArea.left) pos_.setX(playArea.left);
    if (pos_.getY() < playArea.top) pos_.setY(playArea.top);
    if (pos_.getX() + b.width > playArea.left + playArea.width) pos_.setX(playArea.left + playArea.width - b.width);
    if (pos_.getY() + b.height > playArea.top + playArea.height) pos_.setY(playArea.top + playArea.height - b.height);

    sprite_.setPosition(pos_.getX(), pos_.getY());
}

void Enemy::reset() {
    hp_ = maxHp_;
    pos_.set(0.f, 0.f);
    sprite_.setPosition(pos_.getX(), pos_.getY());
}

const std::string& Enemy::getName() const { return name_; }
const std::string& Enemy::getTexturePath() const { return texturePath_; }
int Enemy::getHp() const { return hp_; }
int Enemy::getMaxHp() const { return maxHp_; }
float Enemy::getSpeed() const { return speed_; }
const Enemy::Position& Enemy::getPosition() const { return pos_; }

sf::FloatRect Enemy::getBounds() const {
    sf::FloatRect b = sprite_.getGlobalBounds();
    if (b.width <= 0.f || b.height <= 0.f)
        return sf::FloatRect(pos_.getX(), pos_.getY(), 48.f, 48.f);
    return b;
}


void Enemy::setPosition(float newX, float newY) {
    pos_.set(newX, newY);
    sprite_.setPosition(pos_.getX(), pos_.getY());
}

void Enemy::move(float dx, float dy) {
    pos_.translate(dx, dy);
    sprite_.setPosition(pos_.getX(), pos_.getY());
}

void Enemy::draw(sf::RenderWindow& window) const {
    window.draw(sprite_);
}

void Enemy::loadTexture() {
    namespace fs = std::filesystem;

    const string name = texturePath_;
    std::vector<string> candidates;

    candidates.push_back(name);
    if (name.find('/') == string::npos && name.find('\\') == string::npos) {
        candidates.push_back(string("textures/") + name);
        candidates.push_back(string("../textures/") + name);
    }


    bool loaded = false;
    for (const auto& p : candidates) {
        if (texture_.loadFromFile(p)) { loaded = true; break; }
    }

    if (!loaded) {
        sf::Image img; img.create(48, 48, sf::Color(255, 120, 120));  // fallback red box
        texture_.loadFromImage(img);
    }

    texture_.setSmooth(true);
    sprite_.setTexture(texture_);

    auto sz = texture_.getSize();
    const float W = 48.f, H = 48.f;
    if (sz.x > 0 && sz.y > 0) {
        sprite_.setScale(W / static_cast<float>(sz.x), H / static_cast<float>(sz.y));
    }
    sprite_.setPosition(pos_.getX(), pos_.getY());
}


void Enemy::distractTo(const sf::Vector2f& pos, float seconds) {
    distractPos_ = pos;
    distractTimer_ = seconds;
    distracted_ = (seconds > 0.f);
}
