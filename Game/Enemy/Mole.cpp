#include "pch.h"
#include "Game/Enemy/Mole.h"

#include"Game/Enemy/EnemyManager.h"
#include"Game/Map/Map.h"
#include"Game/Maths/Collisionall.h"
Mole::Mole()
{
}
Mole::~Mole()
{

}

void Mole::Initialize(Map& map, Party& party, int x, int y, bool isBoss)
{
	type = EnemyType::Mole;

	name = L"モグラード";

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

	SetEnemyStats(map, party,4, 10);

}

void Mole::Update(Map& map)
{

	//moveTimer++;
	//if (moveTimer == 30)
	//{
	//	moveTimer = 0;
	//	position.x += m_size;
	//
	//}
}

void Mole::Render()
{
	Vector2 drawSize;

	drawSize.x = size.x * m_drawScale;
	drawSize.y = size.y * m_drawScale;

	m_image->DrawMole(position, drawSize);
}

void Mole::Finalize()
{

}

void Mole::OnHit(PlayerManager& playermanager)
{
}

void Mole::RenderBattle()
{
	//DrawBox(500, 150, 650, 300, GetColor(0, 0, 0), TRUE);
	m_image->DrawMole(renderPosition, renderSize);
}