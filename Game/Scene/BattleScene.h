#pragma once
#include "Game/Battle/Battle.h"
class InputManager;
class Enemy;
class EnemyManager;
class Map;
class BattleScene
{
private:
	enum class TeamJoin
	{
		Join,//加入
		Rejection,//拒否
	};
	TeamJoin m_teamjoin;

	int m_receponsTimer;

	int m_joinSelect;
	bool m_isJoinRequested;

	Battle* m_battle;
	Enemy* m_enemy;

	bool m_isReplaceSelect;
	std::unique_ptr<Monster> m_pendingMonster;

	bool m_isFieldRequested;
	bool m_battleWin;
public:
	BattleScene();
	~BattleScene();

	void Initialize(InputManager& inputmanager,Map&map,Party&party);
	void Update(InputManager& inputmanager,EnemyManager&enemyManager,Party&party);
	void Render(Party&party);
	void Finalize();

	bool IsFieldRequested()const;
	bool IsJoinRequested()const;

	void SetEnemy(Enemy* enemy);
};


