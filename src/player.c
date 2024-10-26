#include "player.h"
#include "utils.h"
#include <stdio.h>
#include <stdbool.h>

#define PLAYER_SPEED 5.0f
#define MAX_JUMP_TIME 15

void initPlayer(Player* player) {
    player->x = 100;
    player->y = 100;
    player->vx = 0;
    player->vy = 0;
    player->width = 32;
    player->height = 32;
    player->isJumping = 0;
    player->jumpForce = -12;
    player->canJump = 0;
}

void setPlayerStartPos(Player* player, vec2 playerStartPos)
{
    player->x = playerStartPos.x;
    player->y = playerStartPos.y;
}

void handlePlayerInput(Player* player, const Uint8* currentKeyStates) {
    player->vx = 0;
    if (currentKeyStates[SDL_SCANCODE_LEFT]) {
        player->vx = -PLAYER_SPEED;
    }
    if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
        player->vx = PLAYER_SPEED;
    }
    if (currentKeyStates[SDL_SCANCODE_UP]) {
        if (!player->isJumping && player->canJump) {
            player->isJumping = 1;
            player->vy = player->jumpForce;
        }
    }
}

void updatePlayer(Player* player) {
    player->x += player->vx;
    player->y += player->vy;
}

void renderPlayer(SDL_Renderer* renderer, Player* player) {
    SDL_Rect playerRect = {(int)player->x, (int)player->y, player->width, player->height};
    SDL_SetRenderDrawColor(renderer, 0x01, 0xF0, 0xF0, 0xFF);
    SDL_RenderFillRect(renderer, &playerRect);
}
