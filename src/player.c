#include "player.h"
#include "utils.h"

#define PLAYER_SPEED 5.0f
#define JUMP_VELOCITY -10.0f
#define GRAVITY 0.5f

void initPlayer(Player* player) {
    player->x = 100;
    player->y = 100;
    player->vx = 0;
    player->vy = 0;
    player->width = 32;
    player->height = 32;
    player->isJumping = 0;
}

void handlePlayerInput(Player* player, SDL_Event* e) {
    if (e->type == SDL_KEYDOWN && e->key.repeat == 0) {
        switch (e->key.keysym.sym) {
            case SDLK_LEFT:
                player->vx = -PLAYER_SPEED;
                break;
            case SDLK_RIGHT:
                player->vx = PLAYER_SPEED;
                break;
            case SDLK_SPACE:
                if (!player->isJumping) {
                    player->vy = JUMP_VELOCITY;
                    player->isJumping = 1;
                }
                break;
        }
    } else if (e->type == SDL_KEYUP && e->key.repeat == 0) {
        switch (e->key.keysym.sym) {
            case SDLK_LEFT:
                if (player->vx < 0) player->vx = 0;
                break;
            case SDLK_RIGHT:
                if (player->vx > 0) player->vx = 0;
                break;
        }
    }
}

void updatePlayer(Player* player) {
    player->x += player->vx;
    player->y += player->vy;
    player->vy += GRAVITY;

    // Simple ground collision
    if (player->y > SCREEN_HEIGHT - player->height) {
        player->y = SCREEN_HEIGHT - player->height;
        player->vy = 0;
        player->isJumping = 0;
    }
}

void renderPlayer(SDL_Renderer* renderer, Player* player) {
    SDL_Rect playerRect = {(int)player->x, (int)player->y, player->width, player->height};
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(renderer, &playerRect);
}
