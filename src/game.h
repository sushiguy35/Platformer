#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "player.h"
#include "level.h"

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    int isRunning;
    Player player;
    LevelManager levelManager;
} Game;

int initGame(Game* game);
void gameLoop(Game* game);
void cleanupGame(Game* game);

#endif // GAME_H
