#include "../header/Map.h"


Map::Map(sf::Vector2u windowSize)
    : windowSize_(windowSize), margin_(0.f) {
    rebuildPlayArea();
    background_.setSize(sf::Vector2f(windowSize_.x, windowSize_.y));
    background_.setFillColor(sf::Color(101, 67, 33));
    doorShape_.setSize(sf::Vector2f(60.f, 120.f));
    doorShape_.setFillColor(sf::Color::Red);
    doorShape_.setPosition(windowSize_.x - 80.f, windowSize_.y / 2.f - 60.f);
    playerSpawn_ = sf::Vector2f(80.f, windowSize_.y / 2.f);
    enemySpawn_ = sf::Vector2f(200.f, windowSize_.y / 2.f);
}

Map::~Map() {}

void Map::setWindowSize(sf::Vector2u size) {
    windowSize_ = size;
    rebuildPlayArea();
    background_.setSize(sf::Vector2f(windowSize_.x, windowSize_.y));
}

sf::Vector2u Map::getWindowSize() const {
    return windowSize_;
}

void Map::setMargins(float marginPixels) {
    margin_ = marginPixels;
    rebuildPlayArea();
}

float Map::getMargins() const {
    return margin_;
}

const sf::FloatRect& Map::getPlayArea() const {
    return playArea_;
}

void Map::setDoor(const sf::Vector2f& pos, const sf::Vector2f& size) {
    doorShape_.setPosition(pos);
    doorShape_.setSize(size);
}

const sf::FloatRect Map::getDoorBounds() const {
    return doorShape_.getGlobalBounds();
}

bool Map::reachedDoor(const sf::FloatRect& bounds) const {
    return doorShape_.getGlobalBounds().intersects(bounds);
}

void Map::setPlayerSpawn(const sf::Vector2f& pos) {
    playerSpawn_ = pos;
}

void Map::setEnemySpawn(const sf::Vector2f& pos) {
    enemySpawn_ = pos;
}

sf::Vector2f Map::getPlayerSpawn() const {
    return playerSpawn_;
}

sf::Vector2f Map::getEnemySpawn() const {
    return enemySpawn_;
}

sf::Vector2f Map::clampPosition(const sf::Vector2f& pos, const sf::Vector2f& size) const {
    sf::Vector2f p = pos;
    if (p.x < playArea_.left) p.x = playArea_.left;
    if (p.y < playArea_.top) p.y = playArea_.top;
    if (p.x + size.x > playArea_.left + playArea_.width) p.x = playArea_.left + playArea_.width - size.x;
    if (p.y + size.y > playArea_.top + playArea_.height) p.y = playArea_.top + playArea_.height - size.y;
    return p;
}

void Map::draw(sf::RenderWindow& window) const {
    window.draw(background_);
    window.draw(doorShape_);
}

void Map::rebuildPlayArea() {
    playArea_ = sf::FloatRect(margin_, margin_, windowSize_.x - 2.f * margin_, windowSize_.y - 2.f * margin_);
}
