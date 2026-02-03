#ifndef FRIENDLYENEMY_H
#define FRIENDLYENEMY_H

#include <SFML/Graphics.hpp>
#include <string>
#include "Entity.h"
#include "Map.h"

class FriendlyEnemy : public Entity {
public:
    explicit FriendlyEnemy(std::string name = "Demogorgon", int hp = 2, float speed = 120.f, std::string texturePath = "textures/Demogorgon.png");

    FriendlyEnemy(const FriendlyEnemy& other);
    FriendlyEnemy& operator=(const FriendlyEnemy& other);
    ~FriendlyEnemy() override;

    Entity* clone() const override;
    void print(std::ostream& os) const override;

    void update(float dt) override;
    void updateAI(float dt, const sf::Vector2f& playerPos, const Map& map);

    void reset();

private:
    std::string name_;
    int hp_;
    int maxHp_;
    float speed_;
    std::string texturePath_;

    enum class State { Patrol, Chase };
    State state_ = State::Patrol;

    sf::Vector2f direction_;

    float distractTimer_ = 0.f;
    sf::Vector2f distractPos_;

    void patrol(float dt, const Map& map);

    void loadTexture();
};

#endif //FRIENDLYENEMY_H
