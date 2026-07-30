#include "pch.h"
#include "SceneManager.h"

#include"Game/InputManager/InputManager.h"

SceneManager::SceneManager()
    :m_nextSceneID{}
    ,m_currentSceneID{}
    ,m_monsterCurrentDamge{}
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Initialize(InputManager& inputmanager, SceneManager& sceneManager, Map&map,Party&party)
{

    m_currentSceneID = SceneID::Title;
    m_nextSceneID = SceneID::None;

    InitializeCurrentScene(inputmanager,sceneManager, map,party);

    for (int i = 0;i < MAX_PARTY;i++)//現在のパーティのHP
    {
        m_monsterCurrentDamge[i] = 0;
    }
}

void SceneManager::Update(InputManager& inputmanager,SceneManager&sceneManager,PlayerManager& playerManager, EnemyManager& enemyManager,Map&map,Party&party,Battle&battle)
{
    // 現在シーン更新
    UpdateCurrentScene(inputmanager,sceneManager,playerManager,enemyManager,map,party,battle);

    // シーン切り替え要求があれば切り替える
    if (m_nextSceneID != SceneID::None)
    {
        ChangeScene(inputmanager,sceneManager,map,party);
    }
}

void SceneManager::Render(PlayerManager& playerManager, EnemyManager& enemyManager,Map&map,Party&party)
{
    DrawFormatString(10, 100, GetColor(255, 255, 255), L"%d", m_currentSceneID);
    RenderCurrentScene(playerManager,enemyManager,map,party);
}

void SceneManager::Finalize()
{

}



void SceneManager::NextSceneID(SceneID requestSceneID)
{
    m_nextSceneID = requestSceneID;
}

void SceneManager::ChangeScene(InputManager& inputmanager, SceneManager& sceneManager, Map&map,Party&party)
{
    // 現在シーンの終了処理
    FinalizeCurrentScene();

    // シーンIDの更新
    m_currentSceneID = m_nextSceneID;
    m_nextSceneID = SceneID::None;

    // 次のシーンの初期化
    InitializeCurrentScene(inputmanager,sceneManager, map,party);
}

void SceneManager::InitializeCurrentScene(InputManager& inputmanager,SceneManager&sceneManager,Map&map,Party&party)
{
    switch (m_currentSceneID)
    {
    case SceneID::Title :   m_titleScene .Initialize(inputmanager);  break;
    case SceneID::Field :   m_fieldScene .Initialize(inputmanager,map);   break;
    case SceneID::Battle:   m_battleScene.Initialize(inputmanager,sceneManager,map,party);   break;
    
    default:      assert(!"シーンIDが不正です");break;
    }
}

void SceneManager::UpdateCurrentScene(InputManager&inputmanager,SceneManager&sceneManager,PlayerManager&playerManager,EnemyManager&enemyManager,Map&map,Party&party,Battle&battle)
{
    switch (m_currentSceneID)
    {
    case SceneID::Title:

        m_titleScene.Update(inputmanager);

        if (m_titleScene.IsStartRequested())
        {
            NextSceneID(SceneID::Field);
        }

        break;

    case SceneID::Field:
   
        m_fieldScene.Update(inputmanager,playerManager,enemyManager,map,battle);

        if (m_fieldScene.IsBattleRequested())
        {
            m_battleScene.SetPlayer(&playerManager);
            m_battleScene.SetEnemy(m_fieldScene.GetHitEnemy());
            NextSceneID(SceneID::Battle);
        }

        break;

    case SceneID::Battle:

        m_battleScene.Update(inputmanager,sceneManager,enemyManager,map,party,playerManager);
        //printfDx(L"FieldRequested!!\n");
        if (m_battleScene.IsFieldRequested())
        {
            m_fieldScene.SetAttackEffects(m_battleScene.GetUsedAttackOrder());

            m_battleScene.ClearUsedAttackOrder();

            NextSceneID(SceneID::Field);
        }
        if (m_battleScene.IsTitleRequested())
        {
            NextSceneID(SceneID::Title);

        }

        break;
    }
}

void SceneManager::RenderCurrentScene(PlayerManager& playerManager, EnemyManager& enemyManager,Map&map,Party&party)
{
    switch (m_currentSceneID)
    {
    case SceneID::Title:   m_titleScene.Render();  break;
    case SceneID::Field:    m_fieldScene.Render(playerManager,enemyManager,map);   break;
    case SceneID::Battle:   m_battleScene.Render(party);   break;


    default:      assert(!"シーンIDが不正です");break;
    }
}

void SceneManager::FinalizeCurrentScene()
{
    switch (m_currentSceneID)
    {
    case SceneID::Title:   m_titleScene.Finalize();  break;
    case SceneID::Field:    m_fieldScene.Finalize();   break;
    case SceneID::Battle:   m_battleScene.Finalize();   break;

    default:      assert(!"シーンIDが不正です");break;
    }
}


bool SceneManager::IsTitleRequested() const
{
    return m_battleScene.IsTitleRequested();
}

void SceneManager::ResetTitleRequest()
{
    m_battleScene.ResetTitleRequest();
}