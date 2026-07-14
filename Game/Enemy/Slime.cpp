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

void Slime::Initialize(Map& map)
{
    position.x = 10 * map.m_chipSize;
    position.y = 10 * map.m_chipSize;

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
    DrawBox(
        position.x,position.y,position.x + size.x,position.y + size.y,GetColor(0, 0, 0),TRUE);
}

void Slime::Finalize()
{

}

void Slime::OnHit(PlayerManager&playermanager)
{
	printfDx(L"SlimeHit!!");
}