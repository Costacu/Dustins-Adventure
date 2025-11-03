#pragma once
#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <SFML/Graphics.hpp>
#include <iosfwd>

class Player {
public:
    struct Position {
    private:
        float x = 0.f;
        float y = 0.f;

    public:
        Position() = default;
        Position(float x, float y) : x(x), y(y) {}

        float getX() const { return x; }
        float getY() const { return y; }
        void setX(float newX) { x = newX; }
        void setY(float newY) { y = newY; }
        void set(float newX, float newY) { x = newX; y = newY; }
    };

    explicit Player(std::string name = "Dustin", int hp = 2, float speed = 1.0f, std::string texturePath_ = "textures/Dustin.png");
    Player(const Player& other);
    Player& operator=(const Player& other);
    ~Player();

    void update(float dt);
    void reset();

    void draw(sf::RenderWindow& window) const;

    const std::string& getName() const;
    const std::string& getTexturePath() const;
    int getHp() const;
    int getMaxHp() const;
    float getSpeed() const;
    const Position& getPosition() const;
    sf::FloatRect getBounds() const;

    void setPosition(float newX, float newY);
    void move(float dx, float dy);

private:
    void loadTexture();

    std::string name_;
    std::string texturePath_;
    int hp_;
    int maxHp_;
    float speed_;
    Position pos_;
    sf::Texture texture_;
    sf::Sprite sprite_;
};

std::ostream& operator<<(std::ostream& os, const Player::Position& p);
std::ostream& operator<<(std::ostream& os, const Player& pl);

#endif // PLAYER_H
