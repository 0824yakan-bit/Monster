#pragma once

#include"Game/Player/PlayerMove.h"
#include"Game/Maths/Vector2.h"


class PlayerManager
{
private:
	PlayerMove playermove;
	Map* map;
	
	static constexpr int M_SPEED = 32;
public:
	Vector2 m_position;
	Vector2 m_currentposition;
	int m_size;
public:
	PlayerManager();
	~PlayerManager();

	void Initialize(Map*map);
	void Update(Map*map);
	void Render(Map* map);
	void Finalize();

	Vector2 GetPosition();
	int GetSpeed();
};

