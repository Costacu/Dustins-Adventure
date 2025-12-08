#ifndef ENTITY_H
#define ENTITY_H
#pragma once

#include <SFML/Graphics.hpp>

class Entity {
public:
    class Position {
    private:
        float x = 0.f;
        float y = 0.f;
    public:
        Position() = default;
        Position(float x, float y) : x(x), y(y) {}

        [[nodiscard]] float getX() const { return x; }
        [[nodiscard]] float getY() const { return y; }
        void setX(float newX) { x = newX; }
        void setY(float newY) { y = newY; }
        void set(float newX, float newY) { x = newX; y = newY; }
        void translate(float dx, float dy) { x += dx; y += dy; }
    };

    virtual void update(float dt) = 0;
    [[maybe_unused]] virtual Entity* clone() const = 0;
    Entity(float x = 0.f, float y = 0.f);

    virtual void print(std::ostream& os) const = 0;
    friend std::ostream& operator<<(std::ostream& os, const Entity& e) {
        e.print(os);
        return os;
    }

    virtual sf::FloatRect getBounds() const;
    const Position& getPosition() const { return pos_; }

    virtual void setPosition(float newX, float newY);
    void move(float dx, float dy);
    virtual void draw(sf::RenderWindow& window) const;
    virtual ~Entity() = default;

    [[maybe_unused]] static int getEntityCount();

protected:
    Position pos_;
    sf::Texture texture_;
    sf::Sprite sprite_;
    static int entityCount;
};

#endif //ENTITY_H
