#include "pch.h"
#include "Game/Enemy/Wolf.h"

#include"Game/Map/Map.h"

Wolf::Wolf()
{

}

Wolf::~Wolf()
{

}
void Wolf::Initialize(Map& map, Party& party, int x, int y, bool isBoss)
{
	type = EnemyType::Wolf;

	name = L"ガル";

	direction = Direction::Up;
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

	renderPosition.x = 500;
	renderPosition.y = 100;

	renderSize.x = 200;
	renderSize.y = 200;

	SetEnemyStats(map, party, 4, 10);
	if (map.GetCurrentMap() == 8)
	{
		SetEnemyStats(map, party, 10, 30);

	}
}

void Wolf::Update(Map& map)
{
}

void Wolf::Render()
{
	Vector2 drawSize;

	drawSize.x = size.x * m_drawScale/100;
	drawSize.y = size.y * m_drawScale/100;

	m_image->DrawWolf(position,drawSize);
}

void Wolf::Finalize()
{

}

void Wolf::OnHit(PlayerManager&playermanager)
{

}

void Wolf::RenderBattle()
{
	Vector2 drawPosition = Shake(renderPosition);
	m_image->DrawWolf(drawPosition, renderSize);
}