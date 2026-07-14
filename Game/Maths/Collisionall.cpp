#include "pch.h"
#include "Game/Maths/Collisionall.h"
#include "Game/Enemy/Enemy.h"
bool Collisionall::AABB(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2)
{
    return(x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2);
}
bool Collisionall::HitCharacter(PlayerManager& player, Enemy* enemy)
{
    return AABB(
        player.m_position.x,
        player.m_position.y,
        player.m_size,
        player.m_size,

        enemy->position.x,
        enemy->position.y,
        enemy->size.x,
        enemy->size.y);
}
