#pragma once
class InputManager;
class PlayerManager;
class Enemy;
class EnemyManager;
class Map;
class Battle;
class FieldScene
{
private:
	bool m_isBattleRequested;

	Enemy* m_hitEnemy;
public:
	FieldScene();
	~FieldScene();

	void Initialize(InputManager& inputmanager, Map& map);
	void Update(InputManager& inputManager, PlayerManager& playerManager, EnemyManager& enemyManager,Map&map,Battle&battle);
	void Render(PlayerManager& playerManager, EnemyManager& enemyManager,Map&map);
	void Finalize();

	bool IsBattleRequested() const;

	Enemy* GetHitEnemy() const;

	void ResetBattleRequest();
};

