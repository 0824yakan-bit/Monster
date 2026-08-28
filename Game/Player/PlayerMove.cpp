#include "pch.h"
#include "Game/Player/PlayerMove.h"

#include"Game/Scene/FieldScene.h"
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
	,m_nextmapX		{}
	,m_nextmapY		{}
	,m_type			{}
	,m_nexttile		{}
	,m_hitTreasure	{}
	,m_hitNextFloor	{}
	,m_hitFall		{}
{

}
PlayerMove::~PlayerMove()
{

}



void PlayerMove::Initialize(Map* map,PlayerManager& playermanager)
{
	m_inputManager.Initialize();

	//初期値＝１マス分移動
	m_speed			= playermanager. GetSpeed   ();
	m_chipsize		= map		   ->GetChipSize();

	m_movetimer = 0;

	m_hitTreasure = false;
}

void PlayerMove::Update(FieldScene* field,Map*map,PlayerManager*playermanager,Accessory*accessory)
{

	m_mapX = static_cast<int>(playermanager->m_position.x) / m_chipsize;
	m_mapY = static_cast<int>(playermanager->m_position.y) / m_chipsize;
	m_type = map->GetTileType(m_mapX, m_mapY);
	m_inputManager.Update();
	m_movetimer--;

	m_hitTreasure = map->IsTreasureRect(
		static_cast<int>(playermanager->m_position.x),
		static_cast<int>(playermanager->m_position.y),
		playermanager->m_size.x,
		playermanager->m_size.y);

	if (m_hitTreasure)
	{
		field->UpdateTreasureOpen(m_inputManager, *playermanager,*map,*accessory);
		return;
	}

	m_hitFall = map->IsFallRect(
		static_cast<int>(playermanager->m_position.x),
		static_cast<int>(playermanager->m_position.y),
		playermanager->m_size.x,
		playermanager->m_size.y);

	if (m_hitFall)
	{
		ReductionPlayer();
		return;
	}

	m_hitNextFloor = map->IsNextFloorRect(
		static_cast<int>(playermanager->m_position.x),
		static_cast<int>(playermanager->m_position.y),
		playermanager->m_size.x,
		playermanager->m_size.y);

	if (m_hitNextFloor)
	{
		map->EnterBossArea();
		playermanager->m_position.x = 10 * m_chipsize;
		playermanager->m_position.y = 10 * m_chipsize;
		return;
	}

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
			
			m_nexttile = map->GetTileType(m_nextmapX, m_nextmapY);
				 // プレイヤーサイズ(2マス = 64x64)
				 int playerW = playermanager->m_size.x;
				 int playerH = playermanager->m_size.y;

				 // 当たり判定の四隅
				 int left = static_cast<int>(playermanager->m_position.x) / m_chipsize;
				 int top = static_cast<int>(playermanager->m_position.y) / m_chipsize;
				 int right = static_cast<int>(playermanager->m_position.x + playerW - 1) / m_chipsize;
				 int bottom = static_cast<int>(playermanager->m_position.y + playerH - 1) / m_chipsize;

				 // 4マスのどれかが壁なら戻す
				 bool hitWall = map->IsWallRect(
					 static_cast<int>(playermanager->m_position.x),
					 static_cast<int>(playermanager->m_position.y),
					 playermanager->m_size.x,
					 playermanager->m_size.y);

				 if (hitWall)
				 {
					 playermanager->m_position = playermanager->m_oldposition;
				 }
				
		m_movetimer = 5;
	}
}

void PlayerMove::Render(FieldScene* field, Map*map, PlayerManager* playermanager, Accessory* accessory)
{
	if (m_hitTreasure)
	{
		field->RenderTreasureOpen(*accessory);
	}
}

void PlayerMove::Finalize()
{

}

void PlayerMove::ReductionPlayer()////落ちたときの処理未追加
{
}
