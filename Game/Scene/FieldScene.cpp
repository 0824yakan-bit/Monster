#include "pch.h"
#include "Game/Scene/FieldScene.h"

#include"Game/Maths/Collisionall.h"
FieldScene::FieldScene()
    :m_hitEnemy{nullptr}
    ,m_isBattleRequested{false}
{
}

FieldScene::~FieldScene()
{
}

void FieldScene::Initialize(InputManager& inputmanager)
{
    
    m_hitEnemy = nullptr;
    m_isBattleRequested = false;

}

void FieldScene::Update(InputManager& inputManager,PlayerManager& playerManager,EnemyManager& enemyManager,Map&map)
{
    map.Update(inputManager,playerManager);
    playerManager.Update(&map);
    enemyManager.Update();
    Enemy* enemy = enemyManager.CheckHit(playerManager);

    if (enemy)
    {
        m_hitEnemy = enemy;
        m_isBattleRequested = true;
    }
}

void FieldScene::Render(PlayerManager& playerManager, EnemyManager& enemyManager,Map&map)
{
    map.Render();
    playerManager.Render(&map);
    enemyManager.Render();
}

void FieldScene::Finalize()
{

}

bool FieldScene::IsBattleRequested()const
{
	return m_isBattleRequested;
}

Enemy* FieldScene::GetHitEnemy() const
{
    return m_hitEnemy;
}