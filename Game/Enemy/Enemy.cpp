#include"Game/Enemy/Enemy.h"

Enemy::Enemy()
    :m_image    {}
    ,type       {}
    ,power      {}
    ,name       {}
    ,moveTimer  {}
    ,hp         {}
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

void Enemy::SetImage(ImageManager* image)
{
    m_image = image;
}
