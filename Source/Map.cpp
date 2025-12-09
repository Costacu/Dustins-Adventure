#include "../header/Map.h"
#include <iostream>


Map::Map(sf::Vector2u windowSize)
    : windowSize_(windowSize), margin_(0.f)
{
    background_.setSize(sf::Vector2f(windowSize_.x, windowSize_.y));
    background_.setFillColor(sf::Color(70, 50, 30));

    rebuildPlayArea();

    grid_ = {
        "WWWWWWWWWWWWWWWWWWWWWWWWWW",
        "W.P......W...............W",
        "W.WWWW.W.W.WWWWWW.WWWWWW.W",
        "W.W....W.W......W......W.W",
        "W.W.WWWW.W.WWWW.W.WWW..W.W",
        "W.W......W.W....W...W..W.W",
        "W.WWWWWWWW.W.WWWW.W.WWWW.W",
        "W..........W......W......W",
        "W.WWWWWWWWWWWWWWWWWWWWWW.W",
        "W.W......................W",
        "W.W.WWWWWWWWWWWWWWWWWWW..W",
        "W.W.W..............W.....W",
        "W.W.W.WWWWWWWWWW.W.W.WWW.W",
        "W.W.W.W..........W.W....DW",
        "W.W.W.W.WWWWWWWWWW.W.WWWW.",
        "W.W.....W...........W....W",
        "W.WWWW.W.WWWWWWWWWWWW.WW.W",
        "W......W..............W..W",
        "W.E....WWWWWWWWWWWWWWWW..W",
        "WWWWWWWWWWWWWWWWWWWWWWWWWW"
    };

    buildMap();
}

Map::~Map() = default;

void Map::buildMap() {
    walls_.clear();

    for (size_t r = 0; r < grid_.size(); r++) {
        for (size_t c = 0; c < grid_[r].size(); c++) {
            char tile = grid_[r][c];
            float x = c * TILE;
            float y = r * TILE;

            if (tile == 'W') {
                sf::RectangleShape w({TILE, TILE});
                w.setPosition(x, y);
                w.setFillColor(sf::Color(60, 60, 60));
                walls_.push_back(w);
            }
            else if (tile == 'P') {
                playerSpawn_ = {x + TILE/4, y + TILE/4};
            }
            else if (tile == 'E') {
                enemySpawn_ = {x + TILE/4, y + TILE/4};
            }
            else if (tile == 'D') {
                doorShape_.setSize({TILE, TILE});
                doorShape_.setFillColor(sf::Color::Red);
                doorShape_.setPosition(x, y);
                doorBounds_ = doorShape_.getGlobalBounds();
            }
        }
    }
}

bool Map::collidesWithWall(const sf::FloatRect& box) const {
    return std::any_of(walls_.begin(), walls_.end(),
        [&](const sf::RectangleShape& w) {
            return box.intersects(w.getGlobalBounds());
        });
}


bool Map::reachedDoor(const sf::FloatRect& bounds) const {
    return bounds.intersects(doorBounds_);
}

sf::Vector2f Map::getPlayerSpawn() const { return playerSpawn_; }
sf::Vector2f Map::getEnemySpawn() const { return enemySpawn_; }

void Map::draw(sf::RenderWindow& window) const {
    window.draw(background_);
    for (const auto& w : walls_) window.draw(w);
    window.draw(doorShape_);
}

void Map::rebuildPlayArea() {
    playArea_ = sf::FloatRect(
        margin_,
        margin_,
        windowSize_.x - margin_ * 2.f,
        windowSize_.y - margin_ * 2.f
    );
}
