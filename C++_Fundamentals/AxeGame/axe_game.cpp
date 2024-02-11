#include "raylib.h"

int main()
{
    // window dimensions
    int width = 800;
    int height = 450;
    InitWindow(width, height, "Axe Game!");

    // circle coordinates
    int circleX{200};
    int circleY{200};
    int circleRadius{25};
    // circle edges
    int L_circleX{circleX - circleRadius};
    int R_circleX{circleX + circleRadius};
    int U_circleY{circleY - circleRadius};
    int B_circleY{circleY + circleRadius};

    // axe coordinates
    int axeX{400};
    int axeY{0};
    int axeLenght{50};
    // axe edges
    int L_aceX{axeX};
    int R_axeX{axeX + axeLenght};
    int U_axeY{axeY};
    int B_axeY{axeY + axeLenght};

    // axe movement
    int direction{10};

    bool collisionWithAxe = (B_axeY >= U_circleY) && 
                            (U_axeY <= B_circleY) && 
                            (R_axeX >= L_circleX) && 
                            (L_aceX <= R_circleX);

    SetTargetFPS(60);
    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        if(collisionWithAxe)
        {
            DrawText("Game Over!",400,200,20,RED);
        }
        else
        {
            /// game logic starts
            
            // update the edges
            L_circleX = circleX - circleRadius;
            R_circleX = circleX + circleRadius;
            U_circleY = circleY - circleRadius;
            B_circleY = circleY + circleRadius;
            L_aceX = axeX;
            R_axeX = axeX + axeLenght;
            U_axeY = axeY;
            B_axeY = axeY + axeLenght;
            // update collision
            collisionWithAxe =  (B_axeY >= U_circleY) && 
                                (U_axeY <= B_circleY) && 
                                (R_axeX >= L_circleX) && 
                                (L_aceX <= R_circleX);

            // draw circle
            DrawCircle(circleX,circleY,circleRadius,GREEN);
            DrawRectangle(axeX,axeY,axeLenght,axeLenght,RED);

            // move the axe
            axeY += direction;
            if(axeY > height || axeY < 0)
            {
              direction = -direction;
            }
        
            // moving circle
            if(IsKeyDown(KEY_D) && (circleX + circleRadius) < width)
            {
                circleX += 10;
            }
            if(IsKeyDown(KEY_A) && (circleX - circleRadius) > 0)
            {
                circleX -= 10;
            }
            if(IsKeyDown(KEY_W) && (circleY - circleRadius) > 0)
            {
                circleY -= 10;
            }
            if(IsKeyDown(KEY_S) && (circleY + circleRadius) < height)
            {
                circleY += 10;
            }

            /// game logic ends
        }

        EndDrawing();
    }
}