#include "raylib.h"
#include <iostream>
#include "level.hpp"

namespace Player
{
    // Player variables
    int x = 0;
    int y = 0;
    int width = 25;
    int height = 50;
    Rectangle rec = {x, y, width, height};
    bool isJumping = false;
    bool inAir = true;
    int jumpHeight = 100; // The height of the jump
    int jumpSpeed = 10; // The speed of the jump
    int gravityForce = 5; // The gravity pulling the player down
    bool jumpButtonHeld = false; // Whether the jump button is being held down

    // Player actions
    void jump()
    {
        if (!isJumping && !inAir) // Only start a new jump if the player is not already jumping and is not in the air
        {
            isJumping = true;
            jumpSpeed = 10; // Reset the jump speed
        }
    }

    void gravity()
    {
        if (isJumping)
        {
            y -= jumpSpeed; // Move the player up
            jumpSpeed -= 1; // Decrease the jump speed

            if (jumpSpeed <= 0) // If the jump speed reaches 0
            {
                isJumping = false; // End the jump
                if (jumpButtonHeld) // If the jump button is being held down
                {
                    jump(); // Start another jump
                }
            }
        }
        else if (inAir)
        {
            y += gravityForce; // Pull the player down
        }
        else
        {
            jumpButtonHeld = false; // The player has landed, so the jump button is not being held down
        }
    }

    void checkCollision()
    {
        Rectangle nextFrameRec = {x, y + gravityForce, width, height}; // The player's bounding box in the next frame

        if (CheckCollisionRecs(nextFrameRec, Level::N1::rec)) // If the player is about to hit the ground in the next frame
        {
            if (!jumpButtonHeld) // If the jump button is not being held down
            {
                inAir = false; // The player is not in the air
                isJumping = false; // The player is not jumping
            }
        }
        else
        {
            inAir = true; // The player is in the air
        }
    }

    void draw()
    {
        DrawRectangle(x, y, width, height, RED);
    }

    void update()
    {
        if (IsKeyDown(KEY_RIGHT))
        {
            x += 5;
        }
        if (IsKeyDown(KEY_LEFT))
        {
            x -= 5;
        }
        if (IsKeyDown(KEY_UP)) // If the UP key is being held down
        {
            jumpButtonHeld = true;
            jump();
        }
        else if (IsKeyUp(KEY_UP)) // If the UP key is released
        {
            jumpButtonHeld = false;
        }

        checkCollision();

        printf("%d\n", inAir);

        gravity();
    }
}