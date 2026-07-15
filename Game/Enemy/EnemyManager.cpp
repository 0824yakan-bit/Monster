#include "pch.h"
#include "Game/Enemy/EnemyManager.h"

#include"Game/Maths/Collisionall.h"
#include"Game/Enemy/Enemy.h"
#include "Game/Enemy/Slime.h"
#include "Game/Enemy/Wolf.h"
EnemyManager::EnemyManager()
{
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Initialize(Map& map)
{
    auto slime = std::make_unique<Slime>();
    auto wolf  = std::make_unique<Wolf>();

    slime->Initialize(map);
    wolf->Initialize(map);

    m_enemies.push_back(std::move(slime));
    m_enemies.push_back(std::move(wolf));
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