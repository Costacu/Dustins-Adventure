//
// Created by stefa on 11/2/2025.
//
#pragma once
#ifndef MAP_H
#define MAP_H
#include <SFML/Graphics.hpp>


class Map {
public:
    explicit Map(sf::Vector2u windowSize = {800, 600});
    ~Map();

    void setWindowSize(sf::Vector2u size);
    sf::Vector2u getWindowSize() const;

    void setMargins(float marginPixels);
    float getMargins() const;

    const sf::FloatRect& getPlayArea() const;

    void setDoor(const sf::Vector2f& pos, const sf::Vector2f& size);
    const sf::FloatRect getDoorBounds() const;
    bool reachedDoor(const sf::FloatRect& bounds) const;

    void setPlayerSpawn(const sf::Vector2f& pos);
    void setEnemySpawn(const sf::Vector2f& pos);
    sf::Vector2f getPlayerSpawn() const;
    sf::Vector2f getEnemySpawn() const;

    sf::Vector2f clampPosition(const sf::Vector2f& pos, const sf::Vector2f& size) const;


    void draw(sf::RenderWindow& window) const;

private:
    void rebuildPlayArea();

    sf::Vector2u windowSize_;
    float margin_;
    sf::FloatRect playArea_;
    sf::RectangleShape doorShape_;
    sf::Vector2f playerSpawn_;
    sf::Vector2f enemySpawn_;
    sf::RectangleShape background_;
};


#endif //MAP_H
