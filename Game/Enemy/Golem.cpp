#include "pch.h"
#include "Game/Enemy/Golem.h"

#include"Game/Enemy/EnemyManager.h"
#include"Game/Map/Map.h"
#include"Game/Maths/Collisionall.h"
Golem::Golem()
{
}
Golem::~Golem()
{

}

void Golem::Initialize(Map& map, int x, int y)
{
	type = EnemyType::Golem;

	name = L"ゴーレム";

	position.x = x * map.m_chipSize;
	position.y = y * map.m_chipSize;

	size.x = map.m_chipSize * 2;
	size.y = map.m_chipSize * 2;

	power = 10;
	hp = 10;
}

void Golem::Update()
{

	//moveTimer++;
	//if (moveTimer == 30)
	//{
	//	moveTimer = 0;
	//	position.x += m_size;
	//
	//}
}

void Golem::Render()
{
	DrawBox(position.x, position.y, position.x + size.x, position.y + size.y, GetColor(0, 255, 0), TRUE);
}

void Golem::Finalize()
{

}

void Golem::OnHit(PlayerManager& playermanager)
{
	printfDx(L"GolemHit!!");
}

void Golem::RenderBattle()
{
	DrawBox(500, 150, 650, 300, GetColor(0, 0, 0), TRUE);
}