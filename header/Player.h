#pragma once
#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <SFML/Graphics.hpp>
#include <iosfwd>
#include "Entity.h"

class Player: public Entity {
public:
    explicit Player(std::string name = "Dustin", int hp = 2, float speed = 1.0f, std::string texturePath_ = "textures/Dustin1.png");
    Player(const Player& other);
    Player& operator=(const Player& other);
    ~Player() override;

    Entity* clone() const override;

    void print(std::ostream& os) const override;

    void update(float dt) override;
    void reset();
    void setPosition(float newX, float newY);
    const Position& getPosition() const;
    void draw(sf::RenderWindow& window) const override;
    const std::string& getName() const;
    const std::string& getTexturePath() const;
    int getHp() const;
    int getMaxHp() const;
    float getSpeed() const;
    void move(float dx, float dy);

private:
    void loadTexture();
    std::string name_;
    std::string texturePath_;
    int hp_;
    int maxHp_;
    float speed_;
};

std::ostream& operator<<(std::ostream& os, const Player::Position& p);
std::ostream& operator<<(std::ostream& os, const Player& pl);

#endif // PLAYER_H
