#pragma once

#include"Enemy.h"

class Map;
class Slime: public Enemy
{
private:

public:
	

public:
	Slime();
	~Slime();

	void Initialize(Map&map)override;
	void Update()override;
	void Render()override;
	void Finalize()override;

	virtual void OnHit(PlayerManager& playermanager)override;
};

