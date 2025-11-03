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

    explicit Enemy(std::string name = "Demogorgon", int hp = 2, float speed = 100.f, std::string texturePath_ = "textures/Demogorgon.png");
    Enemy(const Enemy& other);
    Enemy& operator=(const Enemy& other);
    ~Enemy();

    void update(float dt, const sf::Vector2f& playerPos, const sf::FloatRect& playArea);void reset();

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


    void distractTo(const sf::Vector2f& pos, float seconds);

private:
    void loadTexture();

    std::string name_;
    std::string texturePath_;
    int hp_;
    int maxHp_;
    float speed_;
    Position pos_;
    sf::Texture texture_;
    sf::Sprite sprite_;


    bool distracted_ = false;
    float distractTimer_ = 0.f;
    sf::Vector2f distractPos_;
};


#endif //ENEMY_H
