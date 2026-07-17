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

void EnemyManager::Initialize(Map& map)
{
    CreateSlime(10, 10, map);
    CreateSlime(15, 12, map);
    CreateSlime(18, 8, map);
    CreateSlime(25, 20, map);

    CreateWolf(3, 8, map);
    CreateWolf(20, 10, map);
    CreateWolf(15, 6, map);

    CreateDragon(20, 20, map);
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