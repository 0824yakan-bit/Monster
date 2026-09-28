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

void Mole::Render(Map& map)
{
	Vector2 drawSize;

	drawSize.x = size.x * m_drawScale/100;
	drawSize.y = size.y * m_drawScale/100;

	m_image->DrawMole(position * map.m_chipSize, drawSize * map.m_chipSize);
}

void Mole::Finalize()
{

}

void Mole::OnHit(PlayerManager& playermanager)
{
}

void Mole::RenderBattle()
{
	Vector2 drawPosition = Shake(renderPosition);
	m_image->DrawMole(drawPosition, renderSize);
}