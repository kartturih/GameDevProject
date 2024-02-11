#include "raylib.h"

struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

bool isOnGround(AnimData data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rec.height;
}

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)
{
    //update running time
    data.runningTime += deltaTime;
    if (data.runningTime >= data.updateTime)
    {
        data.runningTime = 0.0;
        // update animation frame
        data.rec.x = data.frame * data.rec.width;
        data.frame++;
        if (data.frame > maxFrame)
        {
            data.frame = 0;
        }
    }
    return data;
}

int main()
{
    // window dimensions
    int windowDimensions[2];
    windowDimensions[0] = 600;
    windowDimensions[1] = 450;

    // initialize window
    InitWindow(windowDimensions[0],windowDimensions[1],"Dapper Dasher!");

    // acceleration due to gravity (pixels/second)/second
    const int gravity{1000};

    // scarfy variables
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    AnimData scarfyData;
    scarfyData.rec.width = scarfy.width/6;
    scarfyData.rec.height = scarfy.height;
    scarfyData.rec.x = 0;
    scarfyData.rec.y = 0;
    scarfyData.pos.x = windowDimensions[0]/4 - scarfyData.rec.width/2;
    scarfyData.pos.y = windowDimensions[1] - scarfyData.rec.height;
    scarfyData.frame = 0;
    scarfyData.updateTime = 1.0 / 10.0;
    scarfyData.runningTime = 0.0;

     // nebula variables
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

    // array for nebulae
    const int sizeOfNebulae{10};
    AnimData nebulae[sizeOfNebulae]{};

    for(int i = 0; i < sizeOfNebulae; i++)
    {
        nebulae[i].rec.x = 0.0;
        nebulae[i].rec.y = 0.0;
        nebulae[i].rec.width = nebula.width/8;
        nebulae[i].rec.height = nebula.height/8;
        nebulae[i].pos.y = windowDimensions[1] - nebula.height/8;
        nebulae[i].frame = 0;
        nebulae[i].runningTime = 0.0;
        nebulae[i].updateTime = 1.0 / 16.0; 
        nebulae[i].pos.x = windowDimensions[0] + i * 400;
    }

    // finish line
    float finishLine{nebulae[sizeOfNebulae-1].pos.x};

    // nebula x velocity (pixels/second)
    int nebulaVel{-200};

    // jump velocity(pixels/second)
    const int jumpVel{-650};
    // is rectangle in air?
    bool isInAir{};

    int velocity{0};

    Texture2D background = LoadTexture("textures/far-buildings.png");
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float bgX{};
    float mgX{};
    float fgX{};

    bool collision{};
    SetTargetFPS(60);
    while(!WindowShouldClose())
    {        
        // delta time(time since last frame)
        const float dT{GetFrameTime()};

        // start drawing
        BeginDrawing();
        ClearBackground(WHITE);

        bgX -= 20 * dT;
        if(bgX <= -background.width*2.5)
        {
            bgX = 0.0;
        }
        mgX -= 40 * dT;
        if(mgX <= -background.width*2.5)
        {
            mgX = 0.0;
        }
        fgX -= 80 * dT;
        if(fgX <= -background.width*2.5)
        {
            fgX = 0.0;
        }

        // draw background
        Vector2 bg1Pos{bgX, 0.0};
        DrawTextureEx(background, bg1Pos, 0.0, 2.5, WHITE);
        Vector2 bg2Pos{bgX + background.width*2.5, 0.0};
        DrawTextureEx(background, bg2Pos, 0.0, 2.5, WHITE);
        // draw midground
        Vector2 mgPos{mgX, 0.0};
        DrawTextureEx(midground, mgPos, 0.0, 2.5, WHITE);
        Vector2 mg2Pos{mgX + background.width*2.5, 0.0};
        DrawTextureEx(midground, mg2Pos, 0.0, 2.5, WHITE);
        // draw foreground
        Vector2 fgPos{fgX, 0.0};
        DrawTextureEx(foreground, fgPos, 0.0, 2.5, WHITE);
        Vector2 fg2Pos{fgX + background.width*2.5, 0.0};
        DrawTextureEx(foreground, fg2Pos, 0.0, 2.5, WHITE);

        // ground check
        if(isOnGround(scarfyData, windowDimensions[1]))
        {
            // rectangle is on the ground
            velocity = 0;
            isInAir = false;
        }
        else
        {
            // rectangle is in the air
            velocity += gravity * dT;
            isInAir = true;
        }
        
        // check for jumping
        if(IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocity += jumpVel;
        }
 
        // update scarfy position
        scarfyData.pos.y += velocity * dT;

        for(int i = 0; i < sizeOfNebulae; i++)
        {   
            // update the position of each nebula
            nebulae[i].pos.x += nebulaVel * dT;
        }

        // update finish line
        finishLine += nebulaVel * dT;

        if(!isInAir)
        {
            scarfyData = updateAnimData(scarfyData, dT, 5);
        }

        for(int i = 0; i < sizeOfNebulae; i++)
        {
            nebulae[i] = updateAnimData(nebulae[i], dT, 7);
        }
        
        for (AnimData nebula : nebulae)
        {
            float pad{50};
            Rectangle nebulaRec{
                nebula.pos.x + pad,
                nebula.pos.y + pad,
                nebula.rec.width - 2*pad,
                nebula.rec.height - 2*pad
            };
            Rectangle scarfyRec{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width,
                scarfyData.rec.height
            };
            if(CheckCollisionRecs(nebulaRec, scarfyRec))
            {
                collision = true;
            }
        }

        if(collision)
        {
            // lose the game
            DrawText("GAME OVER!", windowDimensions[0] / 4, windowDimensions[1] / 2, 50, MAGENTA);
        }
        else if(scarfyData.pos.x >= finishLine)
        {
            // WIN GAME!
            DrawText("YOU WIN!", windowDimensions[0] / 4.3, windowDimensions[1] / 2.2, 70, GREEN);
        }      
        else
        {
            // draw scarfy
            DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);

            // draw nebula
            for(int i = 0; i < sizeOfNebulae; i++)
            {
                DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
            }
        }

        // stop drawing
        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();
}