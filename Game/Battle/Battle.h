#pragma once
#include"Game/Party/Party.h"
#include "Game/Enemy/Enemy.h"

class Party;
class Enbemy;

class Battle
{
public:

private:
	Party* m_party = nullptr;
	Enemy* m_enemy = nullptr;

	enum class BattleState
	{
		Command,
		Attack,
		Tool,
		Scout,
		Run,
	};

	BattleState m_state;

	int m_select;
	int m_receponsTimer;

	bool m_IsActive;
	bool m_Window;
	bool m_isJoinWindow ;

	int m_windowWidthFront;
	int m_windowWidth;
	int m_monsterSelect;
	int m_attackSelect;

	std::vector<int> m_selectedAttack;

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

	void RenderCommand();

	void UpdateAttack();
	void RenderAttack();
	
	void UpdateTool();
	void RenderTool();
	
	void UpdateScout();
	void RenderScout();

	void UpdateRun();
	void RenderRun();


	void SetParty(Party* party);
	void SetEnemy(Enemy* enemy);

	void SetJoinWindow(bool flag);

};

