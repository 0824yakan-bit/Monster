#include "pch.h"
#include "Game/Player/PlayerMove.h"

#include"Game/Player/PlayerManager.h"
#include"Game/Map/Map.h"
#include"Game/Battle/Battle.h"
PlayerMove::PlayerMove()
	:m_inputManager {}
	

	,m_speed		{}
	,m_movetimer	{}
	,m_chipsize		{}
	,m_mapX			{}
	,m_mapY			{}
	,type			{}
	,nexttile		{}
	,m_nextmapX		{}
	,m_nextmapY		{}
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
		playermanager->m_oldposition=playermanager->m_position ;

		if (m_inputManager.IsPress(KEY_INPUT_RIGHT)/*|| m_inputManager.IsPress(PAD_INPUT_RIGHT)*/)
		{
			playermanager->m_position.x += m_speed;
			playermanager->m_direction = playermanager->Direction::Right;
		}
		else if (m_inputManager.IsPress(KEY_INPUT_LEFT)/*|| m_inputManager.IsPress(PAD_INPUT_LEFT )*/)
		{
			playermanager->m_position.x -= m_speed;
			playermanager->m_direction = playermanager->Direction::Left;

		}
		else if (m_inputManager.IsPress(KEY_INPUT_UP)/*|| m_inputManager.IsPress(PAD_INPUT_UP   )*/)
		{
			playermanager->m_position.y -= m_speed;
			playermanager->m_direction = playermanager->Direction::Up;

		}
		else if (m_inputManager.IsPress(KEY_INPUT_DOWN)/*|| m_inputManager.IsPress(PAD_INPUT_DOWN )*/)
		{
			playermanager->m_position.y += m_speed;
			playermanager->m_direction = playermanager->Direction::Down;

		}
		m_nextmapX = static_cast<int>(playermanager->m_position.x) / m_chipsize;
			 m_nextmapY = static_cast<int>(playermanager->m_position.y) / m_chipsize;
			 
			 nexttile = map->GetTileType(m_nextmapX, m_nextmapY);
				 // プレイヤーサイズ(2マス = 64x64)
				 int playerW = playermanager->m_size.x;
				 int playerH = playermanager->m_size.y;

				 // 当たり判定の四隅
				 int left = static_cast<int>(playermanager->m_position.x) / m_chipsize;
				 int top = static_cast<int>(playermanager->m_position.y) / m_chipsize;
				 int right = static_cast<int>(playermanager->m_position.x + playerW - 1) / m_chipsize;
				 int bottom = static_cast<int>(playermanager->m_position.y + playerH - 1) / m_chipsize;

				 // 4マスのどれかが壁なら戻す
				 bool hitWall =
					 map->GetTileType(left, top) == Map::TileType::Wall ||
					 map->GetTileType(right, top) == Map::TileType::Wall ||
					 map->GetTileType(left, bottom) == Map::TileType::Wall ||
					 map->GetTileType(right, bottom) == Map::TileType::Wall;

				 if (hitWall)
				 {
					 playermanager->m_position = playermanager->m_oldposition;
				 }
		m_movetimer = 5;
	}
}

void PlayerMove::Render(Map*map, PlayerManager* playermanager)
{
	//DrawBox(playermanager->m_position.x, playermanager->m_position.y, playermanager->m_position.x+32, playermanager->m_position.y+32, GetColor(255, 255, 255), TRUE);
 

	

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