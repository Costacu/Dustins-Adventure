#include "../header/Player.h"
#include "../header/TextureManager.h"
#include <iostream>
#include <algorithm>

Player::Player(std::string name, int hp, float speed, std::string texturePath)
    : Entity(0.f, 0.f),
      name_(std::move(name)),
      texturePath_(std::move(texturePath)),
      hp_(hp),
      speed_(speed),
      decoyCount_(5),
      hasShovel_(false),
      isHidden_(false),
      isExitingCloset_(false)
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
      hasShovel_(other.hasShovel_),
      isHidden_(other.isHidden_),
      isExitingCloset_(other.isExitingCloset_),
      map_(other.map_)
{
    loadTexture();
}

Player& Player::operator=(const Player& other) {
    if (this != &other) {
        Entity::operator=(other);
        name_ = other.name_;
        texturePath_ = other.texturePath_;
        hp_ = other.hp_;
        speed_ = other.speed_;
        decoyCount_ = other.decoyCount_;
        hasShovel_ = other.hasShovel_;
        isHidden_ = other.isHidden_;
        isExitingCloset_ = other.isExitingCloset_;
        map_ = other.map_;
        loadTexture();
    }
    return *this;
}

Player::~Player() = default;



void Player::print(std::ostream& os) const {
    os << "Player(name=" << name_ << ", decoys=" << decoyCount_ << ", shovel=" << hasShovel_ << ")";
}

sf::FloatRect Player::getBounds() const {
    sf::FloatRect b = sprite_.getGlobalBounds();
    float margin = 10.f;
    return sf::FloatRect(b.left + margin, b.top + margin, b.width - 2 * margin, b.height - 2 * margin);
}

void Player::setMap(Map* m) { map_ = m; }

void Player::update(float dt) {
    if (isHidden_) return;

    sf::Vector2f movement(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) movement.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) movement.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) movement.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) movement.x += 1.f;

    if (movement.x != 0.f || movement.y != 0.f) {
        float currentSpeed = speed_.get();

        move(movement.x * currentSpeed * dt, 0.f);
        if (map_ && map_->collidesWithWall(getBounds(), isExitingCloset_)) {
            move(-movement.x * currentSpeed * dt, 0.f);
        }

        move(0.f, movement.y * currentSpeed * dt);
        if (map_ && map_->collidesWithWall(getBounds(), isExitingCloset_)) {
            move(0.f, -movement.y * currentSpeed * dt);
        }

        if (isExitingCloset_ && map_) {
            const auto& closets = map_->getClosets();
            bool stillIntersecting = std::any_of(closets.begin(), closets.end(), [&](const sf::RectangleShape& c) {
                return getBounds().intersects(c.getGlobalBounds());
            });

            if (!stillIntersecting) {
                isExitingCloset_ = false;
            }
        }
    }
}

void Player::addDecoys(int count) {
    decoyCount_ += count;
    if (decoyCount_ > 5) decoyCount_ = 5;
}

bool Player::useDecoy() {
    if (decoyCount_ > 0) {
        decoyCount_--;
        return true;
    }
    return false;
}
int Player::getDecoyCount() const { return decoyCount_; }

void Player::collectShovel() { hasShovel_ = true; }
bool Player::hasShovel() const { return hasShovel_; }

void Player::setHidden(bool h) {
    isHidden_ = h;
    if (isHidden_) sprite_.setColor(sf::Color(255, 255, 255, 100));
    else sprite_.setColor(sf::Color::White);
}
bool Player::isHidden() const { return isHidden_; }

void Player::setExitingCloset(bool e) { isExitingCloset_ = e; }

void Player::reset() {
    hp_.reset();
    decoyCount_ = 5;
    hasShovel_ = false;
    isHidden_ = false;
    isExitingCloset_ = false;
    sprite_.setColor(sf::Color::White);
    setPosition(0.f, 0.f);
}

void Player::loadTexture() {
    sf::Texture& tex = TextureManager::getInstance().getTexture(texturePath_);
    sprite_.setTexture(tex);

    auto sz = tex.getSize();
    sprite_.setScale(48.f / sz.x, 48.f / sz.y);
}