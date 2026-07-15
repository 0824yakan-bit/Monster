#include "pch.h"
#include "Game/Player/PlayerMove.h"

#include"Game/Player/PlayerManager.h"
#include"Game/Map/Map.h"
PlayerMove::PlayerMove()
	:m_inputManager {}


	,m_speed		{}
	,m_movetimer	{}
	,m_chipsize		{}
{

}
PlayerMove::~PlayerMove()
{

}



void PlayerMove::Initialize(Map* map,PlayerManager& playermanager)
{
	m_tileRole	  .Initialize(*map);
	m_inputManager.Initialize();

	//初期値＝１マス分移動
	m_speed			= playermanager. GetSpeed   ();
	m_chipsize		= map		   ->GetChipSize();

	m_movetimer = 0;
}

void PlayerMove::Update(Map*map,PlayerManager*playermanager)
{
	m_mapX = static_cast<int>(playermanager->m_position.x) / m_chipsize;
	m_mapY = static_cast<int>(playermanager->m_position.y) / m_chipsize;
	type = map->GetTileType(m_mapX, m_mapY);
	m_tileRole.Update(*map,playermanager->m_position);
	m_inputManager.Update();
	m_movetimer--;
	if (m_movetimer < 0)
	{
		playermanager->m_currentposition = playermanager->m_position;//移動前のポジションを保存

			 if (m_inputManager.IsPress(KEY_INPUT_RIGHT))playermanager->m_position.x += m_speed;
		else if (m_inputManager.IsPress(KEY_INPUT_LEFT ))playermanager->m_position.x -= m_speed;
		else if (m_inputManager.IsPress(KEY_INPUT_UP   ))playermanager->m_position.y -= m_speed;
		else if (m_inputManager.IsPress(KEY_INPUT_DOWN ))playermanager->m_position.y += m_speed;
			 m_nextmapX = static_cast<int>(playermanager->m_position.x) / m_chipsize;
			 m_nextmapY = static_cast<int>(playermanager->m_position.y) / m_chipsize;
			 nexttile = map->GetTileType(m_nextmapX, m_nextmapY);
			 if (nexttile==Map::TileType::Wall)//NextがWallの場合移動できない
			 {
				 playermanager->m_position=playermanager->m_currentposition;
			 }
		m_movetimer = 5;
	}
}

void PlayerMove::Render(Map*map, PlayerManager* playermanager)
{
	DrawBox(playermanager->m_position.x, playermanager->m_position.y, playermanager->m_position.x+32, playermanager->m_position.y+32, GetColor(255, 255, 255), TRUE);
	 

	

	const wchar_t* name = L"";

	switch (type)
	{
	case Map::TileType::Floor:
		name = L"Floor";
		break;

	case Map::TileType::Wall:
		name = L"Wall";
		break;

	case Map::TileType::Object:
		name = L"Object";
		break;
	}
	DrawFormatString(10, 50, GetColor(255, 255, 255), L"Map(%d,%d) : %s", m_mapX, m_mapY, name);//現在のタイルの種類
	int tileNo = map->GetTileNo(m_mapX, m_mapY);

	DrawFormatString(10, 30, GetColor(255, 255, 0), L"TileNo : %d", tileNo);//チップナンバー

}

void PlayerMove::Finalize()
{

}