#pragma once
#include "Game/Maths/Vector2.h"
#include"Enemy.h"
class Map;
class Slime: public Enemy
{
private:

public:
	int m_moveTimer;

public:
	Slime();
	~Slime();

	void Initialize(Map&map)override;
	void Update()override;
	void Render()override;
	void Finalize()override;
};

