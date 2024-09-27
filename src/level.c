#include "level.h"
#include "utils.h"

void initLevelManager(LevelManager* levelManager) {
    levelManager->currentLevel = 0;
    levelManager->totalLevels = 2; // Start with 2 levels

    // Level 1
    Level* level1 = &levelManager->levels[0];
    level1->platformCount = 3;
    level1->platforms[0] = (SDL_Rect){0, SCREEN_HEIGHT - 20, SCREEN_WIDTH, 20};
    level1->platforms[1] = (SDL_Rect){100, 400, 200, 20};
    level1->platforms[2] = (SDL_Rect){500, 300, 200, 20};

    // Level 2
    Level* level2 = &levelManager->levels[1];
    level2->platformCount = 4;
    level2->platforms[0] = (SDL_Rect){0, SCREEN_HEIGHT - 20, SCREEN_WIDTH, 20};
    level2->platforms[1] = (SDL_Rect){100, 450, 150, 20};
    level2->platforms[2] = (SDL_Rect){400, 350, 150, 20};
    level2->platforms[3] = (SDL_Rect){700, 250, 150, 20};
}

void loadLevel(LevelManager* levelManager, int levelIndex) {
    if (levelIndex >= 0 && levelIndex < levelManager->totalLevels) {
        levelManager->currentLevel = levelIndex;
    }
}

void renderCurrentLevel(SDL_Renderer* renderer, LevelManager* levelManager) {
    Level* currentLevel = &levelManager->levels[levelManager->currentLevel];
    SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
    for (int i = 0; i < currentLevel->platformCount; i++) {
        SDL_RenderFillRect(renderer, &currentLevel->platforms[i]);
    }
}

void checkCollisionCurrentLevel(Player* player, LevelManager* levelManager) {
    Level* currentLevel = &levelManager->levels[levelManager->currentLevel];
    for (int i = 0; i < currentLevel->platformCount; i++) {
        SDL_Rect playerRect = {(int)player->x, (int)player->y, player->width, player->height};
        SDL_Rect* platform = &currentLevel->platforms[i];
        
        if (SDL_HasIntersection(&playerRect, platform)) {
            // Collision from above
            if (player->vy > 0 && player->y + player->height <= platform->y + player->vy) {
                player->y = platform->y - player->height;
                player->vy = 0;
                player->isJumping = 0;
            }
            // Collision from below
            else if (player->vy < 0 && player->y >= platform->y + platform->h + player->vy) {
                player->y = platform->y + platform->h;
                player->vy = 0;
            }
            // Collision from left
            else if (player->vx > 0 && player->x + player->width <= platform->x + player->vx) {
                player->x = platform->x - player->width;
            }
            // Collision from right
            else if (player->vx < 0 && player->x >= platform->x + platform->w + player->vx) {
                player->x = platform->x + platform->w;
            }
        }
    }
}
