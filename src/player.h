#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

typedef struct {
    float x, y;
    float vx, vy;
    int width, height;
    int isJumping;
} Player;

void initPlayer(Player* player);
void handlePlayerInput(Player* player, SDL_Event* e);
void updatePlayer(Player* player);
void renderPlayer(SDL_Renderer* renderer, Player* player);

#endif // PLAYER_H
