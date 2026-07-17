#include "pch.h"
#include "SceneManager.h"

#include"Game/InputManager/InputManager.h"
SceneManager::SceneManager()
    :m_nextSceneID{}
    ,m_currentSceneID{}
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Initialize(InputManager& inputmanager,Map&map,Party&party)
{
    m_playerManager.Initialize(&map);

    m_enemyManager.Initialize(map);
    m_currentSceneID = SceneID::Title;
    m_nextSceneID = SceneID::None;

    InitializeCurrentScene(inputmanager,map,party);
}

void SceneManager::Update(InputManager& inputmanager,PlayerManager& playerManager, EnemyManager& enemyManager,Map&map,Party&party)
{
    // 現在シーン更新
    UpdateCurrentScene(inputmanager,playerManager,enemyManager,map,party);

    // シーン切り替え要求があれば切り替える
    if (m_nextSceneID != SceneID::None)
    {
        ChangeScene(inputmanager,map,party);
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

void SceneManager::ChangeScene(InputManager& inputmanager,Map&map,Party&party)
{
    // 現在シーンの終了処理
    FinalizeCurrentScene();

    // シーンIDの更新
    m_currentSceneID = m_nextSceneID;
    m_nextSceneID = SceneID::None;

    // 次のシーンの初期化
    InitializeCurrentScene(inputmanager,map,party);
}

void SceneManager::InitializeCurrentScene(InputManager& inputmanager,Map&map,Party&party)
{
    switch (m_currentSceneID)
    {
    case SceneID::Title :   m_titleScene .Initialize(inputmanager);  break;
    case SceneID::Field :   m_fieldScene .Initialize(inputmanager);   break;
    case SceneID::Battle:   m_battleScene.Initialize(inputmanager,map,party);   break;
    
    default:      assert(!"シーンIDが不正です");break;
    }
}

void SceneManager::UpdateCurrentScene(InputManager&inputmanager,PlayerManager&playerManager,EnemyManager&enemyManager,Map&map,Party&party)
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

        m_fieldScene.Update(inputmanager,playerManager,enemyManager,map);

        if (m_fieldScene.IsBattleRequested())
        {
            m_battleScene.SetEnemy(m_fieldScene.GetHitEnemy());
            NextSceneID(SceneID::Battle);
        }

        break;

    case SceneID::Battle:

        m_battleScene.Update(inputmanager,enemyManager,party);
        printfDx(L"FieldRequested!!\n");
        if (m_battleScene.IsFieldRequested())
        {
            NextSceneID(SceneID::Field);
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