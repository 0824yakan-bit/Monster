#pragma once
#include "Game/Battle/Battle.h"
#include"Game/Scene/FieldScene.h"
class InputManager;
class FieldScene;
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
	const wchar_t* m_enemyName;
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
	void Update(InputManager& inputmanager,SceneManager&sceneManager,FieldScene&fieldScene, GameOver& gameOver,EnemyManager&enemyManager,Map&map,Party&party,PlayerManager&player);
	void Render(GameOver& gameOver,Party&party);
	void Finalize();

	bool IsFieldRequested()const;
	bool IsJoinRequested()const;

	bool IsTitleRequested()const;
	void ResetTitleRequest();

	void SetPlayer(PlayerManager* player);
	void SetEnemy(Enemy* enemy);

	const std::vector<Battle::UsedAttackInfo>& GetUsedAttackOrder() const;
	void ClearUsedAttackOrder();
};


