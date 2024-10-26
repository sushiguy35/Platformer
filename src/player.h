#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include "utils.h"

typedef struct {
    float x, y;
    float vx, vy;
    int width, height;
    int isJumping;
    int jumpForce;
    int canJump;
} Player;

void initPlayer(Player* player);
void handlePlayerInput(Player* player, const Uint8* currentKeyStates);
void updatePlayer(Player* player);
void renderPlayer(SDL_Renderer* renderer, Player* player);
void setPlayerStartPos(Player* player, vec2 playerStartPos);

#endif // PLAYER_H
