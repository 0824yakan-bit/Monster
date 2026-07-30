#pragma once
#include<vector>
#include"Game/Battle/Battle.h"
class InputManager;
class PlayerManager;
class Enemy;
class EnemyManager;
class Map;
class Battle;
class FieldScene
{
public:
	std::vector<Battle::UsedAttackInfo> m_attackEffects;
	int m_effectIndex = 0;
	int m_effectTimer = 0;
	bool m_playEffect = false;

	void SetAttackEffects(const std::vector<Battle::UsedAttackInfo>& effects);

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

