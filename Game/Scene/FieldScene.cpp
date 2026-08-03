#include "pch.h"
#include "Game/Scene/FieldScene.h"

#include"Game/Maths/Collisionall.h"
#include"Game/Battle/Battle.h"
FieldScene::FieldScene()
    :m_hitEnemy{nullptr}
    ,m_isBattleRequested{false}
    , m_isMapActive{ false }
    ,m_isMenuActive{false}
    ,m_menuList{MenuList::None}
    ,m_menuListSelect{-1}
{
}

FieldScene::~FieldScene()
{
}

void FieldScene::Initialize(InputManager& inputmanager, Map& map)
{
    m_isMapActive = false;
    m_isMenuActive = false;

    if (map.GetCurrentMap() == 0 && map.GetStageStartMap() == 0)
    {
        map.ChangeStage(0);
    }
    m_hitEnemy = nullptr;
    m_isBattleRequested = false;

    Mposition.x = 10.0f;
    Mposition.y = 10.0f;
    
    Nposition.x = 50.0f;
    Nposition.y = 50.0f;

    size.x = 50.0f;
    size.y = 50.0f;
}

void FieldScene::Update(InputManager& inputManager,PlayerManager& playerManager,EnemyManager& enemyManager,Map&map,Battle&battle)
{
    inputManager.Update();
    map.Update(inputManager, playerManager);

////マップ表示・メニュー表示状態管理
   // printfDx(L"現在 (%d,%d)\n", playerManager.m_position.x,playerManager.m_position.y);
    if(inputManager.IsTrigger(KEY_INPUT_M)&&!m_isMapActive&&!m_isMenuActive)
    {
        m_isMapActive=true; //マップ表示
    }
    else if (inputManager.IsTrigger(KEY_INPUT_M) && m_isMapActive)
    {
        m_isMapActive = false;//マップ非表示
    }

    if (inputManager.IsTrigger(KEY_INPUT_N) && !m_isMenuActive&&!m_isMapActive)
    {
        m_menuListSelect = 0;//選択中を一番目に設定
        m_menuList = MenuList::None;
        m_isMenuActive = true; //マップ表示
    }
    else if (inputManager.IsTrigger(KEY_INPUT_N) && m_isMenuActive)
    {
        m_isMenuActive = false;//マップ非表示
    }

    if (m_isMenuActive)
    {
        if (inputManager.IsTrigger(KEY_INPUT_UP))
        {
            m_menuListSelect--;
        }
        if (inputManager.IsTrigger(KEY_INPUT_DOWN))
        {
            m_menuListSelect++;
        }
        if (inputManager.IsTrigger(KEY_INPUT_RETURN))
        {
            switch (m_menuListSelect)
            {
            case 0:
                m_menuList = MenuList::CooperativeMove;
                break;
            case 1:
                m_menuList = MenuList::PartyCheck;
                break;
            case 2:
                m_menuList = MenuList::ToolCheck;
                break;
            }//switch (m_menuListSelect)

        }//(inputManager.IsTrigger(KEY_INPUT_ESCAPE))
        else if (inputManager.IsTrigger(KEY_INPUT_BACK))
        {
            m_menuList = MenuList::None;
            m_menuListSelect = 0;
        }
        switch (m_menuList)
        {
        case MenuList::CooperativeMove:
            UpdateCooperativeMove();
            break;
        case MenuList::PartyCheck:
            UpdatePartyCheck();
            break;
        case MenuList::ToolCheck:
            UpdateToolCheck();
            break;

        }//switch (m_menuList)

    }// if(m_isMenuActive)

/////プレイヤー管理

    if (!m_isMapActive && !m_isMenuActive)playerManager.Update(&map);


    if (playerManager.m_oldposition != playerManager.m_position)
    {
        playerManager.m_invicible = false;
    }
/////エネミー管理
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
////エフェクト管理
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
/////
    

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

    if (m_isMapActive)
    {
        DrawBox(200, 200, 900, 900, GetColor(0, 0, 0), TRUE);
        DrawString(500, 500, L"マップオープン", GetColor(255, 255, 255), TRUE);
    } 
    if (m_isMenuActive)
    {
        DrawBox(50, 50, 250, 400, GetColor(255, 255, 255), TRUE);
        DrawString(500, 500, L"メニューオープン", GetColor(0, 0, 0), TRUE);
        switch (m_menuList)
        {
        case MenuList::CooperativeMove:
            RenderCooperativeMove();
            break;
        case MenuList::PartyCheck:
            RenderPartyCheck();
            break;
        case MenuList::ToolCheck:
            RenderToolCheck();
            break;

        case MenuList::None:
            RenderNone();
            break;


        }//switch (m_menuList)
    }//if (m_isMenuActive)
    if (m_image == nullptr)
    {
        printfDx(L"m_image is nullptr!");
        return;
    }
    m_image->DrawM(Mposition, size);
    m_image->DrawN(Nposition, size);


}

void FieldScene::Finalize()
{

}



void FieldScene::UpdateCooperativeMove()
{
}

void FieldScene::UpdatePartyCheck()
{
}

void FieldScene::UpdateToolCheck()
{
}


void FieldScene::RenderNone()//メニュー選択初期画面
{
    float positionx = 60.0f;
    float positiony = 65.0f;
    float sizex = 100.0f;
    float sizey = 50.0f;

    float cursorY = positiony + 50.0f * m_menuListSelect;

    DrawBox(positionx, cursorY,positionx + sizex, cursorY + sizey,GetColor(0, 255, 255), FALSE);
    SetFontSize(30);
        DrawString(60, 70 , L"　技　", GetColor(0, 0, 0), TRUE);
        DrawString(60, 120, L"仲　間", GetColor(0, 0, 0), TRUE);
        DrawString(60, 170, L"道　具", GetColor(0, 0, 0), TRUE);
    SetFontSize(10);

}
void FieldScene::RenderCooperativeMove()
{
    DrawBox(200, 50, 1230, 680, GetColor(0, 255, 255), TRUE);
    DrawString(500, 500, L"連携技選択オープン", GetColor(0, 0, 0), TRUE);
}

void FieldScene::RenderPartyCheck()
{
    DrawBox(200, 50, 1230, 680, GetColor(255, 0, 255), TRUE);
    DrawString(500, 500, L"味方一覧オープン", GetColor(0, 0, 0), TRUE);
}

void FieldScene::RenderToolCheck()
{
    DrawBox(200, 50, 1230, 680, GetColor(255, 255, 0), TRUE);
    DrawString(500, 500, L"道具一覧オープン", GetColor(0, 0, 0), TRUE);
}







void FieldScene::SetImage(ImageManager* image)
{
    m_image = image;
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