#pragma once
#include "Game/Player/PlayerManager.h"
#include "Game/Enemy/EnemyManager.h"

class Collisionall
{
public:
    float x;
    float y;
    float width;
    float height;


    static bool AABB(
        float x1, float y1,
        float w1, float h1,
        float x2, float y2,
        float w2, float h2);

    static bool HitCharacter(PlayerManager& player, Enemy* enemy);

    //static bool HitShotEnemy(Shot& shot, Character& enemy);

    //static bool HitShotPlayer(BossAction& bossaction, Character& player);
};