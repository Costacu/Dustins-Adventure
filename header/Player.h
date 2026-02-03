#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "Map.h"
#include "GameTemplates.h"

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
    void setMap(const Map* map);

    void draw(sf::RenderWindow& window) const override;

    bool useDecoy();
    int getDecoyCount() const;
    void addDecoys(int count);

    void setHidden(bool hidden);
    bool isHidden() const;

    void setExitingCloset(bool exiting);
    bool isExitingCloset() const;

    bool hasShovel() const;
    void collectShovel();

private:
    void loadTexture();

    std::string name_;
    std::string texturePath_;
    const Map* mapRef_ = nullptr;

    GameProperty<int> hp_;
    GameProperty<float> speed_;

    int decoyCount_ = 5;
    bool isHidden_ = false;
    bool isExitingCloset_ = false;
    bool hasShovel_ = false;
};

#endif