#pragma once
#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Map {
public:
    explicit Map(sf::Vector2u windowSize = {800, 600});
    ~Map();

    void draw(sf::RenderWindow& window) const;

    const sf::FloatRect& getPlayArea() const;
    bool reachedDoor(const sf::FloatRect& bounds) const;

    sf::Vector2f getPlayerSpawn() const;
    sf::Vector2f getEnemySpawn() const;

    bool collidesWithWall(const sf::FloatRect& box) const;

private:
    void rebuildPlayArea();
    void buildMap();

    sf::Vector2u windowSize_;
    float margin_;
    sf::FloatRect playArea_;

    std::vector<std::string> grid_;
    std::vector<sf::RectangleShape> walls_;

    sf::RectangleShape doorShape_;
    sf::FloatRect doorBounds_;

    sf::Vector2f playerSpawn_;
    sf::Vector2f enemySpawn_;

    sf::RectangleShape background_;

    static constexpr float TILE = 70.f;
};

#endif
