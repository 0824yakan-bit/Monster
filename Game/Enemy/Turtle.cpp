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

	SetEnemyStats(map, party, 4, 20);

}

void Turtle::Update(Map& map)
{

}

void Turtle::Render(Map& map)
{
	Vector2 drawSize;

	drawSize.x = size.x * m_drawScale/100;
	drawSize.y = size.y * m_drawScale/100;

	m_image->DrawTurtle(position * map.m_chipSize, drawSize * map.m_chipSize);
}

void Turtle::Finalize()
{

}

void Turtle::OnHit(PlayerManager& playermanager)
{
}

void Turtle::RenderBattle()
{
	Vector2 drawPosition = Shake(renderPosition);
	m_image->DrawTurtle(drawPosition, renderSize);
}