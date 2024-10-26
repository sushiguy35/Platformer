#ifndef LEVEL_H
#define LEVEL_H

#include <SDL2/SDL.h>
#include "player.h"
#include <stdbool.h>
#include "utils.h"

#define MAX_LEVELS 1
#define MAX_PLATFORMS 100

extern vec2 playerStartPosition;

typedef struct {
    SDL_Rect rect;
    SDL_Color color;
    unsigned short int id;
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

void changeLevel(LevelManager* levelManager, Player* player, int direction);
void initLevelManager(LevelManager* levelManager);
void loadLevelsFromFile(LevelManager* levelManager, const char* filename);
void loadLevel(LevelManager* levelManager, int levelIndex);
void renderCurrentLevel(SDL_Renderer* renderer, LevelManager* levelManager);
void checkCollisionCurrentLevel(Player* player, LevelManager* levelManager);

#endif // LEVEL_H
