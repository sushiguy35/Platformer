#include <SDL2/SDL.h>
#include "game.h"

int main(int argc, char* argv[]) {
    printf("Starting main function\n");
    Game game;
    printf("About to call initGame\n");
    if (initGame(&game) == 0) {
        printf("initGame successful, about to enter gameLoop\n");
        gameLoop(&game);
    } else {
        printf("initGame failed\n");
    }
    printf("About to call cleanupGame\n");
    cleanupGame(&game);
    printf("Exiting main function\n");
    return 0;
}

