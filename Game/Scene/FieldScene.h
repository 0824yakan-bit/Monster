#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <iterator>

#include "Game/Battle/Battle.h"
#include "Game/Maths/Vector2.h"
#include "Game/Scene/CooperatList.h"


// 前方宣言
class TextManager;
class ImageManager;
class SEManager;
class InputManager;
class PlayerManager;
class Enemy;
class EnemyManager;
class BossManager;
class Map;
class Battle;
class Party;
class Accessory;

// FieldScene
class FieldScene
{
public:
    // 構造体
    struct Text
    {
        bool m_start;
        bool m_signboard_1;
        bool m_signboard_2;
        bool m_signboard_3;
        bool m_signboard_4;
        bool m_boss_1;
        bool m_boss_2;
        bool m_boss_3;
        bool m_lastBossDefeated;
        bool m_lastBossAlive;
        bool m_end;
    };
    Text STtext;
    int m_count = 0;

private:

    struct FieldBreakEffect
    {
        Monster::CharacteRistics element;
        Vector2 position;
        int timer;
    };

    // 列挙型
    enum class MenuList
    {
        CooperativeMove,
        PartyCheck,
        ToolCheck,
        OperationInstructions,
        Empty,
    };

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

    // 定数
    static constexpr int MAX_PARTY = 4;

    // 基本状態

    bool m_isTreasureOpen = false;
    bool m_annihilation = false;

    int m_monsterhp[MAX_PARTY] = {};
    //ボス状態
    bool m_hasShownBoss1Text;
    bool m_hasShownBoss2Text;
    bool m_hasShownBoss3Text;

    //ラストボス状態
    bool m_hasShownLastBossAliveText = false;
    bool m_hasShownLastBossDefeatedText = false;

    // 戦闘関連
    bool m_isBattleRequested = false;
    Enemy* m_hitEnemy = nullptr;
    std::vector<Battle::UsedAttackInfo> m_attackEffects;
    int m_effectIndex = 0;
    int m_effectTimer = 0;
    bool m_playEffect = false;

    // フィールド・破壊エフェクト
    int m_breakLevel = 0;
    std::vector<FieldBreakEffect> m_breakEffects;

    // 仲間加入・交換
    std::unique_ptr<Monster> m_pendingJoinedMonster;
    bool m_isMonsterReplaceSelect = false;
    // 交換する現在のパーティメンバー
    int m_replaceSelect = 0;

    // 各種マネージャー
    ImageManager* m_image = nullptr;
    SEManager* m_sound = nullptr;
    BossManager& m_bossManager;
    Party* m_party = nullptr;

    // 座標・サイズ
    Vector2 Mposition;
    Vector2 Nposition;
    Vector2 size;

    Vector2 drawSlimePosition;
    Vector2 drawSlimeSize;

    Vector2 drawMenuBoxPosition;
    Vector2 drawMenuBoxSize;

    Vector2 drawMenuBoxPosition_1;
    Vector2 drawMenuBoxSize_1;

    Vector2 drawSelectCursorPosition;
    Vector2 drawSelectCursorSize;

    // メニュー
    bool m_isMapActive = false;
    bool m_isMenuActive = false;
    int m_menuListSelect = 0;
    MenuList m_menuList = MenuList::CooperativeMove;
    
    const wchar_t* m_menuText[static_cast<int>(MenuList::Empty)]
    {
        L"技",
        L"仲間",
        L"所持品",
        L"操作指南",
    };

    // 技一覧
    bool m_isCooperatDetailActive = false;
    int m_CooperatDetailSelect = 0;
    int m_skillsPerPage = 6;
    bool m_isSkillLearned = false;
    int m_skillLearnTimer = 0;
    bool m_isAddMonster = false;
    int m_addMonsterTimer = 0;
    std::wstring m_monsterName;
    bool m_isStairOpened = false;
    bool m_hasShownStairOpened = false;
    int m_stairOpenTimer = 0;

    CooperatList m_cooperatList;
    CooperatList m_learnedSkill = CooperatList::None;

    // 取得済みの技属性
    std::vector<CooperatList> m_unlockedSkills;

    // 現在表示する技一覧
    std::vector<CooperatList> m_visibleSkills;

    // 技・エフェクト描画
    Vector2 drawCooperatDetailActivePosition;
    Vector2 drawCooperatDetailActiveSize;

    Vector2 drawEffectPosition;
    Vector2 drawEffectSize;

    // 宝箱
    std::vector<int> result;

public:
    // コンストラクタ・デストラクタ
    FieldScene(BossManager& bossManager, Party& party);
    ~FieldScene();

    // 初期化・更新・描画
    void Initialize(
        SceneManager&sceneManager,
        TextManager& textManager,
        InputManager& inputManager,
        PlayerManager& playerManager,
        Map& map
    );

    void Update(
        TextManager& textManager,
        InputManager& inputManager,
        GameOver& gameOver,
        PlayerManager& playerManager,
        EnemyManager& enemyManager,
        Map& map,
        Battle& battle,
        Accessory& accessory,
        Party& party
    );

    void Render(
        TextManager& textManager,
        GameOver& gameOver,
        PlayerManager& playerManager,
        EnemyManager& enemyManager,
        Map& map,
        Accessory& accessory,
        Party& party
    );

    void Finalize();

    // メニュー更新
    void UpdateCooperativeMove();
    void UpdatePartyCheck();
    void UpdateToolCheck();
    void UpdateOperationInstructions();

    // メニュー描画
    void RenderCooperativeMove(TextManager& textManager);
    void RenderPartyCheck(Party& party);
    void RenderToolCheck();
    void RenderOperationInstructions();

    // レベル関連
    void Level1();
    void Level2();
    void Level3();
    void Level4();
    void Level5(EnemyManager& enemyManager, Map& map);

    // 技関連
    // 技属性を取得
    void LearnCompositeSkill(CooperatList skill);
    // Monsterが持っている技属性を取得
    void LearnMonsterSkills(const Monster& monster);
    // 技名を取得
    const wchar_t* GetSkillName(CooperatList skill);
    // 技を習得できるか確認
    bool TryLearnSkill(CooperatList skill);
    // 技を所持しているか確認
    bool HasSkill(CooperatList skill) const;
    void RefreshPartySkills();
    // ボス関連
    void Boss1Defeat();
    void Boss2Defeat();
    void Boss3Defeat();
    void LastBossDefeat(Battle&battle);

    // 宝箱関連
    void UpdateTreasureOpen(InputManager& inputManager,PlayerManager& playerManager,Map& map,Accessory& accessory);
    void RenderTreasureOpen(Accessory& accessory);

    // エフェクト関連
    void SetAttackEffects(const std::vector<Battle::UsedAttackInfo>& effects);
    void SetBreakEffects(Map& map,Monster::CharacteRistics element);

    // 各種設定
    void SetImage(ImageManager* image);
    void SetSound(SEManager* sound);

    // 戦闘関連
    bool IsBattleRequested() const;
    Enemy* GetHitEnemy() const;
    void ResetBattleRequest();

    // 仲間加入・交換
    void ReceiveJoinedMonster(std::unique_ptr<Monster> monster);

    void UpdateMonsterReplaceSelect(InputManager& inputManager);
    void RenderMonsterReplaceSelect();
};

