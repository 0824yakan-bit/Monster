#include "pch.h"
#include "Game/Enemy/Turtle.h"

#include"Game/Enemy/EnemyManager.h"
#include"Game/Map/Map.h"
#include"Game/Maths/Collisionall.h"
Turtle::Turtle()
{
}
Turtle::~Turtle()
{

}

void Turtle::Initialize(Map& map, Party& party, int x, int y, bool isBoss)
{
	type = EnemyType::Turtle;

	name = L"タート";

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

	SetEnemyStats(map, party, 4, 20);

}

void Turtle::Update(Map& map)
{

}

void Turtle::Render()
{
	//DrawBox(position.x, position.y, position.x + size.x, position.y + size.y, GetColor(0, 255, 0), TRUE);
	m_image->DrawTurtle(position, size);
}

void Turtle::Finalize()
{

}

void Turtle::OnHit(PlayerManager& playermanager)
{
}

void Turtle::RenderBattle()
{
	//DrawBox(500, 150, 650, 300, GetColor(0, 0, 0), TRUE);
	m_image->DrawTurtle(renderPosition, renderSize);
}