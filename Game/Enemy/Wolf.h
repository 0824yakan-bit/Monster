#pragma once

#include"Enemy.h"
class Wolf:public Enemy
{
public:
	Wolf();
	~Wolf();

	void Initialize(Map& map, Party& party, int x,int y, bool isBoss)override;
	void Update(Map& map)override;
	void Render()override;
	void Finalize()override;

	virtual void OnHit(PlayerManager& player)override;

	void RenderBattle()override;
};

