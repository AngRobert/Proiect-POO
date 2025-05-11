#include <iostream>

#include "src/Game.h"
#include "src/GameExceptions.h"

int main() {
    // Init Game
    std::unique_ptr<Game> game;
    try {
        game = std::make_unique<Game>();
    } catch (const ResourceLoadException& e) {
        std::cout << e.what() << std::endl;
    }
    while (game->isRunning()) {
        game->pollEvents();

        game->update();

        game->render();
    }
    return 0;
}
