#include "pch.h"
#include "Game/Enemy/Fairy.h"

#include"Game/Enemy/EnemyManager.h"
#include"Game/Map/Map.h"
#include"Game/Maths/Collisionall.h"
Fairy::Fairy()
{
}
Fairy::~Fairy()
{

}

void Fairy::Initialize(Map& map, int x, int y)
{
	type = EnemyType::Fairy;

	name = L"フェアリー";

	position.x = x * map.m_chipSize;
	position.y = y * map.m_chipSize;

	size.x = map.m_chipSize * 2;
	size.y = map.m_chipSize * 2;

	power = 10;
	hp = 10;
}

void Fairy::Update()
{

	//moveTimer++;
	//if (moveTimer == 30)
	//{
	//	moveTimer = 0;
	//	position.x += m_size;
	//
	//}
}

void Fairy::Render()
{
	DrawBox(position.x, position.y, position.x + size.x, position.y + size.y, GetColor(0, 255, 0), TRUE);
}

void Fairy::Finalize()
{

}

void Fairy::OnHit(PlayerManager& playermanager)
{
	printfDx(L"FairyHit!!");
}

void Fairy::RenderBattle()
{
	DrawBox(500, 150, 650, 300, GetColor(0, 0, 0), TRUE);
}