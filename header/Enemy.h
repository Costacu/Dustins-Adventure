#pragma once
#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include <string>
#include "Entity.h"
#include "Map.h"

class Enemy : public Entity {
public:
    explicit Enemy(std::string name = "Demogorgon",
                   int hp = 2,
                   float speed = 120.f,
                   std::string texturePath = "textures/Demogorgon.png");

    Enemy(const Enemy& other);
    Enemy& operator=(const Enemy& other);
    ~Enemy() override;

    Entity* clone() const override;
    void print(std::ostream& os) const override;

    void update(float dt) override;
    void updateAI(float dt, const sf::Vector2f& playerPos, const Map& map);

    void distractTo(const sf::Vector2f& pos, float seconds);
    void reset();

private:
    std::string name_;
    int hp_;
    int maxHp_;
    float speed_;
    std::string texturePath_;

    enum class State { Patrol, Chase };
    State state_ = State::Patrol;

    sf::Vector2f direction_;

    bool distracted_ = false;
    float distractTimer_ = 0.f;
    sf::Vector2f distractPos_;

    bool detectPlayer(const sf::Vector2f& playerPos) const;
    void patrol(float dt, const Map& map);
    void chase(float dt, const sf::Vector2f& playerPos, const Map& map);

    void loadTexture();
};

#endif
