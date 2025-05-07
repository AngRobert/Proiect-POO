#include "src/Game.h"

int main() {
    // Init Game
    Game game;
    while (game.isRunning()) {
        game.pollEvents();

        game.update();

        game.render();
    }
    return 0;
}
