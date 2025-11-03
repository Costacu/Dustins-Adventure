#include "../header/GameEngine.h"

#include "../header/GameEngine.h"
#include <iostream>
#include <cmath>

GameEngine::GameEngine(unsigned int width, unsigned int height, const std::string& title)
    : window_(sf::VideoMode(width, height), title),
      isRunning_(true),
      gameOver_(false),
      playerWon_(false),
      player_("Dustin", 3, 220.f, "Dustin.png"),
      enemy_("Ghoul", 2, 120.f, "Demogorgon.png"),
      map_({width, height}) {
    player_.setPosition(map_.getPlayerSpawn().x, map_.getPlayerSpawn().y);
    enemy_.setPosition(map_.getEnemySpawn().x, map_.getEnemySpawn().y);
    setupUI();
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
        if (e.type == sf::Event::KeyPressed) {
            if (e.key.code == sf::Keyboard::Space && !gameOver_) {
                throwDecoy();
            }
        }

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
                if (gameOver_) {
                    reset();
                }
            }
        }
        if (e.type == sf::Event::Resized) {
            sf::View view = window_.getView();
            view.setSize(static_cast<float>(e.size.width), static_cast<float>(e.size.height));
            view.setCenter(static_cast<float>(e.size.width) * 0.5f, static_cast<float>(e.size.height) * 0.5f);
            window_.setView(view);
            overlay_.setSize(sf::Vector2f(static_cast<float>(e.size.width), static_cast<float>(e.size.height)));
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

    for (auto& d : decoys_) d.update(dt, map_.getPlayArea());

    decoys_.erase(std::remove_if(decoys_.begin(), decoys_.end(),
        [](const Decoy& d){ return !d.active(); }), decoys_.end());

    if (!decoys_.empty()) {
        float bestDist2 = std::numeric_limits<float>::max();
        sf::Vector2f bestPos = decoys_.front().position();
        for (auto& d : decoys_) {
            if (!d.active()) continue;
            sf::Vector2f dp = d.position() - sf::Vector2f(enemy_.getPosition().x, enemy_.getPosition().y);
            float dist2 = dp.x * dp.x + dp.y * dp.y;
            if (dist2 < bestDist2) { bestDist2 = dist2; bestPos = d.position(); }
        }
        enemy_.distractTo(bestPos, 0.2f);
    }

    checkCollisions();
    checkWinCondition();
}


void GameEngine::render() {
    window_.clear();
    map_.draw(window_);
    for (auto& d : decoys_) d.draw(window_);
    player_.draw(window_);
    enemy_.draw(window_);
    if (gameOver_) {
        window_.draw(overlay_);
        window_.draw(uiText_);
        window_.draw(uiText_);
    }
    window_.display();
}


void GameEngine::reset() {
    gameOver_ = false;
    playerWon_ = false;
    player_.reset();
    enemy_.reset();
    decoys_.clear();
    player_.setPosition(map_.getPlayerSpawn().x, map_.getPlayerSpawn().y);
    enemy_.setPosition(map_.getEnemySpawn().x, map_.getEnemySpawn().y);
}

void GameEngine::checkCollisions() {
    sf::FloatRect pB = player_.getBounds();
    sf::FloatRect eB = enemy_.getBounds();
    if (pB.intersects(eB)) {
        gameOver_ = true;
        playerWon_ = false;
        updateOverlayText("You Died!", "Press R to retry");
    }
}

void GameEngine::checkWinCondition() {
    if (map_.reachedDoor(player_.getBounds())) {
        gameOver_ = true;
        playerWon_ = true;
        updateOverlayText("You Win!", "Press R to play again");
    }
}

void GameEngine::setupUI() {
    overlay_.setSize(sf::Vector2f(static_cast<float>(window_.getSize().x), static_cast<float>(window_.getSize().y)));
    overlay_.setFillColor(sf::Color(0, 0, 0, 150));

    if (!uiFont_.loadFromFile("fonts/MomoTrustDisplay-Regular.ttf")) {
        uiFont_.loadFromFile("../fonts/MomoTrustDisplay-Regular.ttf");
    }

    uiText_.setFont(uiFont_);
    uiText_.setCharacterSize(36);
    uiText_.setFillColor(sf::Color::White);
    uiText_.setOutlineColor(sf::Color::Black);
    uiText_.setOutlineThickness(2.f);
    uiText_.setString("Press R to retry");

    sf::FloatRect bounds = uiText_.getLocalBounds();
    uiText_.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    uiText_.setPosition(static_cast<float>(window_.getSize().x) / 2.f, static_cast<float>(window_.getSize().y) / 2.f);

}

void GameEngine::updateOverlayText(const std::string& titleLine, const std::string& hintLine) {
    std::string msg = titleLine + "\n" + hintLine;
    uiText_.setString(msg);

    sf::FloatRect bounds = uiText_.getLocalBounds();

    uiText_.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);

    uiText_.setPosition(static_cast<float>(window_.getSize().x) / 2.f, static_cast<float>(window_.getSize().y) / 2.f);
}



void GameEngine::throwDecoy() {
    sf::Vector2f start(player_.getPosition().x + player_.getBounds().width * 0.5f,
                       player_.getPosition().y + player_.getBounds().height * 0.5f);

    sf::Vector2i mousePixels = sf::Mouse::getPosition(window_);
    sf::Vector2f mouseWorld = window_.mapPixelToCoords(mousePixels);

    sf::Vector2f dir = mouseWorld - start;
    float len = std::sqrt(dir.x*dir.x + dir.y*dir.y);
    if (len > 0.f) { dir.x /= len; dir.y /= len; } else { dir = {1.f, 0.f}; }

    Decoy d;
    d.spawn(start, dir, decoyThrowSpeed_, decoyLifetime_);
    decoys_.push_back(d);
}
