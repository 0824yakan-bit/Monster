#include "pch.h"
#include "Game/Scene/FieldScene.h"

#include"Game/Maths/Collisionall.h"
#include"Game/Party/Monster.h"
#include"Game/Battle/Battle.h"

static std::vector<Battle::UsedAttackInfo>MakeFieldEffect(Monster::CharacteRistics element, const wchar_t* name)
{
    Battle::UsedAttackInfo info;
    info.element = element;
    info.attackName = name;

    return { info };
}

FieldScene::FieldScene()
    :m_hitEnemy{nullptr}
    ,m_isBattleRequested{false}
    , m_isMapActive{ false }
    ,m_isMenuActive{false}
    ,m_menuList{MenuList::Empty}
    ,m_menuListSelect{0}
    ,m_isCooperatDetailActive{false}
    ,m_CooperatDetailSelect{0}
    ,m_cooperatList{CooperatList::Empty}
    
{
}

FieldScene::~FieldScene()
{
}


void FieldScene::Initialize(InputManager& inputmanager,PlayerManager&playerManager, Map& map)
{
    playerManager.SetImage(m_image);

    // 初回だけ登録
    if (m_unlockedSkills.empty())
    {
        m_unlockedSkills.insert(CooperatList::None);//初期スライム用
        m_unlockedSkills.insert(CooperatList::Water);//初期スライム用
    }

    m_isMapActive = false;
    m_isMenuActive = false;

    m_isCooperatDetailActive = false;

    if (map.GetCurrentMap() == 0 && map.GetStageStartMap() == 0)
    {
        map.ChangeStage(0);
    }
    m_hitEnemy = nullptr;
    m_isBattleRequested = false;


    //画像配置
    Mposition.x = 10.0f;
    Mposition.y = 600.0f;
    
    Nposition.x = 50.0f;
    Nposition.y = 650.0f;

    size.x = 50.0f;
    size.y = 50.0f;
}

void FieldScene::Update(InputManager& inputManager,PlayerManager& playerManager,EnemyManager& enemyManager,Map&map,Battle&battle)
{
    inputManager.Update();
    map.Update(inputManager, playerManager);
    m_breakLevel = map.GetBreakLevel();

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
        m_menuListSelect = 0;//メニュー選択初期化
        m_CooperatDetailSelect = 0;//連携技選択初期化
        m_menuList = MenuList::Empty;
        m_cooperatList = CooperatList::Empty;

        m_isMenuActive = true; //メニュー表示
    }
    else if (inputManager.IsTrigger(KEY_INPUT_N) && m_isMenuActive)
    {
        m_isMenuActive = false;//メニュー非表示
    }

    if (m_isMenuActive)
    {
        if (inputManager.IsTrigger(KEY_INPUT_UP))
        {
            if (m_isCooperatDetailActive)
            {
                m_CooperatDetailSelect--;
            }
            else
            {
                m_menuListSelect--;
            }
        }
        if (inputManager.IsTrigger(KEY_INPUT_DOWN))
        {
            if (m_isCooperatDetailActive)
            {
                m_CooperatDetailSelect++;
            }
            else
            {
                m_menuListSelect++;
            }
        }
        if (inputManager.IsTrigger(KEY_INPUT_RETURN))
        {
            if (m_isCooperatDetailActive)//連携技選択
            {
                if (m_CooperatDetailSelect >= 0 &&
                    m_CooperatDetailSelect < (int)m_visibleSkills.size())
                {
                    m_cooperatList = m_visibleSkills[m_CooperatDetailSelect];
                }
            }
            else//メニュー
            {
                switch (m_menuListSelect)
                {
                case 0:
                    m_menuList = MenuList::CooperativeMove;
                    m_isCooperatDetailActive = true;
                    break;
                case 1:
                    m_menuList = MenuList::PartyCheck;
                    break;
                case 2:
                    m_menuList = MenuList::ToolCheck;
                    break;
                case 3:
                    m_menuList = MenuList::OperationInstructions;
                }//switch (m_menuListSelect)
            }
        }//(inputManager.IsTrigger(KEY_INPUT_ESCAPE))
        else if (inputManager.IsTrigger(KEY_INPUT_BACK))
        {
            m_menuList = MenuList::Empty;
            m_menuListSelect = 0;
            m_isCooperatDetailActive = false;
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
        case MenuList::OperationInstructions:
            UpdateOperationInstructions();

        }//switch (m_menuList)
        switch (m_cooperatList)//メニューから技使用可能
        {
        case CooperatList::None:
            map.NormalBreak(playerManager);
            printfDx(L"UsedNormalBreak");
            SetAttackEffects(MakeFieldEffect(Monster::CharacteRistics::None, L""));

            m_isMenuActive = false;
            m_isCooperatDetailActive = false;
            m_cooperatList = CooperatList::Empty;
            break;

        case CooperatList::Fire:
            map.FireBreak(playerManager);
            SetAttackEffects(MakeFieldEffect(Monster::CharacteRistics::Fire, L""));

            printfDx(L"UsedFireBreak");
            m_isMenuActive = false;
            m_isCooperatDetailActive = false;
            m_cooperatList = CooperatList::Empty;
            break;

        case CooperatList::Water:
            map.WaterBreak(playerManager);
            SetAttackEffects(MakeFieldEffect(Monster::CharacteRistics::Water, L""));

            printfDx(L"UsedWaterBreak");
            m_isMenuActive = false;
            m_isCooperatDetailActive = false;
            m_cooperatList = CooperatList::Empty;
            break;

        case CooperatList::Grass:
            map.GrassBreak(playerManager);
            SetAttackEffects(MakeFieldEffect(Monster::CharacteRistics::Grass, L""));

            printfDx(L"UsedGrassBreak");
            m_isMenuActive = false;
            m_isCooperatDetailActive = false;
            m_cooperatList = CooperatList::Empty;
            break;

        case CooperatList::Soil:
            map.SoilBreak(playerManager);
            SetAttackEffects(MakeFieldEffect(Monster::CharacteRistics::Soil, L""));

            printfDx(L"UsedSoilBreak");
            m_isMenuActive = false;
            m_isCooperatDetailActive = false;
            m_cooperatList = CooperatList::Empty;
            break;

        case CooperatList::Thunder:
            map.ThunderBreak(playerManager);
            SetAttackEffects(MakeFieldEffect(Monster::CharacteRistics::Thunder, L""));

            printfDx(L"UsedThunderBreak");
            m_isMenuActive = false;
            m_isCooperatDetailActive = false;
            m_cooperatList = CooperatList::Empty;
            break;

        case CooperatList::Wind:
            map.WindBreak(playerManager);
            SetAttackEffects(MakeFieldEffect(Monster::CharacteRistics::Wind, L""));

            printfDx(L"UsedWindBreak");
            m_isMenuActive = false;
            m_isCooperatDetailActive = false;
            m_cooperatList = CooperatList::Empty;
            break;


        }
    }// if(m_isMenuActive)

/////プレイヤー管理

    if (!m_isMapActive && !m_isMenuActive)playerManager.Update(&map);


    if (playerManager.m_oldposition != playerManager.m_position)
    {
        playerManager.m_invicible = false;
    }
/////エネミー管理
    enemyManager.Update(map);

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
/////ブレイクレベル管理
        int m_level = m_breakLevel / 10;
        switch (m_level)
        {
        case 0://一段階目//初期
            Level5(enemyManager,map);

            break;
        case 1://二段階目
            Level2();

            break;
        case 2://三段階目
            Level3();

            break;
        case 3://四段階目
            Level4();

            break;
        case 4://五段階目//最大
            Level5(enemyManager,map);

            break;
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

    if (m_isMapActive)
    {
        DrawBox(200, 200, 900, 900, GetColor(0, 0, 0), TRUE);
        DrawString(500, 500, L"マップオープン", GetColor(255, 255, 255), TRUE);
    } 
    if (m_isMenuActive)
    {
        DrawBox(50, 50, 250, 400, GetColor(255, 255, 255), TRUE);
        DrawString(500, 500, L"メニューオープン", GetColor(0, 0, 0), TRUE);

        float positionx = 60.0f;
        float positiony = 65.0f;
        float sizex = 100.0f;
        float sizey = 50.0f;

        float cursorY = positiony + 50.0f * m_menuListSelect;

        DrawBoxAA(positionx, cursorY, positionx + sizex, cursorY + sizey, GetColor(0, 255, 255), FALSE);
        SetFontSize(30);
        DrawString(60, 70, L"技", GetColor(0, 0, 0), TRUE);
        DrawString(60, 120, L"仲間", GetColor(0, 0, 0), TRUE);
        DrawString(60, 170, L"道具", GetColor(0, 0, 0), TRUE);
        DrawString(60, 220, L"操作方法", GetColor(0, 0, 0), TRUE);
        SetFontSize(10);

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
        case MenuList::OperationInstructions:
            RenderOperationInstructions();
            break;

        case MenuList::Empty:
            break;

        }//switch (m_menuList)

    }//if (m_isMenuActive)
    if (m_image == nullptr)
    {
        printfDx(L"m_image is nullptr!");
        return;
    }
    DrawFormatString(10, 200, GetColor(255, 255, 255), L"BreakLevel:%d", m_breakLevel, TRUE);//ブレイクレベル

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

void FieldScene::UpdateOperationInstructions()
{
}

void FieldScene::RenderCooperativeMove()
{
    DrawBox(200, 50, 1230, 680, GetColor(0, 255, 255), TRUE);
    DrawString(500, 500, L"連携技選択オープン", GetColor(0, 0, 0), TRUE);

    float positionx = 250.0f;
    float positiony = 150.0f;
    float sizex = 200.0f;
    float sizey = 50.0f;

    // 表示中の技一覧を作り直す
    m_visibleSkills.clear();

    auto AddSkill = [&](CooperatList skill)
        {
            if (HasSkill(skill))
            {
                m_visibleSkills.push_back(skill);
            }
        };

    AddSkill(CooperatList::None);
    AddSkill(CooperatList::Fire);
    AddSkill(CooperatList::Water);
    AddSkill(CooperatList::Grass);
    AddSkill(CooperatList::Soil);
    AddSkill(CooperatList::Wind);
    AddSkill(CooperatList::Thunder);

    // カーソル補正
    if (m_visibleSkills.empty())
    {
        return;
    }

    if (m_CooperatDetailSelect < 0)
    {
        m_CooperatDetailSelect = (int)m_visibleSkills.size() - 1;
    }

    if (m_CooperatDetailSelect >= (int)m_visibleSkills.size())
    {
        m_CooperatDetailSelect = 0;
    }

    float cursorY = positiony + 50.0f * m_CooperatDetailSelect;

    DrawBoxAA(positionx, cursorY,
        positionx + sizex, cursorY + sizey,
        GetColor(0, 0, 0), FALSE);

    SetFontSize(50);
    DrawString(250, 50, L"技一覧", GetColor(0, 0, 0), TRUE);

    int y = 150;

    for (auto skill : m_visibleSkills)
    {
        const wchar_t* name = L"";

        switch (skill)
        {
        case CooperatList::None:    name = L"無属性"; break;
        case CooperatList::Fire:    name = L"火属性"; break;
        case CooperatList::Water:   name = L"水属性"; break;
        case CooperatList::Grass:   name = L"草属性"; break;
        case CooperatList::Soil:    name = L"土属性"; break;
        case CooperatList::Wind:    name = L"風属性"; break;
        case CooperatList::Thunder: name = L"雷属性"; break;
        default: break;
        }

        DrawString(250, y, name, GetColor(0, 0, 0), TRUE);
        y += 50;
    }

    SetFontSize(10);
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

void FieldScene::RenderOperationInstructions()
{
    DrawBox(200, 50, 1230, 680, GetColor(255,0, 0), TRUE);
    DrawString(500, 500, L"操作説明オープン", GetColor(0, 0, 0), TRUE);
}

void FieldScene::Level1()
{
}

void FieldScene::Level2()
{

}

void FieldScene::Level3()
{
}

void FieldScene::Level4()
{
}

void FieldScene::Level5(EnemyManager&enemyManager, Map& map)
{
    enemyManager.Update(map);
}



void FieldScene::LearnSkill(CooperatList skill)
{
    printfDx(L"FieldScene LearnSkill %d\n", (int)skill);
    m_unlockedSkills.insert(skill);
}

bool FieldScene::HasSkill(CooperatList skill) const
{
    return m_unlockedSkills.count(skill) > 0;
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

