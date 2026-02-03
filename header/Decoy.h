#pragma once
#ifndef DECOY_H
#define DECOY_H

#include <SFML/Graphics.hpp>
#include "Map.h"

class Decoy {
public:
    Decoy();
    ~Decoy();

    void spawn(const sf::Vector2f& pos, float duration, const sf::Vector2f& velocity = {0.f, 0.f});
    void update(float dt, const Map& map);
    void draw(sf::RenderWindow& window) const;

    bool active() const;
    sf::Vector2f position() const;

private:
    sf::CircleShape shape_;
    sf::Vector2f velocity_;
    float lifetime_;
    bool active_;

    static sf::Texture texture_;
    static bool textureLoaded_;
};

#endif