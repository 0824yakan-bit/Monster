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

	position.x = x * map.m_chipSize;
	position.y = y * map.m_chipSize;

	int enemysize = 2;
	if (isBoss)
	{
		enemysize = 10;
	}
	size.x = map.m_chipSize * enemysize;
	size.y = map.m_chipSize * enemysize;


	renderPosition.x = 100;
	renderPosition.y = 100;

	renderSize.x = 500;
	renderSize.y = 500;

	SetEnemyStats(map,party, 10, 30);
}

void Daemon::Update(Map& map)
{
}

void Daemon::Render()
{
	//DrawBox(position.x, position.y, position.x + size.x, position.y + size.y, GetColor(0, 255, 0), TRUE);
	m_image->DrawDaemon(position, size);
}

void Daemon::Finalize()
{

}

void Daemon::OnHit(PlayerManager& playermanager)
{
}

void Daemon::RenderBattle()
{
	m_image->DrawDaemon(renderPosition, renderSize);
}