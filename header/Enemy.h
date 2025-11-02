//
// Created by stefa on 11/2/2025.
//
#pragma once
#ifndef ENEMY_H
#define ENEMY_H
#include <SFML/Graphics.hpp>
#include <string>

class Enemy {
public:
    struct Position {
        float x = 0.f;
        float y = 0.f;
    };

    explicit Enemy(std::string name = "Ghoul", int hp = 2, float speed = 100.f, std::string texturePath = "../textures/Demogorgon.png");
    Enemy(const Enemy& other);
    Enemy& operator=(const Enemy& other);
    ~Enemy();

    void update(float dt, const sf::Vector2f& playerPos, const sf::FloatRect& playArea);
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
    void draw(sf::RenderWindow& window) const;

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


#endif //ENEMY_H
