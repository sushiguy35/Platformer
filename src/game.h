#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "player.h"
#include "level.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

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
