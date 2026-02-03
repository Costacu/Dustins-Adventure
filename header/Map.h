#pragma once
#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>

struct GameButton {
    sf::RectangleShape shape;
    int id;
    bool active;
};

class Map {
public:
    explicit Map(sf::Vector2u windowSize = {800, 600});
    ~Map();

    void draw(sf::RenderWindow& window) const;

    bool reachedWinDoor(const sf::FloatRect& bounds) const;
    bool reachedTransitionDoor(const sf::FloatRect& bounds) const;

    sf::Vector2f getPlayerSpawn() const;
    sf::Vector2f getEnemySpawn() const;
    sf::Vector2f getTransitionSpawn() const;

    sf::Vector2f getMapSize() const;

    bool collidesWithWall(const sf::FloatRect& box, bool ignoreClosets = false) const;

    const std::vector<sf::RectangleShape>& getClosets() const;
    bool isClosetVisited(size_t index) const;
    void markClosetVisited(size_t index);

    std::vector<sf::Vector2f> findPath(const sf::Vector2f& start, const sf::Vector2f& target) const;

    void loadLevel(int index);
    void resetToFirstLevel();
    int getCurrentLevel() const;

    bool isShovelTaken() const;
    void takeShovel();

    bool isRubbleCleared(int index) const;
    void clearRubble(int index);
    int getIntersectingRubbleIndex(const sf::FloatRect& bounds) const;

    bool isGeneratorOn() const;
    void turnOnGenerator();

    void toggleButton(int id);
    bool checkButtonCondition() const;
    int getIntersectingButtonIndex(const sf::FloatRect& bounds) const;

    sf::FloatRect getShovelBounds() const;
    sf::FloatRect getGeneratorBounds() const;
    const std::vector<sf::RectangleShape>& getRubbleShapes() const;

private:
    void buildMap();
    void checkDoorSound();

    sf::Vector2u windowSize_;
    std::vector<std::string> grid_;

    std::vector<sf::RectangleShape> walls_;
    sf::Texture wallTexture_;

    std::vector<sf::RectangleShape> floorTiles_;
    sf::Texture floorTexture_;

    std::vector<sf::RectangleShape> closets_;
    sf::Texture closetTexture_;
    std::vector<bool> closetVisited_;

    sf::RectangleShape winDoorShape_;
    sf::Texture exitDoorTexture_;
    sf::FloatRect winDoorBounds_;

    std::vector<sf::RectangleShape> transitionShapes_;

    sf::Texture portalTexture_;
    sf::Texture doorTexture_;

    sf::Vector2f playerSpawn_;
    sf::Vector2f enemySpawn_;

    sf::RectangleShape background_;

    std::vector<std::vector<std::string>> levels_;
    int currentLevelIndex_ = 0;

    sf::RectangleShape shovelShape_;
    sf::Texture shovelTexture_;
    bool shovelTaken_ = false;

    std::vector<sf::RectangleShape> rubbleShapes_;
    sf::Texture rubbleTexture_;
    std::vector<bool> rubbleActive_;

    sf::RectangleShape generatorShape_;
    sf::Texture generatorTexture_;
    bool generatorOn_ = false;

    std::vector<GameButton> currentLevelButtons_;
    std::vector<bool> globalButtonStates_;

    sf::RectangleShape dustOverlayShape_;
    sf::Texture dustOverlayTexture_;

    sf::SoundBuffer generatorBuffer_;
    sf::Sound generatorSound_;

    sf::SoundBuffer doorBuzzerBuffer_;
    sf::Sound doorBuzzerSound_;
    bool doorSoundPlayed_ = false;

    static constexpr float TILE = 70.f;
};

#endif