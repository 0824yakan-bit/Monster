#pragma once

#include"Enemy.h"

class Map;
class Dragon : public Enemy
{
public:
	Dragon();
	~Dragon();

	void Initialize(Map& map, int x, int y)override;
	void Update()override;
	void Render()override;
	void Finalize()override;

	virtual void OnHit(PlayerManager& playermanager)override;

	void RenderBattle()override;
};

