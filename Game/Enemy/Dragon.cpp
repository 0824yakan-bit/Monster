#include "pch.h"
#include "Game/Enemy/Dragon.h"

#include"Game/Enemy/EnemyManager.h"
#include"Game/Map/Map.h"
#include"Game/Maths/Collisionall.h"
Dragon::Dragon()
{
}
Dragon::~Dragon()
{

}

void Dragon::Initialize(Map& map, int x, int y)
{
	type = EnemyType::Dragon;

	name = L"ドラゴン";

	position.x = x * map.m_chipSize;
	position.y = y * map.m_chipSize;

	size.x = map.m_chipSize*5;
	size.y = map.m_chipSize*5;


	renderPosition.x = 500.0f;
	renderPosition.y = 100.0f;

	renderSize.x = 200.0f;
	renderSize.y = 200.0f;

	power = 10;
	hp = 10;
}

void Dragon::Update(Map& map)
{

	//moveTimer++;
	//if (moveTimer == 30)
	//{
	//	moveTimer = 0;
	//	position.x += m_size;
	//
	//}
}

void Dragon::Render()
{
	//DrawBox(position.x, position.y, position.x + size.x, position.y + size.y, GetColor(0, 255, 0), TRUE);
	m_image->DrawDragon(position, size);
}

void Dragon::Finalize()
{

}

void Dragon::OnHit(PlayerManager& playermanager)
{
	printfDx(L"DragonHit!!");
}

void Dragon::RenderBattle()
{
	//DrawBox(500, 150, 650, 300, GetColor(0, 0, 0), TRUE);
	m_image->DrawDragon(renderPosition, renderSize);
}