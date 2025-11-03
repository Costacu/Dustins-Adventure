#pragma once
#ifndef DECOY_H
#define DECOY_H
#include <SFML/Graphics.hpp>

class Decoy {
public:
    Decoy()
        : pos_(0.f, 0.f), lifetime_(0.f), radius_(10.f) {
        shape_.setRadius(radius_);
        shape_.setFillColor(sf::Color(180, 180, 180));
        shape_.setOrigin(radius_, radius_);
    }

    // Spawn decoy at player's center and stay there
    void spawn(const sf::Vector2f& playerCenter, float lifetime) {
        pos_ = playerCenter;
        lifetime_ = lifetime;
        shape_.setPosition(pos_);
    }

    void update(float dt, const sf::FloatRect&) {
        if (lifetime_ > 0.f)
            lifetime_ -= dt;
    }

    bool active() const { return lifetime_ > 0.f; }
    const sf::Vector2f& position() const { return pos_; }
    void draw(sf::RenderWindow& window) const {
        if (active()) window.draw(shape_);
    }

private:
    sf::Vector2f pos_;
    float lifetime_;
    float radius_;
    sf::CircleShape shape_;
};

#endif // DECOY_H