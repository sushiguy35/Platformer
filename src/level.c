// level.c
#include "level.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

void initLevelManager(LevelManager* levelManager) {
    levelManager->currentLevel = 0;
    levelManager->totalLevels = 0;
    for (int i = 0; i < MAX_LEVELS; i++) {
        levelManager->levels[i].platformCount = 0;  // Initialize platformCount for each level
    }
    loadLevelsFromFile(levelManager, "levels.txt");
}

void loadLevelsFromFile(LevelManager* levelManager, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file: %s\n", filename);
        return;
    }

    char line[256];
    int currentLevel = -1;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;  // Remove newline
        printf("Read line: %s\n", line);  // Debug print

        if (strstr(line, "MAX_LEVELS")) {
            sscanf(line, "MAX_LEVELS = %d;", &levelManager->totalLevels);
            printf("Set total levels to: %d\n", levelManager->totalLevels);  // Debug print
        } 
        else if (strstr(line, "Level")) {
            sscanf(line, "Level %d:", &currentLevel);
            currentLevel--;  // Adjust for 0-based index
            if (currentLevel < 0 || currentLevel >= MAX_LEVELS) {
                printf("Error: Invalid level number %d\n", currentLevel + 1);
                continue;
            }
            printf("Processing level: %d\n", currentLevel + 1);  // Debug print
        } 
        else if (!strchr(line, ':') && strlen(line) > 0) {
            if (currentLevel < 0 || currentLevel >= MAX_LEVELS) {
                printf("Error: Trying to add platform to invalid level %d\n", currentLevel + 1);
                continue;
            }

            Platform platform;
            float r, g, b, a; // Change to float if reading 0.0 to 1.0
            sscanf(line, "%d %d %d %d %f %f %f %f", 
                &platform.rect.x, &platform.rect.y, 
                &platform.rect.w, &platform.rect.h, 
                &r, &g, &b, &a);

            platform.color.r = (Uint8)(fmax(0, fmin(255, r * 255.0f)));
            platform.color.g = (Uint8)(fmax(0, fmin(255, g * 255.0f)));
            platform.color.b = (Uint8)(fmax(0, fmin(255, b * 255.0f)));
            platform.color.a = (Uint8)(fmax(0, fmin(255, a * 255.0f)));


            // Debug print color values to confirm they're parsed correctly
            printf("Parsed platform color: r=%d, g=%d, b=%d, a=%d\n", 
                   platform.color.r, platform.color.g, 
                   platform.color.b, platform.color.a);

            Level* level = &levelManager->levels[currentLevel];
            if (level->platformCount >= MAX_PLATFORMS) {
                printf("Error: Too many platforms in level %d\n", currentLevel + 1);
                continue;
            }
            level->platforms[level->platformCount++] = platform;
            printf("Added platform to level %d: (%d, %d, %d, %d)\n", 
                   currentLevel + 1, platform.rect.x, platform.rect.y, 
                   platform.rect.w, platform.rect.h);  // Debug print
        }
    }

    fclose(file);
    printf("Loaded %d levels\n", levelManager->totalLevels);

    // Print summary of loaded levels
    for (int i = 0; i < levelManager->totalLevels; i++) {
        printf("Level %d has %d platforms\n", i + 1, levelManager->levels[i].platformCount);
    }
}

void loadLevel(LevelManager* levelManager, int levelIndex) {
    if (levelIndex >= 0 && levelIndex < levelManager->totalLevels) {
        levelManager->currentLevel = levelIndex;
        printf("Loaded level %d\n", levelIndex + 1);  // Debug print
    } else {
        printf("Error: Attempted to load invalid level %d\n", levelIndex + 1);  // Debug print
    }
}

void renderCurrentLevel(SDL_Renderer* renderer, LevelManager* levelManager) {
    if (levelManager->currentLevel < 0 || levelManager->currentLevel >= levelManager->totalLevels) {
        printf("Error: Trying to render invalid level %d\n", levelManager->currentLevel + 1);
        return;
    }
    Level* currentLevel = &levelManager->levels[levelManager->currentLevel];
    if (currentLevel->platformCount == 0) {
        printf("Warning: Attempting to render empty level %d\n", levelManager->currentLevel + 1);
        return;
    }

    for (int i = 0; i < currentLevel->platformCount; i++) {
        Platform* platform = &currentLevel->platforms[i];
        SDL_SetRenderDrawColor(renderer, 
                               platform->color.r, 
                               platform->color.g, 
                               platform->color.b, 
                               platform->color.a);
        SDL_RenderFillRect(renderer, &platform->rect);
    }
}



void checkCollisionCurrentLevel(Player* player, LevelManager* levelManager) {
    if (levelManager->currentLevel < 0 || levelManager->currentLevel >= levelManager->totalLevels) {
        printf("Error: Trying to check collisions for invalid level %d\n", levelManager->currentLevel + 1);
        return;
    }
    
    Level* currentLevel = &levelManager->levels[levelManager->currentLevel];
    if (currentLevel->platformCount == 0) {
        printf("Warning: Attempting to check collisions for empty level %d\n", levelManager->currentLevel + 1);
        return;
    }

    float nextX = player->x + player->vx;
    float nextY = player->y + player->vy;
    SDL_Rect nextPlayerRect = {(int)nextX, (int)nextY, player->width, player->height};
    SDL_Rect currentPlayerRect = {(int)player->x, (int)player->y, player->width, player->height};

    for (int i = 0; i < currentLevel->platformCount; i++) {
        SDL_Rect* platform = &currentLevel->platforms[i].rect;
        
        if (SDL_HasIntersection(&nextPlayerRect, platform)) {
            // Collision detected, determine the direction
            float overlapLeft = nextX + player->width - platform->x;
            float overlapRight = platform->x + platform->w - nextX;
            float overlapTop = nextY + player->height - platform->y;
            float overlapBottom = platform->y + platform->h - nextY;

            // Find the smallest overlap
            float minOverlapX = (overlapLeft < overlapRight) ? overlapLeft : overlapRight;
            float minOverlapY = (overlapTop < overlapBottom) ? overlapTop : overlapBottom;

            // Resolve collision based on the smallest overlap
            if (minOverlapX < minOverlapY) {
                if (overlapLeft < overlapRight) {
                    player->x = platform->x - player->width;
                } else {
                    player->x = platform->x + platform->w;
                }
                player->vx = 0;
            } else {
                if (overlapTop < overlapBottom) {
                    player->y = platform->y - player->height;
                    player->vy = 0;
                    player->isJumping = 0;
                } else {
                    player->y = platform->y + platform->h;
                    player->vy = 0;
                }
            }

            // Update the current player rectangle after collision resolution
            currentPlayerRect.x = (int)player->x;
            currentPlayerRect.y = (int)player->y;
        }
    }

    // Apply gravity if not on a platform
    bool onPlatform = false;
    SDL_Rect feetRect = {(int)player->x, (int)(player->y + player->height), player->width, 1};
    for (int i = 0; i < currentLevel->platformCount; i++) {
        if (SDL_HasIntersection(&feetRect, &currentLevel->platforms[i].rect)) {
            onPlatform = true;
            player->isJumping = true;
            break;
        }
    }

    if (!onPlatform) {
        player->vy += GRAVITY;
    } else {
        player->isJumping = 0;
    }

    // Ensure the player stays within the screen bounds
    if (player->x < 0) player->x = 0;
    if (player->x > SCREEN_WIDTH - player->width) player->x = SCREEN_WIDTH - player->width;
    if (player->y < 0) player->y = 0;
    if (player->y > SCREEN_HEIGHT - player->height) {
        player->y = SCREEN_HEIGHT - player->height;
        player->vy = 0;
        player->isJumping = 0;
    }
}
