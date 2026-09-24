#include "pch.h"
#include "Game/Enemy/Dragon.h"

#include"Game/Enemy/EnemyManager.h"
#include"Game/Map/Map.h"
#include"Game/Maths/Collisionall.h"
Dragon::Dragon()
{
}
Dragon::~Dragon()
{

}

void Dragon::Initialize(Map& map, Party& party, int x, int y,bool isBoss)
{
	type = EnemyType::Dragon;

	name = L"マグナ";

	position.x = x * map.m_chipSize;
	position.y = y * map.m_chipSize;

	int enemysize = 2;
	if (isBoss)
	{
		enemysize = 6;
	}
	size.x = map.m_chipSize * enemysize;
	size.y = map.m_chipSize * enemysize;


	renderPosition.x = 500;
	renderPosition.y = 100;

	renderSize.x = 200;
	renderSize.y = 200;

	SetEnemyStats(map, party, 8, 50);

}

void Dragon::Update(Map& map)
{
}

void Dragon::Render()
{
	Vector2 drawSize;

	drawSize.x = size.x * m_drawScale/100;
	drawSize.y = size.y * m_drawScale/100;

	m_image->DrawDragon(position, drawSize);
}

void Dragon::Finalize()
{

}

void Dragon::OnHit(PlayerManager& playermanager)
{
}

void Dragon::RenderBattle()
{
	Vector2 drawPosition = Shake(renderPosition);
	m_image->DrawDragon(drawPosition, renderSize);
}