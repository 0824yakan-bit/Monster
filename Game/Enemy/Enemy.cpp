#include"Game/Enemy/Enemy.h"
#include"Game/Map/Map.h"
#include "Game/Party/Party.h"

Enemy::Enemy()
    :m_image    {}
    ,type       {}
    ,power      {}
    ,name       {}
    ,moveTimer  {}
    ,hp         {}
    ,moveCounter{}
    ,level      {}
    ,direction  {}
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

Vector2 Enemy::GetSize()
{
    return Vector2(size.x, size.y);
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

bool Enemy::IsFalling() const
{
    return m_isFalling;
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

    case EnemyType::Fairy:
        return Monster::Type::Fairy;

    case EnemyType::Turtle:
        return Monster::Type::Turtle;

    case EnemyType::Mole:
        return Monster::Type::Mole;
    
    case EnemyType::Fox:
        return Monster::Type::Fox;
    
    case EnemyType::Golem:
        return Monster::Type::Golem;

    case EnemyType::Phoenix:
        return Monster::Type::Phoenix;

    case EnemyType::Dragon:
        return Monster::Type::Dragon;

    case EnemyType::Daemon:
        return Monster::Type::Daemon;

    }

    return Monster::Type::Slime;
}

void Enemy::SetEnemyStats(Map& map,Party&party, int basePower, int baseHp)
{
    int level = map.GetBreakLevel();
    int partycount = party.GetMonsterCount();
    hp = baseHp+5*(level)+10*(partycount-1);
    power = basePower + 2 * (level+partycount-1);
}

void Enemy::SetEnemyId(int id)
{
    m_enemyId = id;
}

int Enemy::GetEnemyId() const
{
    return m_enemyId;
}

void Enemy::SetAttackNames(const std::wstring& singleAttackName, const std::wstring& allAttackName)
{
    m_singleAttackName = singleAttackName;
    m_allAttackName = allAttackName;
}

const std::wstring& Enemy::GetSingleAttackName() const
{
    return m_singleAttackName;
}

const std::wstring& Enemy::GetAllAttackName() const
{
    return m_allAttackName;
}
