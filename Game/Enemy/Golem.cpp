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

void Golem::Render()
{
	Vector2 drawSize;

	drawSize.x = size.x * m_drawScale/100;
	drawSize.y = size.y * m_drawScale/100;

	m_image->DrawGolem(position, drawSize);
}

void Golem::Finalize()
{

}

void Golem::OnHit(PlayerManager& playermanager)
{
}

void Golem::RenderBattle()
{
	//DrawBox(500, 150, 650, 300, GetColor(0, 0, 0), TRUE);
	m_image->DrawGolem(renderPosition, renderSize);
}