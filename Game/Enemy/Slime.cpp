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

void Slime::Initialize(Map& map, Party& party, int x, int y, bool isBoss)
{

	type = EnemyType::Slime;

	name = L"スライム";

	direction = Direction::Right;
	moveCounter = 0;

	position.x = x * map.m_chipSize;
	position.y = y * map.m_chipSize;

	int enemysize = 2;
	if (isBoss)
	{
		enemysize = 5;
	}
	size.x = map.m_chipSize * enemysize;
	size.y = map.m_chipSize * enemysize;

	size.x = map.m_chipSize;
	size.y = map.m_chipSize;

	renderPosition.x = 500;
	renderPosition.y = 100;

	renderSize.x = 200;
	renderSize.y = 200;

	SetEnemyStats(map, party, 2, 5);

}

void Slime::Update(Map&map)
{
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
}

void Slime::RenderBattle()
{
	m_image->DrawSlime(renderPosition, renderSize);
}