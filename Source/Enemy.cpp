#include "../header/Enemy.h"
#include <cmath>
#include <filesystem>
#include <vector>
#include <algorithm>
using std::string;

//constructor in clasa derivata
Enemy::Enemy(std::string name, int hp, float speed, std::string texturePath)
    : Entity(0.f, 0.f),
      name_(std::move(name)),
      texturePath_(std::move(texturePath)),
      hp_(hp),
      maxHp_(hp),
      speed_(speed),
      distractPos_(0.f, 0.f)
{
    loadTexture();
}


Enemy::Enemy(const Enemy& other)
    : Entity(0.f, 0.f),
      name_(other.name_),
      texturePath_(other.texturePath_),
      hp_(other.hp_),
      maxHp_(other.maxHp_),
      speed_(other.speed_),
      distracted_(other.distracted_),
      distractTimer_(other.distractTimer_),
      distractPos_(other.distractPos_) {
    pos_ = other.pos_;
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
        distracted_ = other.distracted_;
        distractPos_ = other.distractPos_;
    }
    return *this;
}

Entity* Enemy::clone() const {
    return new Enemy(*this);
}

Enemy::~Enemy() = default;

void Enemy::update(float dt) {
    if (distracted_) {
        distractTimer_ -= dt;
        if (distractTimer_ <= 0.f)
            distracted_ = false;
    }
}

void Enemy::print(std::ostream& os) const {
    os << "Enemy(name=" << name_ << ", speed=" << speed_;

}



void Enemy::chase(float dt, const sf::Vector2f& playerPos, const sf::FloatRect& playArea) {
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
    setPosition(0.f, 0.f);
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

    const bool loaded = std::any_of(candidates.begin(), candidates.end(),
        [this](const std::string& p) {
            return texture_.loadFromFile(p);
        });

    if (!loaded) {
        sf::Image img; img.create(48, 48, sf::Color(255, 120, 120));
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
