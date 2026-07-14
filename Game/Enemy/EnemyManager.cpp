#include "pch.h"
#include "Game/Enemy/EnemyManager.h"
#include"Game/Enemy/Enemy.h"
#include "Game/Enemy/Slime.h"
EnemyManager::EnemyManager()
{
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Initialize(Map& map)
{
    auto slime = std::make_unique<Slime>();

    slime->Initialize(map);

    m_enemies.push_back(std::move(slime));
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