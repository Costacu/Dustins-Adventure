#include <iostream>
#include <array>
#include <chrono>
#include <thread>

#include <SFML/Graphics.hpp>

#include "../header/GameEngine.h"

int main() {
    GameEngine game(1280, 720, "Dustin's Adventure");
    game.run();
    return 0;
}
