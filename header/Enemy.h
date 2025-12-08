#pragma once
#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include <string>
#include "Entity.h"

class Enemy : public Entity {
public:
    explicit Enemy(std::string name = "Demogorgon", int hp = 2, float speed = 100.f, std::string texturePath = "textures/Demogorgon.png");

    Enemy(const Enemy& other);
    Enemy& operator=(const Enemy& other);
    ~Enemy() override;

    Entity* clone() const override;
    void print(std::ostream& os) const override;
    void update(float dt) override;

    void chase(float dt, const sf::Vector2f& playerPos, const sf::FloatRect& playArea);
    void distractTo(const sf::Vector2f& pos, float seconds);
    void reset();

    const std::string& getName() const;
    const std::string& getTexturePath() const;
    float getSpeed() const;

private:
    void loadTexture();

    std::string name_;
    std::string texturePath_;
    int hp_;
    int maxHp_;
    float speed_;

    bool distracted_ = false;
    float distractTimer_ = 0.f;
    sf::Vector2f distractPos_;
};

#endif // ENEMY_H
