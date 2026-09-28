#include "pch.h"
#include "Game/Enemy/Slime.h"

#include"Game/Enemy/EnemyManager.h"
#include"Game/Map/Map.h"
#include"Game/Maths/Collisionall.h"
Slime::Slime()
{
}
Slime::~Slime()
{

}

void Slime::Initialize(Map& map, Party& party, int x, int y, bool isBoss)
{

	type = EnemyType::Slime;

	name = L"スライム";

	direction = Direction::Right;
	moveCounter = 0;

	position.x = x;
	position.y = y;

	int enemysize = 1;
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

	SetEnemyStats(map, party, 2, 15);

}

void Slime::Update(Map&map)
{
}

void Slime::Render(Map& map)
{
	Vector2 drawSize;

	drawSize.x = size.x * m_drawScale/100;
	drawSize.y = size.y * m_drawScale/100;

	m_image->DrawSlime(position * map.m_chipSize, drawSize * map.m_chipSize);
}

void Slime::Finalize()
{

}

void Slime::OnHit(PlayerManager&playermanager)
{
}

void Slime::RenderBattle()
{
	Vector2 drawPosition = Shake(renderPosition);
	m_image->DrawSlime(drawPosition, renderSize);
}