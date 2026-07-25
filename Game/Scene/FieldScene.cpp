#include "pch.h"
#include "Game/Scene/FieldScene.h"

#include"Game/Maths/Collisionall.h"
#include"Game/Battle/Battle.h"
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

void FieldScene::Update(InputManager& inputManager,PlayerManager& playerManager,EnemyManager& enemyManager,Map&map,Battle&battle)
{
    printfDx(L"現在 (%d,%d)\n",
        playerManager.m_position.x,
        playerManager.m_position.y);


    map.Update(inputManager, playerManager);
    playerManager.Update(&map);

    if (playerManager.m_oldposition != playerManager.m_position)
    {
        playerManager.m_invicible = false;
    }

    enemyManager.Update();

    Enemy* enemy = enemyManager.CheckHit(playerManager);

    if (!playerManager.m_invicible)
    {
        if (enemy)
        {
            playerManager.m_currentposition = playerManager.m_position;
            m_hitEnemy = enemy;
            m_isBattleRequested = true;
            playerManager.m_invicible = true;
        }
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
void FieldScene::ResetBattleRequest()
{
    m_isBattleRequested = false;
    m_hitEnemy = nullptr;
}