#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <set>
#include <algorithm>
#include <iterator>

#include "Game/Battle/Battle.h"
#include "Game/Maths/Vector2.h"

class TextManager;
class InputManager;
class PlayerManager;
class Enemy;
class EnemyManager;
class BossManager;
class Map;
class Battle;
class Party;
class Accessory;

class FieldScene
{
public:
    struct Text
    {
        bool m_start;
        bool m_signboard_1;
        bool m_signboard_2;
        bool m_signboard_3;
        bool m_signboard_4;
        bool m_end;
    };
    Text STtext;
    int m_count;
    bool m_isTreasureOpen;

    std::vector<Battle::UsedAttackInfo> m_attackEffects;

    int m_effectIndex = 0;
    int m_effectTimer = 0;
    bool m_playEffect = false;

    void SetAttackEffects(const std::vector<Battle::UsedAttackInfo>& effects);

private:
    static constexpr int MAX_PARTY = 4;
    bool m_annihilation;
    int m_monsterhp[MAX_PARTY];

    bool m_isBattleRequested;
    int m_breakLevel;
    // 仲間加入待ち
    std::unique_ptr<Monster> m_pendingJoinedMonster;

    // 仲間交換選択中
    bool m_isMonsterReplaceSelect = false;

    // 交換する現在のパーティメンバー
    int m_replaceSelect = 0;
    ImageManager* m_image = nullptr;
    BossManager& m_bossManager;
    Party* m_party;
    Enemy* m_hitEnemy;

    Vector2 Mposition;
    Vector2 Nposition;
    Vector2 size;

    Vector2 drawSlimePosition;
    Vector2 drawSlimeSize;

    // メニュー一覧
    bool m_isMapActive;
    bool m_isMenuActive;

    int m_menuListSelect;

    Vector2 drawMenuBoxPosition;
    Vector2 drawMenuBoxSize;

    Vector2 drawMenuBoxPosition_1;
    Vector2 drawMenuBoxSize_1;

    Vector2 drawSelectCursorPosition;
    Vector2 drawSelectCursorSize;

    enum class MenuList
    {
        CooperativeMove,
        PartyCheck,
        ToolCheck,
        OperationInstructions,
        Empty,
    };

    MenuList m_menuList;

    const wchar_t* m_menuText[static_cast<int>(MenuList::Empty)]
    {
        L"技",
        L"仲間",
        L"所持品",
        L"操作指南",
    };

    // 技一覧
    bool m_isCooperatDetailActive;
    int m_CooperatDetailSelect;

public:
    enum class CooperatList
    {
        Empty,
        None,
        Fire,
        Water,
        Grass,
        Soil,
        Wind,
        Darkness,
        SteamExplpsion,
        FloorBreak,
        WaterFlows,
        GrawGrass,
        Volcazation,
    };
    CooperatList m_cooperatList;

    // 取得済みの技属性
    std::set<CooperatList> m_unlockedSkills;
    // 現在表示する技一覧
    std::vector<CooperatList> m_visibleSkills;

    Vector2 drawEffectPosition;
    Vector2 drawEffectSize;

    // 宝箱中身
    enum class TreasureList
    {
        Empty,
        None,
        Fire,
        Water,
        Grass,
        Soil,
        Wind,
        Thunder,
    };

    std::vector<int> result;

public:
    FieldScene(BossManager& bossManager,Party&party);
    ~FieldScene();

    void Initialize(InputManager& inputmanager,PlayerManager& playerManager,Map& map);

    void Update(TextManager& textManager, InputManager& inputManager,GameOver&gameOver,PlayerManager& playerManager,EnemyManager& enemyManager,Map& map,Battle& battle,Accessory& accessory,Party& party);

    void Render(TextManager& textManager,GameOver&gameOver, PlayerManager& playerManager,EnemyManager& enemyManager,Map& map,Accessory& accessory,Party& party);

    void Finalize();

    void UpdateCooperativeMove();
    void UpdatePartyCheck();
    void UpdateToolCheck();
    void UpdateOperationInstructions();

    void RenderCooperativeMove();
    void RenderPartyCheck(Party& party);
    void RenderToolCheck();
    void RenderOperationInstructions();

    void Level1();
    void Level2();
    void Level3();
    void Level4();
    void Level5(EnemyManager& enemyManager,Map& map);

    // 技属性を取得
    void LearnCompositeSkill(CooperatList skill);


    // Monsterが持っている技属性を取得
    void LearnMonsterSkills(const Monster& monster);

    bool HasSkill(CooperatList skill) const;

    void LastBossDefeat();

    void UpdateTreasureOpen(InputManager& inputManager,PlayerManager& playerManager,Map& map,Accessory& accessory);

    void RenderTreasureOpen(Accessory& accessory);

    void SetImage(ImageManager* image);

    bool IsBattleRequested() const;

    Enemy* GetHitEnemy() const;

    void ResetBattleRequest();

    void ReceiveJoinedMonster(std::unique_ptr<Monster> monster);
    void UpdateMonsterReplaceSelect(InputManager& inputManager);
    void RenderMonsterReplaceSelect();
};