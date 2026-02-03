#include "../header/Decoy.h"

sf::Texture Decoy::texture_;
bool Decoy::textureLoaded_ = false;

Decoy::Decoy() : lifetime_(0.f), active_(false) {
    if (!textureLoaded_) {
        if (!texture_.loadFromFile("textures/Budinca.png")) {
            texture_.loadFromFile("../textures/Budinca.png");
        }
        textureLoaded_ = true;
    }

    shape_.setRadius(20.f);
    shape_.setOrigin(20.f, 20.f);

    shape_.setTexture(&texture_);
}

Decoy::~Decoy() = default;

void Decoy::spawn(const sf::Vector2f& pos, float duration, const sf::Vector2f& velocity) {
    active_ = true;
    lifetime_ = duration;
    shape_.setPosition(pos);
    velocity_ = velocity;
}

void Decoy::update(float dt, const Map& map) {
    if (!active_) return;

    lifetime_ -= dt;
    if (lifetime_ <= 0.f) {
        active_ = false;
        return;
    }

    if (velocity_.x != 0.f || velocity_.y != 0.f) {
        sf::Vector2f nextPos = shape_.getPosition() + velocity_ * dt;
        sf::FloatRect nextBounds(nextPos.x - 30.f, nextPos.y - 30.f, 60.f, 60.f);

        if (map.collidesWithWall(nextBounds, true)) {
            velocity_ = {0.f, 0.f};
        } else {
            shape_.setPosition(nextPos);
        }
    }
}

void Decoy::draw(sf::RenderWindow& window) const {
    if (active_) {
        window.draw(shape_);
    }
}

bool Decoy::active() const { return active_; }
sf::Vector2f Decoy::position() const { return shape_.getPosition(); }