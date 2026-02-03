#include "../header/Decoy.h"

Decoy::Decoy()
    : timer_(0.f), isActive_(false), velocity_(0.f, 0.f), isMoving_(false)
{
    shape_.setRadius(10.f);
    shape_.setFillColor(sf::Color::White);
    shape_.setOrigin(10.f, 10.f);
}

void Decoy::spawn(const sf::Vector2f& pos, float duration, const sf::Vector2f& vel) {
    shape_.setPosition(pos);
    timer_ = duration;
    isActive_ = true;
    velocity_ = vel;
    isMoving_ = (std::abs(vel.x) > 0.1f || std::abs(vel.y) > 0.1f);
}

void Decoy::update(float dt, const Map& map) {
    if (!isActive_) return;

    if (isMoving_) {
        sf::Vector2f nextPos = shape_.getPosition() + velocity_ * dt;

        sf::FloatRect bounds(nextPos.x - 5.f, nextPos.y - 5.f, 10.f, 10.f);

        if (map.collidesWithWall(bounds)) {
            isMoving_ = false;
            velocity_ = {0.f, 0.f};
        } else {
            shape_.setPosition(nextPos);
        }
    }

    timer_ -= dt;
    if (timer_ <= 0.f) {
        isActive_ = false;
    }
}

void Decoy::draw(sf::RenderWindow& window) const {
    if (isActive_) {
        window.draw(shape_);
    }
}

bool Decoy::active() const {
    return isActive_;
}

sf::Vector2f Decoy::position() const {
    return shape_.getPosition();
}