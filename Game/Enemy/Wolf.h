#pragma once

#include"Enemy.h"
class Wolf:public Enemy
{
public:
	Wolf();
	~Wolf();

	void Initialize(Map& map,int x,int y)override;
	void Update()override;
	void Render()override;
	void Finalize()override;

	virtual void OnHit(PlayerManager& player)override;

	void RenderBattle()override;
};

