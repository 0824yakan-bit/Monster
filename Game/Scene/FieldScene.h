#pragma once
class InputManager;
class PlayerManager;
class Enemy;
class EnemyManager;
class Map;
class FieldScene
{
private:
	bool m_isBattleRequested;

	Enemy* m_hitEnemy;
public:
	FieldScene();
	~FieldScene();

	void Initialize(InputManager& inputmanager);
	void Update(InputManager& inputManager, PlayerManager& playerManager, EnemyManager& enemyManager,Map&map);
	void Render(PlayerManager& playerManager, EnemyManager& enemyManager,Map&map);
	void Finalize();

	bool IsBattleRequested() const;

	Enemy* GetHitEnemy() const;
};

