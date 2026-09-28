#include "pch.h"
#include "Game/Enemy/Fairy.h"

#include"Game/Enemy/EnemyManager.h"
#include"Game/Map/Map.h"
#include"Game/Maths/Collisionall.h"
Fairy::Fairy()
{
}
Fairy::~Fairy()
{

}

void Fairy::Initialize(Map& map, Party& party, int x, int y, bool isBoss)
{
	type = EnemyType::Fairy;

	name = L"リーフ";

	position.x = x;
	position.y = y;

	int enemysize = 2;
	if (isBoss)
	{
		enemysize = 5;
	}
	size.x =enemysize;
	size.y =enemysize;

	renderPosition.x = 500;
	renderPosition.y = 100;

	renderSize.x = 200;
	renderSize.y = 200;

	SetEnemyStats(map, party, 2, 20);

}

void Fairy::Update(Map& map)
{
}

void Fairy::Render(Map& map)
{
	Vector2 drawSize;

	drawSize.x = size.x * m_drawScale/100;
	drawSize.y = size.y * m_drawScale/100;

	m_image->DrawFairy(position * map.m_chipSize, drawSize * map.m_chipSize);
}

void Fairy::Finalize()
{

}

void Fairy::OnHit(PlayerManager& playermanager)
{
}

void Fairy::RenderBattle()
{
	Vector2 drawPosition = Shake(renderPosition);
	m_image->DrawFairy(drawPosition, renderSize);
}