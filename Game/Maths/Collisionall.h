#pragma once
#include "Game/Player/PlayerManager.h"
#include "Game/Enemy/EnemyManager.h"

class Collisionall
{
public:
    int x;
    int y;
    int width;
    int height;


    static bool AABB(
        int x1, int y1,
        int w1, int h1,
        int x2, int y2,
        int w2, int h2);

    static bool HitCharacter(PlayerManager& player, Enemy* enemy);
};