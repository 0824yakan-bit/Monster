#pragma once

#include "Game/Player/PlayerManager.h"
#include "Game/Scene/GameOver.h"
#include "Game/Party/Party.h"
#include "Game/Enemy/Enemy.h"

#include<map>
#include <deque>

class SEManager;
class Map;
class FieldScene;
class SceneManager;
class GameOver;
class PlayerMove;
class Party;
class Enemy;
class BossManager;

class Battle
{
public:

	// 攻撃履歴
	struct UsedAttackInfo
	{
		Monster::CharacteRistics element;
		std::wstring attackName;
	};

	std::vector<UsedAttackInfo> m_usedAttackOrder;
	std::map<Enemy*, int> m_comboDamageTotal;

private:
	// 定数
	static constexpr int MAX_PARTY = 4;
	static constexpr int COMMAND_NUM = 4;
	static constexpr int ATTACK_EFFECT_DURATION = 150;

	// 列挙型
	// 戦闘中の状態
	enum class BattleState
	{
		Command,
		AttackSelect,	// 技選択
		AttackAction,	// 攻撃演出
		Tool,
		Party,
		Run,
		EnemyTurn,
		EnemyDead,
		Annihilation
	};

	// 使用した属性を記録
	enum AttackActive
	{
		USED_NONE = 0,
		USED_NORMAL = 1 << 0,
		USED_FIRE = 1 << 1,
		USED_WATER = 1 << 2,
		USED_GRASS = 1 << 3,
		USED_SOIL = 1 << 4,
		USED_DARKNESS = 1 << 5,
		USED_WIND = 1 << 6
	};

	// ポインタ
	ImageManager* m_image = nullptr;
	SEManager* m_sound = nullptr;
	PlayerManager* m_player = nullptr;
	Party* m_party = nullptr;
	FieldScene* m_fieldScene=nullptr;

	// 敵関連
	BossManager& m_bossManager;
	std::vector<Enemy*> m_enemies;

	int m_targetEnemyIndex			= -1;
	int m_selectedTargetEnemyIndex	= -1;
	int m_enemyAttackType			=  0;
	int m_enemyTargetIndex			=  0;

	Enemy* m_deadEnemy = nullptr;
	std::wstring m_deadEnemyName;

	// 描画関連
	Vector2 drawBgPosition;
	Vector2 drawBgSize;
	Vector2 drawCommandBoxPosition1;
	Vector2 drawCommandBoxSize1;	
	Vector2 drawCommandBoxPosition1_1;
	Vector2 drawCommandBoxSize1_1;
	Vector2 drawCommandBoxPosition2;
	Vector2 drawCommandBoxSize2;
	Vector2 drawCommandBoxPosition2_1;
	Vector2 drawCommandBoxSize2_1;
	Vector2 drawCommandBoxPosition2_2;
	Vector2 drawCommandBoxSize2_2;
	Vector2 drawCommandBoxPosition2_3;
	Vector2 drawCommandBoxSize2_3;
	Vector2 drawEffectPosition;
	Vector2 drawEffectSize;

	bool m_playAttackEffect = false;
	int m_attackEffectTimer = 0;
	Monster::CharacteRistics m_effectElement =Monster::CharacteRistics::None;

	// パーティ関連
	enum class JoinState
	{
		None,
		Confirm,
		Replace
	};
	JoinState m_joinState;

	int m_replaceSelect;

	Enemy* m_joinEnemy;
	int m_monsterhp[MAX_PARTY];
	bool m_requestDefense[MAX_PARTY];

	int m_monsterSelect = 0;
	int m_attackSelect = 0;

	std::vector<int> m_selectedAttack;

	// 戦闘進行関連
	BattleState m_state = BattleState::Command;

	int m_displayIndex = 0;
	int m_select = 0;

	unsigned int state = USED_NONE;

	Monster::CharacteRistics m_characteRistics;

	bool m_isRunSuccess		= false;
	bool m_comboPending		= false;
	bool m_annihilation		= false;

	bool m_IsActive			= false;
	bool m_Window			= false;
	bool m_isJoinWindow		= false;
	bool m_isFieldRequested = false;
	bool m_isEnemyRequested = false;
	bool m_isRun			= false;

	// タイマー・ウィンドウ関連
	int m_receponsTimer = 0;
	int m_displaytextTimer = 0;

	int m_windowWidthFront = 0;
	int m_windowWidth = 0;
	int m_windowHeight = 0;

	// 敵死亡演出関連
	bool m_enemyDeadMotion = false;
	int m_enemyDeadOffsetY = 0;

	// メッセージ
	enum class DisplayMessageType
	{
		Normal,
		Damage,
		Combo
	};

	struct BattleDisplayMessage
	{
		DisplayMessageType type;
		std::wstring text;
	};
	std::deque<BattleDisplayMessage> m_displayQueue;

	// 現在表示しているメッセージ
	BattleDisplayMessage m_currentDisplayMessage;

	// 現在の表示時間
	int m_currentDisplayTimer = 0;

	// 表示中かどうか
	bool m_isDisplayingMessage = false;
	std::wstring m_displayMessage;
	std::wstring m_displayMessageCombo;
	std::wstring m_displayMessageDamage;
	std::vector<std::wstring>m_displayMessageEnemyAttackDamage;


	// コマンド
	const wchar_t* m_command[COMMAND_NUM]
	{
		L"指示する",
		L"所持品",
		L"仲間を見る",
		L"逃げ出す"
	};

	Vector2 drawCommandCursorPosition;
	Vector2 drawCommandCursorSize;

public:


	// 基本処理
	Battle(BossManager&bossManager);												// コンストラクタ
	~Battle();												// デストラクタ

	void Initialize(SceneManager* sceneManager);			// 戦闘の初期化
	void Update(InputManager& inputManager,SceneManager* sceneManager,GameOver& gameOver,Map& map,PlayerManager& player);// 戦闘の更新

	void Render(GameOver& gameOver, Map& map);				// 戦闘画面の描画
	void Finalize();										// 戦闘の終了処理

	// コマンド選択
	void RenderCommand();									// コマンド選択画面の描画
	void RenderCurrentCommand();

	// 攻撃選択
	void UpdateAttackSelect(InputManager& inputManager);								// 攻撃選択の更新
	void RenderAttackSelect();								// 攻撃選択画面の描画

	// 攻撃実行
	void UpdateAttackAction(Map& map,PlayerManager& player);// 攻撃処理の更新
	void RenderAttackAction();								// 攻撃演出の描画

	// 道具
	void UpdateTool(InputManager& inputManager);										// 道具選択の更新
	void RenderTool();										// 道具画面の描画

	// パーティ確認
	void UpdateParty(InputManager& inputManager);			// パーティ確認の更新
	void RenderParty();										// パーティ確認画面の描画

	// 逃走
	void UpdateRun();										// 逃走処理の更新
	void RenderRun();										// 逃走画面の描画

	// 敵ターン
	void UpdateEnemyTurn(SceneManager* sceneManager);		// 敵ターンの更新
	void RenderEnemyTurn();									// 敵ターンの描画
	void RenderCurrentHp();

	// 敵死亡
	void UpdateEnemyDead();									// 敵死亡演出の更新
	void RenderEnemyDead();									// 敵死亡演出の描画

	// 全滅
	void UpdateAnnihilation(GameOver& gameOver,InputManager& inputManager);	// 全滅処理の更新
	void RenderAnnihilation(GameOver& gameOver);				// 全滅画面の描画

	// ターン処理
	void EndTurn();											// ターン終了処理
	void ResetRunSuccess();									//にげる状態リセット

	// 仲間加入確認
	void RequestJoinEnemy(Enemy* enemy);
	void UpdateJoinWindow();
	void RenderJoinWindow();

	// 仲間追加
	void AddJoinEnemy();

	// 仲間入れ替え
	void UpdateJoinReplace(InputManager& inputManager);
	void RenderJoinReplace();
	void ReplaceMonster();

	// 仲間加入状態
	bool IsJoinWindow() const;
	bool IsJoinReplace() const;

	// 敵ターゲット
	void SetTargetEnemyIndex(int index);						// 攻撃対象を設定
	int GetTargetEnemyIndex() const;							// 攻撃対象の番号を取得

	Enemy* GetTargetEnemy();									// 現在の攻撃対象を取得
	Enemy* GetSelectedTargetEnemy();							// 確定した攻撃対象を取得

	int GetValidTargetIndex(int index) const;					// 有効な敵番号を取得
	void SetEnemies(const std::vector<Enemy*>& enemies);		// 戦闘中の敵を設定
	void RemoveEnemy(Enemy* enemy);							// 敵を戦闘リストから削除
	bool AreAllEnemiesDead() const;							// 敵が全滅しているか確認

	void AddDisplayMessage(DisplayMessageType type, const std::wstring& text);
	void UpdateDisplayMessage();
	void ClearDisplayMessageQueue();

	// 各種設定
	void SetFieldScene(FieldScene* fieldScene);				// FieldSceneを設定
	void SetImage(ImageManager* image);						// ImageManagerを設定
	void SetSound(SEManager* sound);						// SEManagerを設定
	void SetPlayer(PlayerManager* player);					// PlayerManagerを設定
	void SetParty(Party* party);							// Partyを設定

	void SetJoinWindow(bool flag);							// 仲間選択画面の表示状態を設定

	// 状態取得
	bool IsFieldRequested();								// フィールド遷移要求を取得
	bool GetAnnihilation();									// 全滅状態を取得
	bool IsEnemyRequested();								// 敵削除要求を取得
	bool IsRunSuccess() const;								// 逃走状態を取得

	// 属性・連携
	void DamageAllEnemies(int damage);//全体ダメージ時関数
	void UesElementalAttack(Map& map,PlayerManager& player);														// 属性連携攻撃を実行
	bool IsComboMember(Monster::CharacteRistics type, bool steamcombo, bool floorcombo, bool waterfallcombo, bool growgrasscombo, bool volcazationcombo);														// 連携攻撃の対象か確認

	// 攻撃履歴
	const std::vector<UsedAttackInfo>&GetUsedAttackOrder() const;								// 攻撃履歴を取得
	void ClearUsedAttackOrder();								// 攻撃履歴をクリア
};