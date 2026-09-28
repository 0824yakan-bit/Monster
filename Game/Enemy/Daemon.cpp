#include "pch.h"
#include "Game/Enemy/Daemon.h"

#include"Game/Enemy/EnemyManager.h"
#include"Game/Map/Map.h"
#include"Game/Maths/Collisionall.h"
Daemon::Daemon()
{
}
Daemon::~Daemon()
{

}

void Daemon::Initialize(Map& map,Party&party, int x, int y,bool isBoss)
{
	type = EnemyType::Daemon;

	name = L"ディアボロ";

	position.x = x;
	position.y = y;

	int enemysize = 2;
	if (isBoss)
	{
		enemysize = 10;
	}
	size.x = enemysize;
	size.y = enemysize;


	renderPosition.x = 100;
	renderPosition.y = 100;

	renderSize.x = 500;
	renderSize.y = 500;

	SetEnemyStats(map,party, 10, 60);
}

void Daemon::Update(Map& map)
{
}

void Daemon::Render(Map&map)
{
	Vector2 drawSize;

	drawSize.x = size.x * m_drawScale/100;
	drawSize.y = size.y * m_drawScale/100;

	m_image->DrawDaemon(position * map.m_chipSize, drawSize * map.m_chipSize);
}

void Daemon::Finalize()
{

}

void Daemon::OnHit(PlayerManager& playermanager)
{
}

void Daemon::RenderBattle()
{
	Vector2 drawPosition = Shake(renderPosition);
	m_image->DrawDaemon(drawPosition, renderSize);
}