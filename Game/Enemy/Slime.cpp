#include "pch.h"
#include "Game/Enemy/Slime.h"

#include"Game/Enemy/EnemyManager.h"
#include"Game/Map/Map.h"
#include"Game/Maths/Collisionall.h"
Slime::Slime()
{
}
Slime::~Slime()
{

}

void Slime::Initialize(Map& map, int x, int y)
{

	type = EnemyType::Slime;

	name = L"スライム";

	direction = Direction::Right;
	moveCounter = 0;

	position.x = x*map.m_chipSize;
	position.y = y*map.m_chipSize;

	size.x = map.m_chipSize;
	size.y = map.m_chipSize;

	renderPosition.x = 500.0f;
	renderPosition.y = 100.0f;

	renderSize.x = 200.0f;
	renderSize.y = 200.0f;

	power = 2;
	hp = 10;
}

void Slime::Update(Map&map)
{
	moveTimer++;
	level=map.GetBreakLevel();
	switch (level)
	{
	case 0:
		if (moveTimer == 120)
		{
			if (moveCounter == 0)
			{
				direction = Direction::Right;
			}
			if (moveCounter == 5)
			{
				direction = Direction::Left;
			}
			switch (direction)
			{
			case Direction::Right:
				moveCounter++;
				moveTimer = 0;
				position.x += size.x;
				break;
			case Direction::Left:
				moveCounter--;
				moveTimer = 0;
				position.x -= size.x;
				break;
			}
			break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
		}
	}
}

void Slime::Render()
{
   // DrawBox(position.x,position.y,position.x + size.x,position.y + size.y,GetColor(0, 0, 0),TRUE);
	m_image->DrawSlime(position, size);
}

void Slime::Finalize()
{

}

void Slime::OnHit(PlayerManager&playermanager)
{
	printfDx(L"SlimeHit!!");
}

void Slime::RenderBattle()
{
	m_image->DrawSlime(renderPosition, renderSize);
}