#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <string>
#include "Entity.h"
#include "Map.h"
#include "GameTemplates.h"

class Player : public Entity {
public:
    explicit Player(std::string name = "Dustin", int hp = 2, float speed = 150.f, std::string texturePath = "textures/Dustin.png");

    Player(const Player& other);
    Player& operator=(const Player& other);
    ~Player() override;

    [[maybe_unused]] Entity* clone() const override;
    void print(std::ostream& os) const override;

    void update(float dt) override;
    void setMap(Map* m);
    sf::FloatRect getBounds() const override;

    void addDecoys(int count);
    bool useDecoy();
    int getDecoyCount() const;

    void collectShovel();
    bool hasShovel() const;

    void setHidden(bool h);
    bool isHidden() const;

    void setExitingCloset(bool e);

    void reset();

private:
    void loadTexture();

    std::string name_;
    std::string texturePath_;

    GameProperty<int> hp_;
    GameProperty<float> speed_;

    int decoyCount_;
    bool hasShovel_;
    bool isHidden_;
    bool isExitingCloset_;

    Map* map_ = nullptr;
};

#endif