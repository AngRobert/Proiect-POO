#include "src/Game.h"
/* NOTHING MAKES SENSE FOR NOW
 *
 *  CREATE POLL-EVENTS FOR MENU SCREEN SO I CAN REGISTER CLICKS (I THINK)
 *  STOP MINIGAME FROM APPEARING ON MENU SCREEN
 */
int main() {
    // Init Game
    Game game;
    while (game.isRunning()) {
        //Update
        //Render
        game.pollEvents();

        game.update();

        game.render();
        // if (game.isGameOver()) {
        //     game.renderGameOver();
        // }
        // else if (game.minigameTime()) {
        //     game.loadMinigame();
        // }
        // else {
        //     game.update();
        //     game.render();
        // }
    }
    return 0;
}
