#pragma once
#include "Game/Battle/Battle.h"
class InputManager;
class SceneManager;
class PlayerManager;
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
	PlayerManager* m_player;
	Enemy* m_enemy;
	SceneManager* m_scenemanager;

	bool m_isReplaceSelect;
	std::unique_ptr<Monster> m_pendingMonster;

	bool m_isFieldRequested;
	bool m_isTitleRequested;

	bool m_battleWin;
public:
	BattleScene();
	~BattleScene();

	void Initialize(InputManager& inputmanager,SceneManager&sceneManager,Map&map,Party&party);
	void Update(InputManager& inputmanager,SceneManager&sceneManager,EnemyManager&enemyManager,Map&map,Party&party);
	void Render(Party&party);
	void Finalize();

	bool IsFieldRequested()const;
	bool IsJoinRequested()const;

	bool IsTitleRequested()const;
	void ResetTitleRequest();

	void SetPlayer(PlayerManager* player);
	void SetEnemy(Enemy* enemy);
};


