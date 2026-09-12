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

	SetEnemyStats(map, party, 2, 20);

}

void Fairy::Update(Map& map)
{
}

void Fairy::Render()
{
	Vector2 drawSize;

	drawSize.x = size.x * m_drawScale/100;
	drawSize.y = size.y * m_drawScale/100;

	m_image->DrawFairy(position, drawSize);
}

void Fairy::Finalize()
{

}

void Fairy::OnHit(PlayerManager& playermanager)
{
}

void Fairy::RenderBattle()
{
	m_image->DrawFairy(renderPosition, renderSize);
}