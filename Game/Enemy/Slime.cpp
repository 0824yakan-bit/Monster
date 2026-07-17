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

void Slime::Initialize(Map& map, int x, int y)
{
	type = EnemyType::Slime;

	name = L"スライム";

	position.x = x*map.m_chipSize;
	position.y = y*map.m_chipSize;

	size.x = map.m_chipSize;
	size.y = map.m_chipSize;

	hp = 10;
}

void Slime::Update()
{
   
	//moveTimer++;
	//if (moveTimer == 30)
	//{
	//	moveTimer = 0;
	//	position.x += m_size;
	//
	//}
}

void Slime::Render()
{
    DrawBox(position.x,position.y,position.x + size.x,position.y + size.y,GetColor(0, 0, 0),TRUE);
}

void Slime::Finalize()
{

}

void Slime::OnHit(PlayerManager&playermanager)
{
	printfDx(L"SlimeHit!!");
}

void Slime::RenderBattle()
{
	DrawBox(500, 150, 650, 300,GetColor(0, 255, 0), TRUE);
}