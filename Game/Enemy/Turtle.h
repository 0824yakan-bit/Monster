#pragma once

#include"Enemy.h"

class Map;
class Turtle : public Enemy
{
public:
	Turtle();
	~Turtle();

	void Initialize(Map& map, Party& party, int x, int y, bool isBoss)override;
	void Update(Map& map)override;
	void Render()override;
	void Finalize()override;

	virtual void OnHit(PlayerManager& playermanager)override;

	void RenderBattle()override;
};
