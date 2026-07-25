#include"Game/Enemy/Enemy.h"

Enemy::Enemy()
{

}

void Enemy::Damage(int power)
{
    hp -= power;

    if (hp < 0)
    {
        hp = 0;
    }
}

int Enemy::GetPower()const
{
    return power;
}

int Enemy::GetHp() const
{
    return hp;
}


const wchar_t* Enemy::GetName() const
{
    return name;
}