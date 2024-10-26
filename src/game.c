// game.c
#include "game.h"
#include "utils.h"
#include <stdio.h>
#include "level.h"

int initGame(Game* game) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    game->window = SDL_CreateWindow("2D Platformer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (game->window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);
    if (game->renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    game->isRunning = 1;
    initPlayer(&game->player);
    initLevelManager(&game->levelManager);

    setPlayerStartPos(&game->player, playerStartPosition);

    return 0;
}

void gameLoop(Game* game) {
    SDL_Event e;
    const Uint8* currentKeyStates;

    while (game->isRunning) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                game->isRunning = 0;
            }

            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_n) {
                int nextLevel = (game->levelManager.currentLevel + 1) % game->levelManager.totalLevels;
                loadLevel(&game->levelManager, nextLevel);
                game->player.x = 100;
                game->player.y = 100;
                printf("Switched to level %d\n", nextLevel + 1);  // Debug print
            }
        }

        currentKeyStates = SDL_GetKeyboardState(NULL);
        handlePlayerInput(&game->player, currentKeyStates);
        updatePlayer(&game->player);
        checkCollisionCurrentLevel(&game->player, &game->levelManager);

        SDL_SetRenderDrawColor(game->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(game->renderer);

        renderCurrentLevel(game->renderer, &game->levelManager);
        renderPlayer(game->renderer, &game->player);

        SDL_RenderPresent(game->renderer);

        SDL_Delay(16);
    }
}

void cleanupGame(Game* game) {
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    SDL_Quit();
}
