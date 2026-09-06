#include "pch.h"
#include "Game/Enemy/Phoenix.h"

#include"Game/Enemy/EnemyManager.h"
#include"Game/Map/Map.h"
#include"Game/Maths/Collisionall.h"
Phoenix::Phoenix()
{
}
Phoenix::~Phoenix()
{

}

void Phoenix::Initialize(Map& map, Party& party, int x, int y, bool isBoss)
{
	type = EnemyType::Phoenix;

	name = L"スザク";

	position.x = x * map.m_chipSize;
	position.y = y * map.m_chipSize;

	int enemysize = 2;
	if (isBoss)
	{
		enemysize = 7;
	}
	size.x = map.m_chipSize * enemysize;
	size.y = map.m_chipSize * enemysize;

	renderPosition.x = 500;
	renderPosition.y = 100;

	renderSize.x = 200;
	renderSize.y = 200;

	SetEnemyStats(map, party, 2,70);

}

void Phoenix::Update(Map& map)
{

	//moveTimer++;
	//if (moveTimer == 30)
	//{
	//	moveTimer = 0;
	//	position.x += m_size;
	//
	//}
}

void Phoenix::Render()
{
	//DrawBox(position.x, position.y, position.x + size.x, position.y + size.y, GetColor(0, 255, 0), TRUE);
	m_image->DrawPhoenix(position, size);
}

void Phoenix::Finalize()
{

}

void Phoenix::OnHit(PlayerManager& playermanager)
{
}

void Phoenix::RenderBattle()
{
	//DrawBox(500, 150, 650, 300, GetColor(0, 0, 0), TRUE);
	m_image->DrawPhoenix(renderPosition, renderSize);
}