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

	position.x = x;
	position.y = y;

	int enemysize = 2;
	if (isBoss)
	{
		enemysize = 6;
	}
	size.x =enemysize;
	size.y =enemysize;


	renderPosition.x = 500;
	renderPosition.y = 100;

	renderSize.x = 200;
	renderSize.y = 200;

	SetEnemyStats(map, party, 8, 50);

}

void Dragon::Update(Map& map)
{
}

void Dragon::Render(Map& map)
{
	Vector2 drawSize;

	drawSize.x = size.x * m_drawScale/100;
	drawSize.y = size.y * m_drawScale/100;

	m_image->DrawDragon(position * map.m_chipSize, drawSize * map.m_chipSize);
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