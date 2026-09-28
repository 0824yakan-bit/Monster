#include "pch.h"
#include "Game/Enemy/Golem.h"

#include"Game/Enemy/EnemyManager.h"
#include"Game/Map/Map.h"
#include"Game/Maths/Collisionall.h"
Golem::Golem()
{
}
Golem::~Golem()
{

}

void Golem::Initialize(Map& map, Party& party, int x, int y, bool isBoss)
{
	type = EnemyType::Golem;

	name = L"ゴーレム";

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

	SetEnemyStats(map, party, 12, 20);

}

void Golem::Update(Map& map)
{

	//moveTimer++;
	//if (moveTimer == 30)
	//{
	//	moveTimer = 0;
	//	position.x += m_size;
	//
	//}
}

void Golem::Render(Map& map)
{
	Vector2 drawSize;

	drawSize.x = size.x * m_drawScale/100;
	drawSize.y = size.y * m_drawScale/100;

	m_image->DrawGolem(position * map.m_chipSize, drawSize * map.m_chipSize);
}

void Golem::Finalize()
{

}

void Golem::OnHit(PlayerManager& playermanager)
{
}

void Golem::RenderBattle()
{
	Vector2 drawPosition = Shake(renderPosition);
	m_image->DrawGolem(drawPosition, renderSize);
}