#include "player.h"
#include "utils.h"
#include <stdio.h>

#define PLAYER_SPEED 5.0f
#define JUMP_INITIAL_VELOCITY -10.0f
#define MAX_JUMP_TIME 15

void initPlayer(Player* player) {
    player->x = 100;
    player->y = 100;
    player->vx = 0;
    player->vy = 0;
    player->width = 32;
    player->height = 32;
    player->isJumping = 0;
    player->jumpTimer = 0;
}

void handlePlayerInput(Player* player, const Uint8* currentKeyStates) {
    player->vx = 0;
    if (currentKeyStates[SDL_SCANCODE_LEFT]) {
        player->vx = -PLAYER_SPEED;
    }
    if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
        player->vx = PLAYER_SPEED;
    }
    
    if (currentKeyStates[SDL_SCANCODE_SPACE]) {
        if (!player->isJumping) {
            player->isJumping = 1;
            player->jumpTimer = 0;
            player->vy = JUMP_INITIAL_VELOCITY;
        } else if (player->jumpTimer < MAX_JUMP_TIME) {
            player->vy = JUMP_INITIAL_VELOCITY;
            player->jumpTimer++;
        }
    } else {
        player->isJumping = 0;
    }
}

void updatePlayer(Player* player) {
    player->x += player->vx;
    player->y += player->vy;

    printf("Player position: (%.2f, %.2f), velocity: (%.2f, %.2f), isJumping: %d\n", 
           player->x, player->y, player->vx, player->vy, player->isJumping);  // Debug print
}

void renderPlayer(SDL_Renderer* renderer, Player* player) {
    SDL_Rect playerRect = {(int)player->x, (int)player->y, player->width, player->height};
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(renderer, &playerRect);
}
