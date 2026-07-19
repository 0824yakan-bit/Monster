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

	size.x = map.m_chipSize*2;
	size.y = map.m_chipSize*2;

	hp = 10;
}

void Dragon::Update()
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
	DrawBox(position.x, position.y, position.x + size.x, position.y + size.y, GetColor(0, 255, 0), TRUE);
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
	DrawBox(500, 150, 650, 300, GetColor(0, 0, 0), TRUE);
}