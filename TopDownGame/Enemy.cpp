#include "enemy.h"
#include "Character.h"
#include "raymath.h"

Enemy::Enemy(Vector2 pos, Texture2D idleTex, Texture2D runTex)
{
    worldPos = pos;
    texture = idleTex;
    idle = idleTex;
    run = runTex;
    width = texture.width / maxFrames;
    height = texture.height;
    speed = 5.f;
}

void Enemy::tick(float deltaTime)
{
    if (!getAlive()) return;
    // get toTarget
    velocity = Vector2Subtract(target->getScreenPos(), getScreenPos());
    if(Vector2Length(velocity) < radius) velocity = {};
    BaseCharacter::tick(deltaTime);

    if(CheckCollisionRecs(getCollisionRec(), target->getCollisionRec()))
    {
        target->takeDamage(DPS * deltaTime);
    }
}

Vector2 Enemy::getScreenPos()
{ 
    return Vector2Subtract(worldPos, target->getWorldPos());
}
