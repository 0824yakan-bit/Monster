#pragma once
class InputManager;
class Enemy;
class EnemyManager;
class Map;
class BattleScene
{
private:
	Enemy* m_enemy;
	bool m_isFieldRequested;
	bool m_battleWin;
public:
	BattleScene();
	~BattleScene();

	void Initialize(InputManager& inputmanager,Map&map);
	void Update(InputManager& inputmanager,EnemyManager&enemyManager);
	void Render();
	void Finalize();

	bool IsFieldRequested()const;

	void SetEnemy(Enemy* enemy);
};


