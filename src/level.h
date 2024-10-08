// level.h
#ifndef LEVEL_H
#define LEVEL_H

#include <SDL2/SDL.h>
#include "player.h"
#include <stdbool.h>

#define MAX_LEVELS 1
#define MAX_PLATFORMS 100

typedef struct {
    SDL_Rect rect;
    SDL_Color color;
} Platform;

typedef struct {
    Platform platforms[MAX_PLATFORMS];
    int platformCount;
} Level;

typedef struct {
    Level levels[MAX_LEVELS];
    int currentLevel;
    int totalLevels;
} LevelManager;

void initLevelManager(LevelManager* levelManager);
void loadLevelsFromFile(LevelManager* levelManager, const char* filename);
void loadLevel(LevelManager* levelManager, int levelIndex);
void renderCurrentLevel(SDL_Renderer* renderer, LevelManager* levelManager);
void checkCollisionCurrentLevel(Player* player, LevelManager* levelManager);

#endif // LEVEL_H
