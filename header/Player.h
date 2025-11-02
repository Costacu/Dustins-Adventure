#ifndef PLAYER_H
#define PLAYER_H
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>


#pragma once
#include <string>
#include <iosfwd>
#include <SFML/Graphics.hpp>

class Player {
public:
    struct Position {
        float x = 0.f;
        float y = 0.f;
    };

    explicit Player(std::string name = "Dustin", int hp = 2, float speed = 1.0f, std::string texturePath = "../textures/Dustin.png");
    Player(const Player& other);
    Player& operator=(const Player& other);
    ~Player();

    void input();
    void update(float dt);
    void takeDamage(int dmg);
    void reset();

    bool isAlive() const;
    const std::string& getName() const;
    const std::string& getTexturePath() const;
    int getHp() const;
    int getMaxHp() const;
    float getSpeed() const;
    const Position& getPosition() const;
    sf::FloatRect getBounds() const;

    void setPosition(float newX, float newY);
    void move(float dx, float dy);

private:
    void loadTexture();
    void clampHp();

    std::string name_;
    std::string texturePath_;
    int hp_;
    int maxHp_;
    float speed_;
    Position pos_;
    sf::Texture texture_;
    sf::Sprite sprite_;
};

std::ostream& operator<<(std::ostream& os, const Player::Position& p);
std::ostream& operator<<(std::ostream& os, const Player& pl);




#endif //PLAYER_H
