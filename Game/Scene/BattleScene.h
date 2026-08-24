#pragma once

#include "Game/Battle/Battle.h"
#include "Game/Scene/FieldScene.h"

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

	Vector2 drawBgPosition;
	Vector2 drawBgSize;
	Vector2 drawCommandBoxPosition;
	Vector2 drawCommandBoxSize;


	enum class TeamJoin
	{
		Join,
		Rejection,
	};

	TeamJoin m_teamjoin;

	int m_receponsTimer;

	int m_joinSelect;
	bool m_isJoinRequested;

	ImageManager* m_image = nullptr;
	Battle* m_battle;
	PlayerManager* m_player;
	EnemyManager* m_enemyManager;
	Enemy* m_enemy;
	const wchar_t* m_enemyName;
	SceneManager* m_scenemanager;

	bool m_isReplaceSelect;

	// [0] 左
	// [1] エンカウント敵・中央
	// [2] 右
	std::vector<Enemy*> m_battleEnemies;

	// 仲間にするモンスター
	std::unique_ptr<Monster> m_pendingMonster;

	bool m_isFieldRequested;
	bool m_isTitleRequested;

	int m_runEnemyBonus;
	bool m_battleWin;

public:

	BattleScene(BossManager& bossManager);
	~BattleScene();

	void Initialize(
		InputManager& inputmanager,
		SceneManager& sceneManager,
		Map& map,
		Party& party
	);

	void Update(
		InputManager& inputmanager,
		SceneManager& sceneManager,
		FieldScene& fieldScene,
		GameOver& gameOver,
		EnemyManager& enemyManager,
		Map& map,
		Party& party,
		PlayerManager& player
	);

	void Render(
		GameOver& gameOver,
		Party& party,
		Map& map
	);

	void Finalize();

	bool IsFieldRequested() const;
	bool IsJoinRequested() const;
	bool IsTitleRequested() const;

	void ResetTitleRequest();

	void SetImage(ImageManager* image);
	void SetPlayer(PlayerManager* player);
	void SetEnemyManager(EnemyManager* enemyManager);
	void SetEnemy(Enemy* enemy);

	void SetBattleEnemyPositions();
	void CreateBattleEnemies(Map& map);
	void SetBattleEnemies();

	// 現在の攻撃対象
	Enemy* GetTargetEnemy() const;

	const std::vector<Battle::UsedAttackInfo>& GetUsedAttackOrder() const;
	void ClearUsedAttackOrder();
};