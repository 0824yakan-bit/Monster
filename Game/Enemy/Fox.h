#pragma once

#include"Enemy.h"

class Map;
class Fox : public Enemy
{
public:
	Fox();
	~Fox();

	void Initialize(Map& map, int x, int y, bool isBoss)override;
	void Update(Map& map)override;
	void Render()override;
	void Finalize()override;

	virtual void OnHit(PlayerManager& playermanager)override;

	void RenderBattle()override;
};
