#include <SDL2/SDL.h>
#include "game.h"

int main(int argc, char* argv[]) {
    Game game;
    if (initGame(&game) == 0) {
        gameLoop(&game);
    }
    cleanupGame(&game);
    return 0;
}
