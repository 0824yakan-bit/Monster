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

Vector2 Enemy::GetPosition()
{
    return Vector2(position.x,position.y);
}

void Enemy::SetBattleEnemy(bool battleEnemy)
{
    m_isBattleEnemy = battleEnemy;
}

bool Enemy::IsBattleEnemy() const
{
    return m_isBattleEnemy;
}

void Enemy::SetBoss(int bossNo)
{
    m_isBoss = true;
    m_bossNo = bossNo;
}

bool Enemy::IsBoss() const
{
    return m_isBoss;
}

int Enemy::GetBossNo() const
{
    return m_bossNo;
}

Monster::Type Enemy::GetMonsterType() const
{
    switch (type)
    {
    case EnemyType::Slime:
        return Monster::Type::Slime;

    case EnemyType::Wolf:
        return Monster::Type::Wolf;

    case EnemyType::Dragon:
        return Monster::Type::Dragon;

    case EnemyType::Golem:
        return Monster::Type::Golem;

    case EnemyType::Fairy:
        return Monster::Type::Fairy;
    }

    return Monster::Type::Slime;
}