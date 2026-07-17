#include"Game/Enemy/Enemy.h"

void Enemy::Damage(int power)
{
    hp -= power;

    if (hp < 0)
    {
        hp = 0;
    }
}

int Enemy::GetHp() const
{
    return hp;
}

