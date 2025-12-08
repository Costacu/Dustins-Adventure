#pragma once
#ifndef DECOY_H
#define DECOY_H
#include <SFML/Graphics.hpp>
#include "Entity.h"

class Decoy : public Entity {
public:
    Decoy();

    void spawn(const sf::Vector2f& playerCenter, float lifetime);

    void update(float dt) override;
    void print(std::ostream& os) const override;
    Entity* clone() const override;

    bool active() const;
    const sf::Vector2f& position() const;

    void draw(sf::RenderWindow& window) const override;

private:
    float lifetime_;
    float radius_;
    sf::CircleShape shape_;
};


#endif // DECOY_H
