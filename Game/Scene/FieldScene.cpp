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

void FieldScene::Initialize(InputManager& inputmanager, Map& map)
{

    if (map.GetCurrentMap() == 0 && map.GetStageStartMap() == 0)
    {
        map.ChangeStage(0);
    }
    m_hitEnemy = nullptr;
    m_isBattleRequested = false;


}

void FieldScene::Update(InputManager& inputManager,PlayerManager& playerManager,EnemyManager& enemyManager,Map&map,Battle&battle)
{
   // printfDx(L"現在 (%d,%d)\n", playerManager.m_position.x,playerManager.m_position.y);



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
    if (m_playEffect)
    {
        m_effectTimer++;

        if (m_effectTimer >= 30)
        {
            m_effectTimer = 0;
            m_effectIndex++;

            if (m_effectIndex >= (int)m_attackEffects.size())
            {
                m_playEffect = false;
            }
        }
    }
}

void FieldScene::Render(PlayerManager& playerManager, EnemyManager& enemyManager,Map&map)
{
    map.Render();
    playerManager.Render(&map);
    enemyManager.Render();
    if (m_playEffect && m_effectIndex < m_attackEffects.size())
    {
        const auto& info = m_attackEffects[m_effectIndex];

        switch (info.element)//属性攻撃ごとのエフェクト描画
        {
        case Monster::CharacteRistics::Fire:
            DrawBox(0, 0, 1280, 720, GetColor(255, 80, 0), TRUE);
            break;

        case Monster::CharacteRistics::Water:
            DrawBox(0, 0, 1280, 720, GetColor(0, 120, 255), TRUE);
            break;

        case Monster::CharacteRistics::Grass:
            DrawBox(0, 0, 1280, 720, GetColor(0, 200, 0), TRUE);
            break;

        case Monster::CharacteRistics::Thunder:
            DrawBox(0, 0, 1280, 720, GetColor(255, 255, 0), TRUE);
            break;

        case Monster::CharacteRistics::Wind:
            DrawBox(0, 0, 1280, 720, GetColor(180, 180, 180), TRUE);
            break;

        default:
            break;
        }

        DrawString(20, 20, info.attackName.c_str(), GetColor(255, 255, 255));
    }
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

void FieldScene::SetAttackEffects( const std::vector<Battle::UsedAttackInfo>& effects)
{
    m_attackEffects = effects;
    m_effectIndex = 0;
    m_effectTimer = 0;
    m_playEffect = !effects.empty();
}