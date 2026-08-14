#include "pch.h"
#include "Game/Maths/Collisionall.h"
#include "Game/Enemy/Enemy.h"
bool Collisionall::AABB(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
    return(x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2);
}
bool Collisionall::HitCharacter(PlayerManager& player, Enemy* enemy)
{
    return AABB(
        player.m_position.x,
        player.m_position.y,
        player.m_size.x,
        player.m_size.y,

        enemy->position.x,
        enemy->position.y,
        enemy->size.x,
        enemy->size.y);
}
