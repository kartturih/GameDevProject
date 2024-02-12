#include "raylib.h"
#include "BaseCharacter.h"
#include "Character.h"
#include <string>

class Enemy : public BaseCharacter
{
public:
    Enemy(Vector2 pos, Texture2D idle, Texture2D run);
    virtual void tick(float deltaTime) override;
    void setTarget(Character* character){target = character;};
    virtual Vector2 getScreenPos() override;
    int getHealth() const {return health;}
    void takeDamage(int damage);
    //std::string getHealthBar() const {return healthBar;}

private:
    Character* target;
    //std::string healthBar = " ";
    float DPS{10.f};
    float radius{100.f};
    int health{50};
};
