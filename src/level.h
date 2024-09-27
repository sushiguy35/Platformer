#ifndef LEVEL_H
#define LEVEL_H

#include <SDL2/SDL.h>
#include "player.h"

#define MAX_PLATFORMS 20
#define MAX_LEVELS 5

typedef struct {
    SDL_Rect platforms[MAX_PLATFORMS];
    int platformCount;
} Level;

typedef struct {
    Level levels[MAX_LEVELS];
    int currentLevel;
    int totalLevels;
} LevelManager;

void initLevelManager(LevelManager* levelManager);
void loadLevel(LevelManager* levelManager, int levelIndex);
void renderCurrentLevel(SDL_Renderer* renderer, LevelManager* levelManager);
void checkCollisionCurrentLevel(Player* player, LevelManager* levelManager);

#endif // LEVEL_H
