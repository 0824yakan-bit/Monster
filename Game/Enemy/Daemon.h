#pragma once

#include"Enemy.h"

class Map;
class Daemon : public Enemy
{
public:
	Daemon();
	~Daemon();

	void Initialize(Map& map, int x, int y, bool isBoss)override;
	void Update(Map& map)override;
	void Render()override;
	void Finalize()override;

	virtual void OnHit(PlayerManager& playermanager)override;

	void RenderBattle()override;
};
