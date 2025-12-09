#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <SFML/Graphics.hpp>
#include "Entity.h"

class Player : public Entity {
public:
    explicit Player(std::string name = "Dustin", int hp = 2, float speed = 1.0f, std::string texturePath = "textures/Dustin.png");

    Player(const Player& other);
    Player& operator=(const Player& other);
    ~Player() override;

    [[maybe_unused]] Entity* clone() const override;
    void print(std::ostream& os) const override;

    void update(float dt) override;
    void reset();

    void draw(sf::RenderWindow& window) const override;

private:
    void loadTexture();

    std::string name_;
    std::string texturePath_;
    int hp_;
    int maxHp_;
    float speed_;
};

#endif // PLAYER_H
