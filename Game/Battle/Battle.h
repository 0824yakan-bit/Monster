#pragma once
#include"Game/Player/PlayerManager.h"
#include"Game/Party/Party.h"
#include "Game/Enemy/Enemy.h"

class Map;
class SceneManager;
class PlayerMove;
class Party;
class Enemy;

class Battle
{
public:
	struct UsedAttackInfo
	{
		Monster::CharacteRistics element; // 属性
		std::wstring attackName;          // 技名
	};

	std::vector<UsedAttackInfo> m_usedAttackOrder;

private:
	static constexpr int MAX_PARTY = 4;
	bool m_requestDefense[MAX_PARTY];
	Monster::CharacteRistics m_characteRistics;
	bool m_comboPending = false;

	PlayerManager* m_player = nullptr;
	Party* m_party = nullptr;
	Enemy* m_enemy = nullptr;

	enum class BattleState
	{
		Command,
		AttackSelect,   // 技選択
		AttackAction,   // 攻撃演出
		Tool,
		Scout,
		Run,
		EnemyTurn,
		EnemyDead,
		Annihilation
	};

	BattleState m_state;
	
	enum AttackActive//属性組み合わせ・バトル終了時まで使った属性は保存
	{
		USED_NONE   = 0 ,
		USED_NORMAL = 1 << 0,//通常　攻撃
		USED_FIRE   = 1 << 1,//　火　属性
		USED_WATER  = 1 << 2,//　水　属性
		USED_GRASS  = 1 << 3,//　草　属性
		USED_SOIL   = 1 << 4,//　土　属性
		USED_THUNDER= 1 << 5,//　雷　属性
		USED_WIND   = 1 << 6,//　風　属性
	};
	unsigned int state = 0;//符号なし（プラスのみ）




	int m_monsterhp[MAX_PARTY];//パーティの現在のhp・4体まで
	int m_displayIndex;//現在の味方行動表示
	int m_select;
	int m_receponsTimer;
	int m_displaytextTimer;
	std::wstring m_deadEnemyName;
	// やられ演出用
	bool m_enemyDeadMotion;
	int  m_enemyDeadOffsetY;

	bool m_IsActive;
	bool m_Window;
	bool m_isFieldRequested;
	bool m_isJoinWindow ;
	bool m_annihilation;
	bool m_isRun;
	bool m_isEnemyRequested;
	

	int m_windowWidthFront;
	int m_windowWidth;
	int m_monsterSelect;
	int m_attackSelect;

	std::vector<int> m_selectedAttack;
	std::wstring m_displayMessage;

	static constexpr int COMMAND_NUM = 4;

	const wchar_t* m_command[COMMAND_NUM]
	{
		L"指揮する",
		L"もちもの",
		L"  さそう",
		L"にげだす",
	};
	
	
public:
	Battle();
	~Battle();

	void Initialize(SceneManager*sceneManager);
	void Update(SceneManager*sceneManager, Map& map, PlayerManager& player);
	void Render();
	void Finalize();

	void RenderCommand();//動作選択画面

	void UpdateAttackSelect();//攻撃
	void RenderAttackSelect();
	
	void UpdateAttackAction(Map& map, PlayerManager& player);//攻撃
	void RenderAttackAction();

	void UpdateTool();//道具
	void RenderTool();
	
	void UpdateScout();//スカウト
	void RenderScout();

	void UpdateRun();//逃げる
	void RenderRun();

	void UpdateEnemyTurn(SceneManager*sceneManagar);//敵ターン
	void RenderEnemyTurn();

	void UpdateEnemyDead();
	void RenderEnemyDead();


	void UpdateAnnihilation();
	void RenderAnnihilation();

	void EndTurn();//ターン終了


	void SetPlayer(PlayerManager* player);
	void SetParty(Party* party);
	void SetEnemy(Enemy* enemy);

	void SetJoinWindow(bool flag);

	bool IsFieldRequested();

	bool GetAnnihilation();

	bool IsEnemyRequested();

	bool IsRun();

	void UesElementalAttack(Map&map,PlayerManager&player);

	const std::vector<UsedAttackInfo>& GetUsedAttackOrder() const;

	void ClearUsedAttackOrder();

	bool IsComboMember(Monster::CharacteRistics type, bool steamcombo, bool floorcombo);
};

