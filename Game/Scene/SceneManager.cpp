#include "pch.h"
#include "SceneManager.h"

#include"Game/InputManager/InputManager.h"
#include"Game/Scene/TextManager.h"
#include"Game/SEManager/SEManager.h"
SceneManager::SceneManager(BossManager&bossManager,Party&party)
    :m_nextSceneID          {}
    ,m_currentSceneID       {}
    ,m_previousSceneID      {}
    ,m_monsterCurrentDamge  {}
    ,m_transitionState      {}
    ,m_fieldScene           {bossManager,party}
    ,m_battleScene          {bossManager}
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Initialize(TextManager& textManager, SEManager&sound,InputManager& inputmanager, SceneManager& sceneManager,PlayerManager&playerManager, Map&map,Party&party,ImageManager&image)
{
    m_hasOnesActive = false;
    m_currentSceneID = SceneID::Title;
    m_nextSceneID = SceneID::None;

    m_transitionState = TransitionStateSceneManager::None;

    m_image = &image;
    m_titleScene.SetImage(&image);
    m_fieldScene.SetImage(&image);
    m_fieldScene.STtext.m_start = true;
    textManager.SetDisplayText();
    m_battleScene.SetImage(&image);
    m_gameOver.SetImage(&image);

    m_sound = &sound;
    m_titleScene.SetSound(&sound);
    m_fieldScene.SetSound(&sound);
    m_battleScene.SetSound(&sound);

    m_gameOver.Initialize();
    m_transitionManager.Initialize();

    InitializeCurrentScene(textManager,inputmanager,sceneManager,playerManager, map,party);

    for (int i = 0;i < MAX_PARTY;i++)//現在のパーティのHP
    {
        m_monsterCurrentDamge[i] = 0;
    }
    if (!m_hasOnesActive)
    {
        m_hasOnesActive = true;
    }
}

void SceneManager::Update(TextManager& textManager, InputManager& inputmanager,SceneManager&sceneManager,PlayerManager& playerManager, EnemyManager& enemyManager,Map&map,Party&party, Accessory& accessory)
{
    m_transitionManager.Update();
    switch (m_transitionState)
    {
    case TransitionStateSceneManager::None:
        // 現在シーン更新
        UpdateCurrentScene(textManager, inputmanager, sceneManager, playerManager, enemyManager, map, party, m_battleScene.GetBattle(), accessory);

        // シーン切り替え要求があれば切り替える
        if (m_nextSceneID != SceneID::None)
        {
            SetFadeOutRequest(map);
            m_transitionManager.StartFadeOut();
            m_transitionState = TransitionStateSceneManager::FadeOut;
        }
        break;
    case TransitionStateSceneManager::FadeOut:
        if (m_transitionManager.IsMax())
        {
            m_transitionState = TransitionStateSceneManager::ChangeScene;
        }
        break;
    case TransitionStateSceneManager::ChangeScene:
        ChangeScene(textManager,inputmanager, sceneManager, playerManager, map, party);

        SetFadeInRequest(map);
        m_transitionManager.StartFadeIn();
        m_transitionState = TransitionStateSceneManager::FadeIn;
        break;
    case TransitionStateSceneManager::FadeIn:
        if (m_transitionManager.IsNone())
        {
            m_transitionState = TransitionStateSceneManager::None;
        }
        break;
    }
}

void SceneManager::Render(TextManager& textManager, PlayerManager& playerManager, EnemyManager& enemyManager,Map&map,Party&party, Accessory& accessory)
{
    RenderCurrentScene(textManager, playerManager,enemyManager,map,party, accessory);
    m_transitionManager.Render();
}

void SceneManager::Finalize()
{

}



void SceneManager::NextSceneID(SceneID requestSceneID)
{
    m_nextSceneID = requestSceneID;
}

void SceneManager::ChangeScene(TextManager&textManager,InputManager& inputmanager, SceneManager& sceneManager, PlayerManager& playerManager, Map&map,Party&party)
{
    // 現在シーンの終了処理
    FinalizeCurrentScene();
    // 遷移前のSceneを保存
    m_previousSceneID = m_currentSceneID;
    // シーンIDの更新
    m_currentSceneID = m_nextSceneID;
    m_nextSceneID = SceneID::None;

    // 次のシーンの初期化
    InitializeCurrentScene(textManager,inputmanager,sceneManager,playerManager, map,party);
}

void SceneManager::InitializeCurrentScene(TextManager&textManager,InputManager& inputManager,SceneManager&sceneManager,PlayerManager&playerManager,Map&map,Party&party)
{
    switch (m_currentSceneID)
    {
    case SceneID::Title :   m_titleScene .Initialize(inputManager);  break;
    case SceneID::Field :   m_fieldScene .Initialize(*this,textManager,inputManager,playerManager,map);   break;
    case SceneID::Battle:   m_battleScene.Initialize(inputManager,sceneManager,map,party);   break;
    
    default:      assert(!"シーンIDが不正です");break;
    }
}

void SceneManager::UpdateCurrentScene(TextManager&textManager,InputManager&inputmanager,SceneManager&sceneManager,PlayerManager&playerManager,EnemyManager&enemyManager,Map&map,Party&party,Battle&battle,Accessory&accessory)
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
   
        m_fieldScene.Update(textManager,inputmanager,m_gameOver,playerManager,enemyManager,map,m_battleScene.GetBattle(), accessory, party);

        if (m_fieldScene.IsBattleRequested())
        {
            m_battleScene.SetPlayer(&playerManager);
            m_battleScene.SetEnemy(m_fieldScene.GetHitEnemy());
            m_battleScene.SetEnemyManager(&enemyManager);
            NextSceneID(SceneID::Battle);
        }

        break;

    case SceneID::Battle:

        m_battleScene.Update(inputmanager,sceneManager,m_fieldScene,m_gameOver,enemyManager,map,party,playerManager);
        if (m_battleScene.IsFieldRequested())
        {
            m_fieldScene.SetAttackEffects(m_battleScene.GetUsedAttackOrder());

            m_battleScene.ClearUsedAttackOrder();
            m_battleScene.SetFieldScene(&m_fieldScene);
            m_battleScene.SetEnemy(nullptr);

            NextSceneID(SceneID::Field);
        }
        if (m_battleScene.IsTitleRequested())
        {
            NextSceneID(SceneID::Title);

        }

        break;
    }
}

void SceneManager::RenderCurrentScene(TextManager& textManager, PlayerManager& playerManager, EnemyManager& enemyManager,Map&map,Party&party, Accessory& accessory)
{
    switch (m_currentSceneID)
    {
    case SceneID::Title:   m_titleScene.Render();  break;
    case SceneID::Field:    m_fieldScene.Render(textManager,m_gameOver,playerManager,enemyManager,map,accessory,party);   break;
    case SceneID::Battle:   m_battleScene.Render(m_gameOver,party,map);   break;


    default:      assert(!"シーンIDが不正です");break;
    }
}

void SceneManager::FinalizeCurrentScene()
{
    switch (m_currentSceneID)
    {
    case SceneID::Title:   m_titleScene.Finalize();m_sound->SoundStop(SEManager::SoundList::TitleBGM);  break;
    case SceneID::Field:    m_fieldScene.Finalize();m_sound->SoundStop(SEManager::SoundList::FieldBGM);   break;
    case SceneID::Battle:   m_battleScene.Finalize();m_sound->SoundStop(SEManager::SoundList::BattleBGM_Normal_1);m_sound->SoundStop(SEManager::SoundList::BattleBGM_Normal_2);m_sound->SoundStop(SEManager::SoundList::BattleBGM_Boss);   break;

    default:      assert(!"シーンIDが不正です");break;
    }
}

void SceneManager::SetFadeOutRequest(Map&map)////フェードアウト時
{
//TitletoPlay
    if (m_currentSceneID == SceneID::Title &&m_nextSceneID == SceneID::Field)
    { m_transitionManager.SetFadeType(TransitionManager::FadeType::TitletoPlayOut); };
//PlaytoTitle//
    //if
//FieldtoBattle
    if (m_currentSceneID == SceneID::Field &&m_nextSceneID == SceneID::Battle)
    {
        switch (map.GetBreakLevel())
        {
        case 0:
            m_transitionManager.SetFadeType(TransitionManager::FadeType::FieldtoBattle_1Out);
            printfDx(L"call_1Out");
            break;
        case 1:
            m_transitionManager.SetFadeType(TransitionManager::FadeType::FieldtoBattle_2Out);
            printfDx(L"call_2Out");
            break;
        case 2:
            m_transitionManager.SetFadeType(TransitionManager::FadeType::FieldtoBattle_3Out);
            printfDx(L"call_3Out");
            break;
        case 3:
            m_transitionManager.SetFadeType(TransitionManager::FadeType::FieldtoBattle_4Out);
            printfDx(L"call_4Out");
            break;
        case 4:
            m_transitionManager.SetFadeType(TransitionManager::FadeType::FieldtoBattle_5Out);
            printfDx(L"call_5Out");
            break;
        }
    }
//BattletoField
    if (m_currentSceneID == SceneID::Battle &&m_nextSceneID == SceneID::Field)
    {
        m_transitionManager.SetFadeType(TransitionManager::FadeType::BattletoField_1Out);
        m_transitionManager.SetFadeType(TransitionManager::FadeType::BattletoField_2Out);
    }
}

void SceneManager::SetFadeInRequest(Map&map)////フェードイン時
{
//TitletoPlay
    if (m_previousSceneID == SceneID::Title &&m_currentSceneID == SceneID::Field)
    { m_transitionManager.SetFadeType(TransitionManager::FadeType::TitletoPlayIn); };
//PlaytoTitle//
    //if
//FieldtoBattle
    if (m_previousSceneID == SceneID::Field &&m_currentSceneID == SceneID::Battle)
    {
        switch (map.GetBreakLevel())
        {
        case 0:
            m_transitionManager.SetFadeType(TransitionManager::FadeType::FieldtoBattle_1In);
            printfDx(L"call_1In");
            break;
        case 1:
            m_transitionManager.SetFadeType(TransitionManager::FadeType::FieldtoBattle_2In);
            printfDx(L"call_2In");
            break;
        case 2:
            m_transitionManager.SetFadeType(TransitionManager::FadeType::FieldtoBattle_3In);
            printfDx(L"call_3In");
            break;
        case 3:
            m_transitionManager.SetFadeType(TransitionManager::FadeType::FieldtoBattle_4In);
            printfDx(L"call_4In");
            break;
        case 4:
            m_transitionManager.SetFadeType(TransitionManager::FadeType::FieldtoBattle_5In);
            printfDx(L"call_5In");
            break;
        }
    }
//BattletoField
    if (m_previousSceneID == SceneID::Battle &&m_currentSceneID == SceneID::Field)
    {
        m_transitionManager.SetFadeType(TransitionManager::FadeType::BattletoField_1In);
        m_transitionManager.SetFadeType(TransitionManager::FadeType::BattletoField_2In);
    }
}


bool SceneManager::IsTitleRequested() const
{

    return m_battleScene.IsTitleRequested()||m_gameOver.IsTitleRequest();
}

void SceneManager::ResetTitleRequest()
{
    m_battleScene.ResetTitleRequest();
    m_gameOver.Initialize();
}