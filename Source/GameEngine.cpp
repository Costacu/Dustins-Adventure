#include "../header/GameEngine.h"

#include "../header/GameEngine.h"
#include <iostream>

// GameEngine.cpp
GameEngine::GameEngine(unsigned int width, unsigned int height, const std::string& title)
    : window_(sf::VideoMode(width, height), title),
      isRunning_(true),
      gameOver_(false),
      playerWon_(false),
      player_("Dustin", 3, 220.f, "textures/Dustin.png"),
      enemy_("Demogorgon", 2, 120.f, "textures/Demogorgon.png"),
      map_({width, height})
{
    player_.setPosition(map_.getPlayerSpawn().x, map_.getPlayerSpawn().y);
    enemy_.setPosition(map_.getEnemySpawn().x, map_.getEnemySpawn().y);
}

GameEngine::~GameEngine() {}

void GameEngine::run() {
    while (isRunning_ && window_.isOpen()) {
        processEvents();
        float dt = clock_.restart().asSeconds();
        update(dt);
        render();
    }
}

void GameEngine::processEvents() {
    sf::Event e;
    while (window_.pollEvent(e)) {
        if (e.type == sf::Event::Closed) {
            window_.close();
            isRunning_ = false;
        }
        if (e.type == sf::Event::KeyPressed) {
            if (e.key.code == sf::Keyboard::Escape) {
                window_.close();
                isRunning_ = false;
            }
            if (e.key.code == sf::Keyboard::R) {
                reset();
            }
        }
    }
}

void GameEngine::update(float dt) {
    if (gameOver_) return;
    player_.update(dt);
    enemy_.update(dt, sf::Vector2f(player_.getPosition().x, player_.getPosition().y), map_.getPlayArea());

    sf::FloatRect pB = player_.getBounds();
    sf::FloatRect eB = enemy_.getBounds();

    sf::Vector2f pSize(pB.width, pB.height);
    sf::Vector2f eSize(eB.width, eB.height);

    sf::Vector2f pClamped = map_.clampPosition(sf::Vector2f(player_.getPosition().x, player_.getPosition().y), pSize);
    sf::Vector2f eClamped = map_.clampPosition(sf::Vector2f(enemy_.getPosition().x, enemy_.getPosition().y), eSize);

    player_.setPosition(pClamped.x, pClamped.y);
    enemy_.setPosition(eClamped.x, eClamped.y);

    checkCollisions();
    checkWinCondition();
}

void GameEngine::render() {
    window_.clear();
    map_.draw(window_);

    sf::FloatRect pB = player_.getBounds();
    sf::RectangleShape pShape;
    pShape.setSize(sf::Vector2f(pB.width, pB.height));
    pShape.setPosition(pB.left, pB.top);
    pShape.setFillColor(sf::Color(200, 200, 255));
    window_.draw(pShape);

    sf::FloatRect eB = enemy_.getBounds();
    sf::RectangleShape eShape;
    eShape.setSize(sf::Vector2f(eB.width, eB.height));
    eShape.setPosition(eB.left, eB.top);
    eShape.setFillColor(sf::Color(255, 120, 120));
    window_.draw(eShape);

    window_.display();
}

void GameEngine::reset() {
    gameOver_ = false;
    playerWon_ = false;
    player_.reset();
    enemy_.reset();
    player_.setPosition(map_.getPlayerSpawn().x, map_.getPlayerSpawn().y);
    enemy_.setPosition(map_.getEnemySpawn().x, map_.getEnemySpawn().y);
}

void GameEngine::checkCollisions() {
    sf::FloatRect pB = player_.getBounds();
    sf::FloatRect eB = enemy_.getBounds();
    if (pB.intersects(eB)) {
        gameOver_ = true;
        playerWon_ = false;
    }
}

void GameEngine::checkWinCondition() {
    if (map_.reachedDoor(player_.getBounds())) {
        gameOver_ = true;
        playerWon_ = true;
    }
}
