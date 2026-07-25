#pragma once
#include"Game/Player/PlayerManager.h"
#include"Game/Party/Party.h"
#include "Game/Enemy/Enemy.h"

class PlayerMove;
class Party;
class Enbemy;

class Battle
{
public:

private:
	static constexpr int MAX_PARTY = 4;
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
	
	int m_monsterhp[MAX_PARTY];//パーティの現在のhp・4体まで
	int m_displayIndex;//現在の味方行動表示
	int m_select;
	int m_receponsTimer;
	int m_displaytextTimer;

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

	void Initialize();
	void Update();
	void Render();
	void Finalize();

	void RenderCommand();//動作選択画面

	void UpdateAttackSelect();//攻撃
	void RenderAttackSelect();
	
	void UpdateAttackAction();//攻撃
	void RenderAttackAction();

	void UpdateTool();//道具
	void RenderTool();
	
	void UpdateScout();//スカウト
	void RenderScout();

	void UpdateRun();//逃げる
	void RenderRun();

	void UpdateEnemyTurn();//敵ターン
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
};

