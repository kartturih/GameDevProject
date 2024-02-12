#include "raylib.h"
#include "raymath.h"
#include "Character.h"
#include "Prop.h"
#include "Enemy.h"
#include <string>

int main()
{
    const int window{1200};
    InitWindow(window, window, "Classy Clash!");

    Texture2D map = LoadTexture("nature_tileset/OpenWorldMap24x24.png");
    Vector2 mapPos{0.0, 0.0};
    const float mapScale{12.f};

    Character knight{window, window};

    Prop props[2]{
        Prop{Vector2{2000.f, 700.f}, LoadTexture("nature_tileset/Rock.png")},
        Prop{Vector2{1200.f, 700.f}, LoadTexture("nature_tileset/Log.png")},
    };

    Enemy slime{
        Vector2{900.f, 1200.f},
        LoadTexture("characters/slime_idle_spritesheet.png"),
        LoadTexture("characters/slime_run_spritesheet.png")};

    Enemy goblin{
        Vector2{800.f, 300.f},
        LoadTexture("characters/goblin_idle_spritesheet.png"),
        LoadTexture("characters/goblin_run_spritesheet.png")};

    Enemy *enemies[]{
        &goblin,
        &slime};

    for (auto enemy : enemies)
    {
        enemy->setTarget(&knight);
    }
    goblin.setTarget(&knight);

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        mapPos = Vector2Scale(knight.getWorldPos(), -1.f);

        // draw the map
        DrawTextureEx(map, mapPos, 0.0, mapScale, WHITE);

        // draw the props
        for (auto prop : props)
        {
            prop.Render(knight.getWorldPos());
        }

        if (!knight.getAlive())
        {
            DrawText("GAME OVER!", window / 3.5, window / 2, 100, RED);
            EndDrawing();
            continue;
        }
        else
        {
            std::string knightsHealth = "Health: ";

            if (knight.getHealth() < 100.f)
            {
                knightsHealth.append(std::to_string(knight.getHealth()), 0, 2);
            }
            else if (knight.getHealth() < 10.f)
            {
                knightsHealth.append(std::to_string(knight.getHealth()), 0, 1);
            }
            else
            {
                knightsHealth.append(std::to_string(knight.getHealth()), 0, 3);
            }
            DrawText(knightsHealth.c_str(), 55.f, 45.f, 80.f, RED);
        }

        knight.tick(GetFrameTime());
        // check map bounds
        if (knight.getWorldPos().x < 0.f ||
            knight.getWorldPos().y < 0.f ||
            knight.getWorldPos().x + window > map.width * mapScale ||
            knight.getWorldPos().y + window > map.width * mapScale)
        {
            knight.undoMovement();
        }
        // check prop collisions
        for (auto prop : props)
        {
            if (CheckCollisionRecs(knight.getCollisionRec(), prop.getCollisionRec(knight.getWorldPos())))
            {
                knight.undoMovement();
            }
        }

        for (auto enemy : enemies)
        {
            enemy->tick(GetFrameTime());
        }

        if (IsKeyPressed(KEY_SPACE))
        {
            for (auto enemy : enemies)
            {
                if (CheckCollisionRecs(knight.getWeaponCollisionRec(), enemy->getCollisionRec()))
                {
                    enemy->takeDamage(knight.getDamage());
                }
            }
        }

        EndDrawing();
    }
    UnloadTexture(map);
    CloseWindow();
}