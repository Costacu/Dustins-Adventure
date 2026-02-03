#pragma once
#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Map.h"
#include "Enemy.h"
#include "Decoy.h"
#include <vector>
#include <memory>

class GameEngine {
public:
    GameEngine(unsigned int width, unsigned int height, const std::string& title);
    ~GameEngine();

    void run();

private:
    void processEvents();
    void update(float dt);
    void render();

    void reset();
    void checkCollisions();
    void checkWinCondition();

    void throwStaticDecoy();
    void throwProjectileDecoy();

    void tryInteract();

    void setupUI();
    void updateOverlayText(const std::string& titleLine, const std::string& hintLine);

    void updateView();

    sf::RenderWindow window_;
    sf::View gameView_;

    bool isRunning_;
    bool gameOver_;
    bool playerWon_;

    Player player_;

    std::vector<std::unique_ptr<Entity>> entities_;

    Map map_;

    sf::Font uiFont_;
    sf::Text uiText_;
    sf::RectangleShape overlay_;

    std::vector<sf::CircleShape> decoyUI_;
    sf::RectangleShape shovelUI_;

    sf::Texture decoyTexture_;
    sf::Texture shovelTextureUI_;

    sf::Clock clock_;
    float decoyLifetime_ = 9.0f;
};

#endif