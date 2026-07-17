#include "pch.h"
#include "Game/Enemy/Wolf.h"

#include"Game/Map/Map.h"

Wolf::Wolf()
{

}

Wolf::~Wolf()
{

}
void Wolf::Initialize(Map& map, int x, int y)
{
	type = EnemyType::Wolf;

	name = L"ウルフ";

	position.x = x*map.m_chipSize;
	position.y = y*map.m_chipSize;

	size.x = map.m_chipSize;
	size.y = map.m_chipSize;

	hp = 10;
}

void Wolf::Update()
{

	//moveTimer++;
	//if (moveTimer == 30)
	//{
	//	moveTimer = 0;
	//	position.x += m_size;
	//
	//}
}

void Wolf::Render()
{
	DrawBox(position.x, position.y, position.x + size.x, position.y + size.y, GetColor(255, 0, 0), TRUE);
}

void Wolf::Finalize()
{

}

void Wolf::OnHit(PlayerManager&playermanager)
{

	printfDx(L"WolfHit!!");
}

void Wolf::RenderBattle()
{
	DrawBox(500, 150, 650, 300,GetColor(255, 255, 0), TRUE);
}