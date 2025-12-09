#include "../header/GameEngine.h"
#include "../header/Exceptions.h"
#include <iostream>


int main() {
    try {
        GameEngine game(1920, 1380, "Dustin's Adventure");
        game.run();
    }
    catch (const EngineInitError& e) {
        std::cerr << e.what() << "\n";
    }
    catch (const GameException& e) {
        std::cerr << "Game error: " << e.what() << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Standard error: " << e.what() << "\n";
    }

    return 0;
}