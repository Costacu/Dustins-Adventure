#ifndef ENTITYPOINTER_H
#define ENTITYPOINTER_H

#pragma once
#include <memory>
#include "Entity.h"
#include "Player.h"
#include "Enemy.h"
#include "Decoy.h"

class EntityHolder {
private:
    Entity* ptr_;

public:
    //constructor
    explicit EntityHolder(Entity* e = nullptr)
        : ptr_(e) {}

    //destructor
    ~EntityHolder() { delete ptr_; }

    //copy constructor
    EntityHolder(const EntityHolder& other)
        : ptr_(other.ptr_ ? other.ptr_->clone() : nullptr) {}

    //swap
    EntityHolder& operator=(EntityHolder other) {
        std::swap(ptr_, other.ptr_);
        return *this;
    }

    void update(float dt) {
        if (ptr_) ptr_->update(dt);
    }

    void draw(sf::RenderWindow& win) const {
        if (ptr_) ptr_->draw(win);
    }

    Player* asPlayer() { return dynamic_cast<Player*>(ptr_); }
    Enemy* asEnemy()   { return dynamic_cast<Enemy*>(ptr_); }
    Decoy* asDecoy()   { return dynamic_cast<Decoy*>(ptr_); }
};


#endif //ENTITYPOINTER_H
