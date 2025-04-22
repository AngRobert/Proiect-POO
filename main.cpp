#include "src/Game.h"

int main() {
    // Init Game
    Game game;
    while (game.isRunning()) {
        //Update
        //Render
        game.pollEvents();
        if (game.isGameOver()) {
            game.renderGameOver();
        }
        else if (game.minigameTime()) {
            game.loadMinigame();
        }
        else {
            game.update();
            game.render();
        }
    }
    return 0;
}
