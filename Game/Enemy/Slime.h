#pragma once

#include"Enemy.h"

class Map;
class Slime: public Enemy
{
public:
	Slime();
	~Slime();

	void Initialize(Map&map, Party& party, int x,int y, bool isBoss)override;
	void Update(Map&map)override;
	void Render(Map& map)override;
	void Finalize()override;

	virtual void OnHit(PlayerManager& playermanager)override;

	void RenderBattle()override;
};

