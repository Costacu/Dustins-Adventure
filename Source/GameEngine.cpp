#include "../header/GameEngine.h"
#include "../header/Exceptions.h"
#include "../header/EntityFactory.h"
#include "../header/GameTemplates.h"
#include <iostream>
#include <cmath>
#include <algorithm>

GameEngine::GameEngine(unsigned int width, unsigned int height, const std::string& title)
    : window_(sf::VideoMode::getDesktopMode(), title, sf::Style::Default),
      isRunning_(true),
      gameOver_(false),
      playerWon_(false),
      player_(EntityFactory::createPlayer()),
      map_({width, height})
{
    window_.setPosition(sf::Vector2i(0, 0));

    std::cout << "Entities: " << Entity::getEntityCount() << "\n";

    player_.setMap(&map_);
    player_.setPosition(map_.getPlayerSpawn().x, map_.getPlayerSpawn().y);

    const auto& spawns = map_.getEnemySpawns();
    for (const auto& pos : spawns) {
        auto enemy = std::make_unique<Enemy>(EntityFactory::createEnemy());
        enemy->setPosition(pos.x, pos.y);
        entities_.push_back(std::move(enemy));
    }

    if (!decoyTexture_.loadFromFile("textures/Budinca.png")) {
        decoyTexture_.loadFromFile("../textures/Budinca.png");
    }
    if (!shovelTextureUI_.loadFromFile("textures/Lopata.png")) {
        shovelTextureUI_.loadFromFile("../textures/Lopata.png");
    }

    updateView();
    setupUI();
}

GameEngine::~GameEngine() = default;

void GameEngine::run() {
    while (isRunning_ && window_.isOpen()) {
        processEvents();
        float dt = clock_.restart().asSeconds();
        update(dt);
        render();
    }
}

void GameEngine::processEvents() {
    sf::Event e{};
    while (window_.pollEvent(e)) {
        if (e.type == sf::Event::MouseButtonPressed) {
            if (e.mouseButton.button == sf::Mouse::Left && !gameOver_) {
                throwStaticDecoy();
            }
            if (e.mouseButton.button == sf::Mouse::Right && !gameOver_) {
                throwProjectileDecoy();
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
                if (gameOver_) reset();
            }
            if (e.key.code == sf::Keyboard::E) {
                tryInteract();
            }
        }
        if (e.type == sf::Event::Resized) {
            updateView();
            setupUI();
        }
    }
}

void GameEngine::updateView() {
    sf::Vector2f mapSize = map_.getMapSize();
    gameView_.setSize(mapSize);
    gameView_.setCenter(mapSize.x / 2.f, mapSize.y / 2.f);

    sf::Vector2u windowSize = window_.getSize();
    float windowRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
    float mapRatio = mapSize.x / mapSize.y;

    if (windowRatio > mapRatio) {
        float scale = mapRatio / windowRatio;
        float offset = (1.f - scale) / 2.f;
        gameView_.setViewport(sf::FloatRect(offset, 0.f, scale, 1.f));
    } else {
        float scale = windowRatio / mapRatio;
        float offset = (1.f - scale) / 2.f;
        gameView_.setViewport(sf::FloatRect(0.f, offset, 1.f, scale));
    }
    window_.setView(gameView_);
}

void GameEngine::update(float dt) {
    if (gameOver_) return;

    player_.update(dt);

    for (auto& entityPtr : entities_) {
        entityPtr->update(dt);

        if (auto* enemy = dynamic_cast<Enemy*>(entityPtr.get())) {
            auto oldPos = enemy->getPosition();

            enemy->updateAI(dt,
                            sf::Vector2f(player_.getPosition().getX(), player_.getPosition().getY()),
                            map_,
                            player_.isHidden());

            if (map_.collidesWithWall(enemy->getBounds())) {
                enemy->setPosition(oldPos.getX(), oldPos.getY());
            }
        }
    }

    entities_.erase(std::remove_if(entities_.begin(), entities_.end(),
        [](const std::unique_ptr<Entity>& e) {
            if (auto* decoy = dynamic_cast<Decoy*>(e.get())) {
                return !decoy->active();
            }
            return false;
        }), entities_.end());

    for (auto& entEnemy : entities_) {
        if (auto* enemy = dynamic_cast<Enemy*>(entEnemy.get())) {
            float bestDistSq = std::numeric_limits<float>::max();
            sf::Vector2f bestPos = {0.f, 0.f};
            bool foundDecoy = false;
            float detectionRangeSq = 450.f * 450.f;

            for (const auto& entDecoy : entities_) {
                if (auto* decoy = dynamic_cast<Decoy*>(entDecoy.get())) {
                    if (!decoy->active()) continue;

                    sf::Vector2f dp(decoy->getPosition().getX(), decoy->getPosition().getY());
                    sf::Vector2f ep(enemy->getPosition().getX(), enemy->getPosition().getY());

                    float distSq = math::getDistSq(dp, ep);
                    if (distSq < detectionRangeSq && distSq < bestDistSq) {
                        bestDistSq = distSq;
                        bestPos = dp;
                        foundDecoy = true;
                    }
                }
            }

            if (foundDecoy) {
                enemy->distractTo(bestPos, 0.2f);
            }
        }
    }

    checkCollisions();
    checkWinCondition();
}

void GameEngine::render() {
    window_.clear();

    window_.setView(gameView_);
    map_.draw(window_);

    for (const auto& e : entities_) {
        e->draw(window_);
    }

    player_.draw(window_);

    window_.setView(window_.getDefaultView());

    int count = player_.getDecoyCount();
    for (int i = 0; i < count && i < static_cast<int>(decoyUI_.size()); ++i) {
        window_.draw(decoyUI_[i]);
    }

    if (player_.hasShovel()) {
        window_.draw(shovelUI_);
    }

    if (gameOver_) {
        window_.draw(overlay_);
        window_.draw(uiText_);
    }

    window_.display();
}

void GameEngine::reset() {
    gameOver_ = false;
    playerWon_ = false;

    map_.resetToFirstLevel();
    player_.reset();

    entities_.clear();
    player_.setPosition(map_.getPlayerSpawn().x, map_.getPlayerSpawn().y);

    const auto& spawns = map_.getEnemySpawns();
    for (const auto& pos : spawns) {
        auto e = std::make_unique<Enemy>(EntityFactory::createEnemy());
        e->setPosition(pos.x, pos.y);
        entities_.push_back(std::move(e));
    }

    updateView();
}

void GameEngine::checkCollisions() {
    if (player_.isHidden()) return;

    sf::FloatRect pB = player_.getBounds();

    for (const auto& e : entities_) {
        if (auto* enemy = dynamic_cast<Enemy*>(e.get())) {
            sf::FloatRect eB = enemy->getBounds();
            if (pB.intersects(eB)) {
                gameOver_ = true;
                playerWon_ = false;
                updateOverlayText("Ai fost mancat de Demogorgon!", "Apasa tasta R pentru a incerca din nou");
                return;
            }
        }
    }
}

void GameEngine::setupUI() {
    sf::Vector2u winSize = window_.getSize();
    overlay_.setSize(sf::Vector2f(static_cast<float>(winSize.x), static_cast<float>(winSize.y)));
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
    uiText_.setPosition(static_cast<float>(winSize.x) / 2.f, static_cast<float>(winSize.y) / 2.f);

    decoyUI_.clear();
    float radius = 60.f;
    float gap = 10.f;
    float startX = winSize.x - 50.f;
    float startY = 50.f;

    for (int i = 0; i < 5; ++i) {
        sf::CircleShape c(radius);
        c.setTexture(&decoyTexture_);
        c.setOutlineColor(sf::Color::Black);
        c.setOutlineThickness(2.f);
        c.setOrigin(radius, radius);
        c.setPosition(startX - i * (radius * 2 + gap), startY);
        decoyUI_.push_back(c);
    }

    shovelUI_.setSize({140.f, 140.f});
    shovelUI_.setTexture(&shovelTextureUI_);
    shovelUI_.setOutlineColor(sf::Color::Black);
    shovelUI_.setOutlineThickness(2.f);
    shovelUI_.setOrigin(70.f, 70.f);
    shovelUI_.setPosition(startX, startY + 200.f);
}

void GameEngine::updateOverlayText(const std::string& titleLine, const std::string& hintLine) {
    std::string msg = titleLine + "\n" + hintLine;
    uiText_.setString(msg);
    sf::FloatRect bounds = uiText_.getLocalBounds();
    uiText_.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    sf::Vector2u winSize = window_.getSize();
    uiText_.setPosition(static_cast<float>(winSize.x) / 2.f, static_cast<float>(winSize.y) / 2.f);
}

void GameEngine::throwStaticDecoy() {
    if (gameOver_ || player_.isHidden()) return;

    if (player_.useDecoy()) {
        sf::FloatRect bounds = player_.getBounds();
        sf::Vector2f center(bounds.left + bounds.width / 2.f,
                            bounds.top + bounds.height / 2.f);

        auto d = std::make_unique<Decoy>();
        d->setMap(&map_);
        d->spawn(center, decoyLifetime_);

        entities_.push_back(std::move(d));
    }
}

void GameEngine::throwProjectileDecoy() {
    if (gameOver_ || player_.isHidden()) return;

    if (player_.useDecoy()) {
        sf::FloatRect bounds = player_.getBounds();
        sf::Vector2f startPos(bounds.left + bounds.width / 2.f,
                              bounds.top + bounds.height / 2.f);

        sf::Vector2i mousePixel = sf::Mouse::getPosition(window_);
        sf::Vector2f mouseWorld = window_.mapPixelToCoords(mousePixel, gameView_);

        sf::Vector2f dir = mouseWorld - startPos;
        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (len != 0) dir /= len;

        float speed = 400.f;

        auto d = std::make_unique<Decoy>();
        d->setMap(&map_);
        d->spawn(startPos, decoyLifetime_, dir * speed);

        entities_.push_back(std::move(d));
    }
}

void GameEngine::tryInteract() {
    if (gameOver_) return;

    sf::FloatRect pBounds = player_.getBounds();
    sf::Vector2f pCenter(pBounds.left + pBounds.width/2.f, pBounds.top + pBounds.height/2.f);

    if (player_.isHidden()) {
        player_.setHidden(false);
        player_.setExitingCloset(true);
        return;
    }

    const auto& closets = map_.getClosets();
    for (size_t i = 0; i < closets.size(); ++i) {
        sf::FloatRect cBounds = closets[i].getGlobalBounds();
        sf::Vector2f cCenter(cBounds.left + cBounds.width/2.f, cBounds.top + cBounds.height/2.f);

        float distSq = math::getDistSq(pCenter, cCenter);

        if (distSq < 90.f * 90.f) {
            float playerW = player_.getBounds().width;
            float playerH = player_.getBounds().height;
            player_.setPosition(cCenter.x - playerW / 2.f, cCenter.y - playerH / 2.f);

            player_.setHidden(true);

            if (player_.getDecoyCount() < 5 && !map_.isClosetVisited(i)) {
                int add = (std::rand() % 2) + 1;
                player_.addDecoys(add);
                map_.markClosetVisited(i);
            }
            return;
        }
    }

    if (map_.getCurrentLevel() == 2) {
        if (!map_.isShovelTaken()) {
             sf::FloatRect sBounds = map_.getShovelBounds();
             sf::Vector2f sCenter(sBounds.left + sBounds.width/2.f, sBounds.top + sBounds.height/2.f);
             if (math::getDistSq(pCenter, sCenter) < 80.f * 80.f) {
                 player_.collectShovel();
                 map_.takeShovel();
                 return;
             }
        }

        if (player_.hasShovel()) {
             sf::FloatRect pRect = player_.getBounds();
             pRect.left -= 40.f; pRect.top -= 40.f;
             pRect.width += 80.f; pRect.height += 80.f;

             int idx = map_.getIntersectingRubbleIndex(pRect);
             if (idx != -1) {
                 map_.clearRubble(idx);
                 return;
             }
        }

        if (!map_.isGeneratorOn()) {
             sf::FloatRect gBounds = map_.getGeneratorBounds();
             sf::Vector2f gCenter(gBounds.left + gBounds.width/2.f, gBounds.top + gBounds.height/2.f);
             if (math::getDistSq(pCenter, gCenter) < 100.f * 100.f) {
                 map_.turnOnGenerator();
                 return;
             }
        }
    }

    if (map_.getCurrentLevel() == 2 || map_.getCurrentLevel() == 3) {
        if (map_.isGeneratorOn()) {
            sf::FloatRect pRect = player_.getBounds();
            int btnId = map_.getIntersectingButtonIndex(pRect);
            if (btnId != -1) {
                map_.toggleButton(btnId);
                return;
            }
        }
    }
}

void GameEngine::checkWinCondition() {
    if (map_.reachedTransitionDoor(player_.getBounds())) {
        int current = map_.getCurrentLevel();
        int next = current;
        sf::Vector2f spawnPos;

        sf::Vector2f pCenter = {
             player_.getBounds().left + player_.getBounds().width/2,
             player_.getBounds().top + player_.getBounds().height/2
        };
        sf::Vector2f mapSize = map_.getMapSize();

        if (current == 0) {
            if (pCenter.y < 200.f) {
                next = 3;
                spawnPos = {mapSize.x / 2.f, mapSize.y - 200.f};
            } else {
                next = 1;
                spawnPos = {70.f, mapSize.y / 2.f};
            }
        }
        else if (current == 1) {
            if (pCenter.y < 200.f) {
                next = 2;
                spawnPos = {mapSize.x / 2.f, mapSize.y - 200.f};
            }
            else {
                next = 0;
                spawnPos = {mapSize.x - 150.f, mapSize.y / 2.f};
            }
        }
        else if (current == 2) {
            next = 1;
            spawnPos = {mapSize.x / 2.f, 90.f};
        }
        else if (current == 3) {
            next = 0;
            spawnPos = {mapSize.x / 2.f, 75.f};
        }

        map_.loadLevel(next);
        player_.setPosition(spawnPos.x, spawnPos.y);

        entities_.clear();
        const auto& spawns = map_.getEnemySpawns();
        for (const auto& pos : spawns) {
            auto e = std::make_unique<Enemy>(EntityFactory::createEnemy());
            e->setPosition(pos.x, pos.y);
            entities_.push_back(std::move(e));
        }

        updateView();
    }

    if (map_.reachedWinDoor(player_.getBounds())) {
        gameOver_ = true;
        playerWon_ = true;
        updateOverlayText("Ai iesit din Laboratorul din Hawkins!", "Apasa tasta R pentru a juca din nou sau Esc pentru a iesi din joc");
    }
}