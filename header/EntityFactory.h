#pragma once
#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H

#include "Player.h"
#include "Enemy.h"
#include <memory>

class EntityFactory {
public:
    static Player createPlayer() {
        return Player("Dustin", 3, 220.f, "Dustin.png");
    }

    static Enemy createEnemy() {
        return Enemy("Demogorgon", 2, 120.f, "Demogorgon.png");
    }
};

#endif