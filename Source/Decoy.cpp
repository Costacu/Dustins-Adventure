#include "../header/Decoy.h"

//constructor in clasa derivata
Decoy::Decoy()
    : Entity(0.f, 0.f),
      lifetime_(0.f),
      radius_(10.f)
{
    shape_.setRadius(radius_);
    shape_.setFillColor(sf::Color(180, 180, 180));
    shape_.setOrigin(radius_, radius_);

    shape_.setPosition(pos_.getX(), pos_.getY());
}

void Decoy::spawn(const sf::Vector2f& playerCenter, float lifetime) {
    pos_.set(playerCenter.x, playerCenter.y);
    lifetime_ = lifetime;
    shape_.setPosition(playerCenter);
}

Entity* Decoy::clone() const {
    return new Decoy(*this);
}

void Decoy::print(std::ostream& os) const {
    os << "Decoy(pos=" << pos_.getX()
       << "," << pos_.getY()
       << ", lifetime=" << lifetime_ << ")";
}

void Decoy::update(float dt) {
    if (lifetime_ > 0.f)
        lifetime_ -= dt;
}

bool Decoy::active() const {
    return lifetime_ > 0.f;
}

const sf::Vector2f& Decoy::position() const {
    return shape_.getPosition();
}

void Decoy::draw(sf::RenderWindow& window) const {
    if (active())
        window.draw(shape_);
}
