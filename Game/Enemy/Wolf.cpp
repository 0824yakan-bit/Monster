#include "pch.h"
#include "Game/Enemy/Wolf.h"

#include"Game/Map/Map.h"

Wolf::Wolf()
{

}

Wolf::~Wolf()
{

}
void Wolf::Initialize(Map& map)
{
	type = EnemyType::Wolf;
	position.x = 20 * map.m_chipSize;
	position.y = 20 * map.m_chipSize;

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

