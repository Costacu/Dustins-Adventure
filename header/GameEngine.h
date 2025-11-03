#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Player.h"
#include "Enemy.h"
#include "Map.h"
#include "Decoy.h"

class GameEngine {
public:
    explicit GameEngine(unsigned int width = 1280, unsigned int height = 720, const std::string& title = "Dustin's Adventure");
    ~GameEngine();

    void run();

private:
    void processEvents();
    void update(float dt);
    void render();
    void reset();

    void checkCollisions();
    void checkWinCondition();

    void setupUI();
    void updateOverlayText(const std::string& titleLine, const std::string& hintLine);

    sf::RenderWindow window_;
    sf::Clock clock_;
    bool isRunning_;
    bool gameOver_;
    bool playerWon_;

    Player player_;
    Enemy enemy_;
    Map map_;

    sf::Font uiFont_;
    sf::Text uiText_;
    sf::RectangleShape overlay_;


    std::vector<Decoy> decoys_;
    float decoyLifetime_ = 3.0f;

    void throwDecoy();
};
