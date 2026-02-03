#pragma once
#ifndef DECOY_H
#define DECOY_H

#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "Map.h"

class Decoy : public Entity {
public:
    Decoy();
    ~Decoy() override;

    Entity* clone() const override;
    void print(std::ostream& os) const override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) const override;
    sf::FloatRect getBounds() const override;

    void spawn(const sf::Vector2f& pos, float duration, const sf::Vector2f& velocity = {0.f, 0.f});
    bool active() const;
    void setMap(const Map* map);

private:
    sf::Vector2f velocity_;
    float lifetime_;
    bool active_;
    const Map* mapRef_ = nullptr;

    static sf::Texture texture_;
    static bool textureLoaded_;
};

#endif