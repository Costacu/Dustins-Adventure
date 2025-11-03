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


    const sf::FloatRect& getPlayArea() const;

    bool reachedDoor(const sf::FloatRect& bounds) const;

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
