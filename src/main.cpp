#include "raylib.h"
#include "headers/player.hpp"
#include <iostream>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 600

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib [core] example - basic window");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
        
            Player::draw();
            Player::update();

            Level::N1::draw();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}