#include "level.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

vec2 playerStartPosition = {0, 0};  // Initialize with default values

void changeLevel(LevelManager* levelManager, Player* player, int direction) {
    int newLevel = levelManager->currentLevel + direction;
    
    if (newLevel >= 0 && newLevel < levelManager->totalLevels) {
        levelManager->currentLevel = newLevel;
        player->x = playerStartPosition.x;
        player->y = playerStartPosition.y;
        printf("Changed to level %d\n", newLevel + 1);
    } else {
        printf("Cannot change to level %d. Staying on current level.\n", newLevel + 1);
    }
}



void initLevelManager(LevelManager* levelManager) {
    levelManager->currentLevel = 0;
    levelManager->totalLevels = 0;
    for (int i = 0; i < MAX_LEVELS; i++) {
        levelManager->levels[i].platformCount = 0;
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
        line[strcspn(line, "\n")] = 0;
        printf("Read line: %s\n", line);

        if (strstr(line, "MAX_LEVELS")) {
            sscanf(line, "MAX_LEVELS = %d;", &levelManager->totalLevels);
            printf("Set total levels to: %d\n", levelManager->totalLevels);
        } else if (strstr(line, "Level")) {
            sscanf(line, "Level %d:", &currentLevel);
            currentLevel--;
            if (currentLevel < 0 || currentLevel >= MAX_LEVELS) {
                printf("Error: Invalid level number %d\n", currentLevel + 1);
                continue;
            }
            printf("Processing level: %d\n", currentLevel + 1);
        } else if (strstr(line, "PLAYER_START_X")) {
            sscanf(line, "PLAYER_START_X = %d:", &playerStartPosition.x);
        } else if (strstr(line, "PLAYER_START_Y")) {
            sscanf(line, "PLAYER_START_Y = %d:", &playerStartPosition.y);
        } else if (!strchr(line, ':') && strlen(line) > 0) {
            if (currentLevel < 0 || currentLevel >= MAX_LEVELS) {
                printf("Error: Trying to add platform to invalid level %d\n", currentLevel + 1);
                continue;
            }

            Platform platform;
            float r, g, b, a;
            sscanf(line, "%d %d %d %d %f %f %f %f %d",
                   &platform.rect.x, &platform.rect.y,
                   &platform.rect.w, &platform.rect.h,
                   &r, &g, &b, &a, &platform.id);
            platform.color.r = (Uint8)(fmax(0, fmin(255, r * 255.0f)));
            platform.color.g = (Uint8)(fmax(0, fmin(255, g * 255.0f)));
            platform.color.b = (Uint8)(fmax(0, fmin(255, b * 255.0f)));
            platform.color.a = (Uint8)(fmax(0, fmin(255, a * 255.0f)));
            
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
                   platform.rect.w, platform.rect.h);
        }
    }
    fclose(file);
    printf("Loaded %d levels\n", levelManager->totalLevels);
    for (int i = 0; i < levelManager->totalLevels; i++) {
        printf("Level %d has %d platforms\n", i + 1, levelManager->levels[i].platformCount);
    }
}

void loadLevel(LevelManager* levelManager, int levelIndex) {
    if (levelIndex >= 0 && levelIndex < levelManager->totalLevels) {
        levelManager->currentLevel = levelIndex;
        printf("Loaded level %d\n", levelIndex + 1);
    } else {
        printf("Error: Attempted to load invalid level %d\n", levelIndex + 1);
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

    bool onPlatform = false;

    for (int i = 0; i < currentLevel->platformCount; i++) {
        SDL_Rect* platform = &currentLevel->platforms[i].rect;

        if (SDL_HasIntersection(&nextPlayerRect, platform)) {
            float overlapLeft = nextX + player->width - platform->x;
            float overlapRight = platform->x + platform->w - nextX;
            float overlapTop = nextY + player->height - platform->y;
            float overlapBottom = platform->y + platform->h - nextY;

            float minOverlapX = (overlapLeft < overlapRight) ? overlapLeft : overlapRight;
            float minOverlapY = (overlapTop < overlapBottom) ? overlapTop : overlapBottom;

            if (minOverlapX < minOverlapY) {
                // Horizontal collision
                if (overlapLeft < overlapRight) {
                    player->x = platform->x - player->width; // Move player to the left
                } else {
                    player->x = platform->x + platform->w; // Move player to the right
                }
                player->vx = 0; // Stop horizontal movement
            } else {
                // Vertical collision
                if (overlapTop < overlapBottom) {
                    player->y = platform->y - player->height; // Move player above the platform
                    player->vy = 0; // Stop downward movement
                    player->isJumping = 0; // Player landed on platform
                    onPlatform = true;

                    // Allow jumping only if the platform id != 1 (not a spike)
                    if (currentLevel->platforms[i].id != 1) {
                        player->canJump = 1;
                    } else {
                        player->canJump = 0; // Prevent jumping on spike platforms
                    }
                } else {
                    // If the player is too low, adjust their position
                    if (nextY + player->height > platform->y + platform->h - 1) {
                        player->y = platform->y + platform->h; // Move player below the platform
                    } else {
                        player->y = platform->y - player->height + 0.1f; // Small offset to prevent snapping
                    }
                    player->vy = 0; // Stop upward movement
                }
            }
        }
    }

    // Check if the player's feet are touching any platform
    SDL_Rect feetRect = {(int)player->x, (int)(player->y + player->height), player->width, 1};
    for (int i = 0; i < currentLevel->platformCount; i++) {
        if (SDL_HasIntersection(&feetRect, &currentLevel->platforms[i].rect)) {
            onPlatform = true; // Player is touching a platform
            break;
        }
    }

    // Apply gravity if not on a platform
    if (!onPlatform) {
        player->vy += GRAVITY;
    } else {
        player->isJumping = 0; // Reset jumping state when on a platform
    }

    // Prevent the player from going out of bounds
    if (player->y < 0) player->y = 0;
    if (player->y > SCREEN_HEIGHT - player->height) {
        player->y = SCREEN_HEIGHT - player->height; // Keep player within bounds
        player->vy = 0; // Stop downward movement
        player->isJumping = 0; // Reset jumping state
    }


    if (player->x >= SCREEN_WIDTH - player->width) {
        if (levelManager->currentLevel < levelManager->totalLevels - 1) {
            levelManager->currentLevel++;
            player->x = 0;
            player->y = playerStartPosition.y;
            printf("Changed to level %d\n", levelManager->currentLevel + 1);
        } else {
            player->x = SCREEN_WIDTH - player->width;
        }
    } else if (player->x <= 0) {
        if (levelManager->currentLevel > 0) {
            levelManager->currentLevel--;
            player->x = SCREEN_WIDTH - player->width;
            player->y = playerStartPosition.y;
            printf("Changed to level %d\n", levelManager->currentLevel + 1);
        } else {
            player->x = 0;
        }
    }



    printf("total: %i\nCURRENT: %i\n", currentLevel, levelManager->totalLevels);
}
