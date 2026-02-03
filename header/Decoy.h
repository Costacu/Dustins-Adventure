#pragma once
#ifndef DECOY_H
#define DECOY_H

#include <SFML/Graphics.hpp>
#include "Map.h"

class Decoy {
public:
    Decoy();

    void spawn(const sf::Vector2f& position, float duration, const sf::Vector2f& velocity = {0.f, 0.f});
    void update(float dt, const Map& map);
    void draw(sf::RenderWindow& window) const;

    bool active() const;
    sf::Vector2f position() const;

private:
    sf::CircleShape shape_;
    float timer_;
    bool isActive_;

    sf::Vector2f velocity_;
    bool isMoving_;
};

#endif