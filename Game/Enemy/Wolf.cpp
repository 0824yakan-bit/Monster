#include "pch.h"
#include "Game/Enemy/Wolf.h"

#include"Game/Map/Map.h"

Wolf::Wolf()
{

}

Wolf::~Wolf()
{

}
void Wolf::Initialize(Map& map, int x, int y)
{
	type = EnemyType::Wolf;

	name = L"ウルフ";

	direction = Direction::Up;
	moveCounter = 0;

	position.x = x*map.m_chipSize;
	position.y = y*map.m_chipSize;

	size.x = map.m_chipSize*2;
	size.y = map.m_chipSize*2;

	renderPosition.x = 500.0f;
	renderPosition.y = 100.0f;

	renderSize.x = 200.0f;
	renderSize.y = 200.0f;

	power = 5;
	hp = 10;
}

void Wolf::Update(Map& map)
{
	moveTimer++;
	if (moveTimer == 90)
	{
		if (moveCounter == -2)
		{
			direction = Direction::Up;
		}
		if (moveCounter == 2)
		{
			direction = Direction::Down;
		}
		switch (direction)
		{
		case Direction::Up:
			moveCounter++;
			moveTimer = 0;
			position.y -= size.y;
			break;
		case Direction::Down:
			moveCounter--;
			moveTimer = 0;
			position.y += size.y;
			break;
		}
	}
}

void Wolf::Render()
{
	//DrawBox(position.x, position.y, position.x + size.x, position.y + size.y, GetColor(255, 0, 0), TRUE);
	m_image->DrawWolf(position, size);
}

void Wolf::Finalize()
{

}

void Wolf::OnHit(PlayerManager&playermanager)
{

	printfDx(L"WolfHit!!");
}

void Wolf::RenderBattle()
{
	//DrawBox(500, 150, 650, 300,GetColor(255, 255, 0), TRUE);
	m_image->DrawWolf(renderPosition, renderSize);
}