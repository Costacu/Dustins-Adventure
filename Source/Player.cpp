#include "../header/Player.h"
#include "../header/Exceptions.h"
#include "../header/TextureManager.h"
#include <filesystem>
#include <algorithm>

Player::Player(std::string name, int hp, float speed, std::string texturePath)
    : Entity(0.f, 0.f),
      name_(std::move(name)),
      texturePath_(std::move(texturePath)),
      hp_(hp),
      speed_(speed),
      decoyCount_(5),
      isHidden_(false),
      isExitingCloset_(false),
      hasShovel_(false)
{
    loadTexture();
}

Player::Player(const Player& other)
    : Entity(other),
      name_(other.name_),
      texturePath_(other.texturePath_),
      hp_(other.hp_),
      speed_(other.speed_),
      decoyCount_(other.decoyCount_),
      isHidden_(other.isHidden_),
      isExitingCloset_(other.isExitingCloset_),
      hasShovel_(other.hasShovel_)
{
    sprite_ = other.sprite_;
}

Player& Player::operator=(const Player& other) {
    if (this != &other) {
        Entity::operator=(other);
        name_ = other.name_;
        texturePath_ = other.texturePath_;
        hp_ = other.hp_;
        speed_ = other.speed_;
        sprite_ = other.sprite_;
        decoyCount_ = other.decoyCount_;
        isHidden_ = other.isHidden_;
        isExitingCloset_ = other.isExitingCloset_;
        hasShovel_ = other.hasShovel_;
    }
    return *this;
}

Player::~Player() = default;

[[maybe_unused]] Entity* Player::clone() const {
    return new Player(*this);
}

void Player::print(std::ostream& os) const {
    os << "Player(name=" << name_
       << ", hp=" << hp_.get() << "/" << hp_.getMax()
       << ", speed=" << speed_.get();
}

void Player::setMap(const Map* map) {
    mapRef_ = map;
}

void Player::update(float dt) {
    if (isHidden_) return;

    float dx = 0.f;
    float dy = 0.f;

    float currentSpeed = speed_.get();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) dx -= currentSpeed * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) dx += currentSpeed * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) dy -= currentSpeed * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) dy += currentSpeed * dt;

    if (dx == 0.f && dy == 0.f) return;

    if (mapRef_) {
        move(dx, 0.f);
        if (mapRef_->collidesWithWall(getBounds(), isExitingCloset_)) {
            move(-dx, 0.f);
        }

        move(0.f, dy);
        if (mapRef_->collidesWithWall(getBounds(), isExitingCloset_)) {
            move(0.f, -dy);
        }

        if (isExitingCloset_) {
            bool intersectsCloset = false;
            const auto& closets = mapRef_->getClosets();
            for (const auto& c : closets) {
                if (getBounds().intersects(c.getGlobalBounds())) {
                    intersectsCloset = true;
                    break;
                }
            }
            if (!intersectsCloset) {
                isExitingCloset_ = false;
            }
        }
    } else {
        move(dx, dy);
    }
}

void Player::reset() {
    hp_.reset();
    decoyCount_ = 5;
    isHidden_ = false;
    isExitingCloset_ = false;
    hasShovel_ = false;
    setPosition(0.f, 0.f);
    sprite_.setColor(sf::Color::White);
}

void Player::draw(sf::RenderWindow& window) const {
    Entity::draw(window);
}

bool Player::useDecoy() {
    if (decoyCount_ > 0) {
        decoyCount_--;
        return true;
    }
    return false;
}

int Player::getDecoyCount() const {
    return decoyCount_;
}

void Player::addDecoys(int count) {
    decoyCount_ += count;
    if (decoyCount_ > 5) decoyCount_ = 5;
}

void Player::setHidden(bool hidden) {
    isHidden_ = hidden;
    if (isHidden_) {
        sprite_.setColor(sf::Color(100, 100, 100, 150));
    } else {
        sprite_.setColor(sf::Color::White);
    }
}

bool Player::isHidden() const {
    return isHidden_;
}

void Player::setExitingCloset(bool exiting) {
    isExitingCloset_ = exiting;
}

bool Player::isExitingCloset() const {
    return isExitingCloset_;
}

bool Player::hasShovel() const { return hasShovel_; }
void Player::collectShovel() { hasShovel_ = true; }

void Player::loadTexture() {
    sf::Texture& tex = TextureManager::getInstance().getTexture(texturePath_);
    sprite_.setTexture(tex, true);

    auto sz = tex.getSize();
    sprite_.setScale(48.f / sz.x, 48.f / sz.y);
    sprite_.setPosition(pos_.getX(), pos_.getY());
}