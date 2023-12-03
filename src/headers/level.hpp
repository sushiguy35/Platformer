#include "raylib.h"
#include <iostream>

namespace Level
{
    namespace N1
    {
        namespace Platform1
        {
            int x = 0;
            int y = 550;
            int width = 1000;
            int height = 50;
            rectangle rec = {x, y, width, height};
        }
        void draw()
        {
            DrawRectangle(rec, GREEN);
        }
    }
} 
