#include "pch.h"
#include "Game/Enemy/EnemyManager.h"

#include"Game/Maths/Collisionall.h"
#include"Game/Enemy/Enemy.h"
#include "Game/Enemy/Slime.h"
#include "Game/Enemy/Wolf.h"
#include"Game/Enemy/Dragon.h"
EnemyManager::EnemyManager()
{
}

EnemyManager::~EnemyManager()
{
}

namespace
{
    const std::vector<EnemyData>enemyData =
    {
         {0,0,10,10},
        {0,0,15,12},
        {0,1,20,8},

        {1,0,5,5},
        {1,1,12,10},
        {1,2,20,15},
    };
}


void EnemyManager::Initialize(Map& map)
{
    m_enemies.clear();

    int mapNo = map.GetCurrentMap();

    for (auto& data : enemyData)
    {
        if (data.mapNo != mapNo)
            continue;
        switch (data.enemyType)
        {
        case 0:
            CreateSlime(data.x, data.y, map);
            break;

        case 1:
            CreateWolf(data.x, data.y, map);
            break;

        case 2:
            CreateDragon(data.x, data.y, map);
            break;
        }
    }


}

void EnemyManager::Update()
{
    for (auto& enemy : m_enemies)
    {
        enemy->Update();
    }
}

void EnemyManager::Render()
{
    for (auto& enemy : m_enemies)
    {
        enemy->Render();
    }
}

void EnemyManager::Finalize()
{
    for (auto& enemy : m_enemies)
    {
        enemy->Finalize();
    }
}

Enemy* EnemyManager::CheckHit(PlayerManager& playermanager)
{

    for (auto& enemy : m_enemies)
    {

        if (Collisionall::HitCharacter(playermanager, enemy.get()))
        {
            enemy->OnHit(playermanager);
            return enemy.get();

        }
    }

    return nullptr;
}
void EnemyManager::RemoveEnemy(Enemy* enemy)
{
    auto it = std::remove_if(
        m_enemies.begin(),
        m_enemies.end(),
        [enemy](const std::unique_ptr<Enemy>& e)
        {
            return e.get() == enemy;
        });

    m_enemies.erase(it, m_enemies.end());
}

void EnemyManager::CreateSlime(int x, int y, Map& map)
{
    auto slime = std::make_unique<Slime>();
    slime->Initialize(map, x, y);

    m_enemies.push_back(std::move(slime));
}

void EnemyManager::CreateWolf(int x, int y, Map& map)
{
    auto wolf = std::make_unique<Wolf>();
    wolf->Initialize(map, x, y);

    m_enemies.push_back(std::move(wolf));
}

void EnemyManager::CreateDragon(int x, int y, Map& map)
{
    auto dragon = std::make_unique<Dragon>();
    dragon->Initialize(map, x, y);

    m_enemies.push_back(std::move(dragon));
}