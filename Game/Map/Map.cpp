#include "pch.h"
#include "Game/Map/Map.h"


#include"Game/InputManager/InputManager.h"
#include"Game/Player/PlayerManager.h"
#include"Game/Screen.h"
#include<fstream>
#include<sstream>
#include<cassert>
Map::Map()
	:m_basemap	 {}
	,m_workmap	 {}
	,m_ghChip	 {}
	,m_chipSize	 {}
	,m_currentMap{}
{
	// グラフィクスハンドルを初期化する
	for (int i = 0; i < GH_MAX; i++)
	{
		m_ghChip[i] = (-1);
	}
}
Map::~Map()
{

}

void Map::Initialize(const wchar_t* fileName)
{
	m_moveDir = MoveDir::None;

	m_isTransition = false;
	m_transition = 0;
	m_nextmap = 0;

	m_chipSize = 32;
	m_currentMap = 0;
	LoadDivGraph(L"Resources/Textures/testmapchip.png",GH_MAX,16,20,m_chipSize,m_chipSize,m_ghChip);	//横１６個、
	for (auto& gh : m_ghChip)
	{
		assert(gh != (-1) && "タイルマップをロードできませんでした");
	}

	LoadMapChip(L"Resources/CSV/map.csv", m_workmap);
	LoadMapChip(L"Resources/CSV/object.csv",m_objectmap);
}
void Map::Update(InputManager&inputManger,PlayerManager&playerManager)
{

	//右端から次のマップへ移動
	if (!m_isTransition && playerManager.m_position.x >= Screen::RIGHT)
	{
		//移動方向を保存
		m_moveDir = MoveDir::Right;

		//次のマップ番号
		m_nextmap = (m_currentMap + 1) % MAP_NUM;

		m_transition = 0;
		m_isTransition = true;
	}
	//左端から
	if (!m_isTransition && playerManager.m_position.x < Screen::LEFT)
	{
		//移動方向を保存
		m_moveDir = MoveDir::Left;

		//前のマップ番号
		m_nextmap = (m_currentMap - 1 + MAP_NUM) % MAP_NUM;

		m_transition = 0;
		m_isTransition = true;
	}
	//上から
	if (!m_isTransition && playerManager.m_position.y < Screen::TOP)
	{
		m_moveDir = MoveDir::Up;

		m_nextmap = (m_currentMap -1 + MAP_NUM) % MAP_NUM;
		
		m_transition = 0;
		m_isTransition = true;
	}
	//下から
	if (!m_isTransition && playerManager.m_position.y >= Screen::BOTTOM)
	{
		m_moveDir = MoveDir::Down;

		m_nextmap = (m_currentMap + 1) % MAP_NUM;
		m_transition = 0;
		m_isTransition = true;
	}
	if (m_isTransition)
	{
		m_transition += 16;

		int limit = (m_moveDir == MoveDir::Up || m_moveDir == MoveDir::Down)
			? Screen::HEIGHT
			: Screen::WIDTH;

		if (m_transition >= limit)
		{
			m_currentMap = m_nextmap;
			m_isTransition = false;

			if (m_moveDir == MoveDir::Right)
				playerManager.m_position.x = 0;
			else if (m_moveDir == MoveDir::Left)
				playerManager.m_position.x = Screen::RIGHT - m_chipSize;
			else if (m_moveDir == MoveDir::Up)
				playerManager.m_position.y = Screen::BOTTOM - m_chipSize;
			else if (m_moveDir == MoveDir::Down)
				playerManager.m_position.y = 0;
			m_moveDir = MoveDir::None;
		}
	}
}
void Map::Render()
{
	if (m_moveDir == MoveDir::Right)
	{
		DrawCurrentMap(-m_transition, 0);
		DrawNextMap(Screen::WIDTH - m_transition, 0);
	}
	else if (m_moveDir == MoveDir::Left)
	{
		DrawCurrentMap(m_transition, 0);
		DrawNextMap(-Screen::WIDTH + m_transition, 0);
	}
	else if (m_moveDir == MoveDir::Up)
	{
		DrawCurrentMap(0, m_transition);
		DrawNextMap(0, -Screen::HEIGHT + m_transition);
	}
	else if (m_moveDir == MoveDir::Down)
	{
		DrawCurrentMap(0, -m_transition);
		DrawNextMap(0, Screen::HEIGHT - m_transition);
	}
	else
	{
		DrawCurrentMap(0, 0);
	}
	DrawFormatString(10, 130, GetColor(255, 255, 255), L"現在マップ%d", m_currentMap);
}
void Map::DrawCurrentMap(int offsetX, int offsetY)
{
	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			int tileNo = m_workmap[m_currentMap][y][x];

			DrawGraph(
				x * m_chipSize + offsetX,
				y * m_chipSize + offsetY,
				m_ghChip[tileNo],
				FALSE);

			int objectNo = m_objectmap[m_currentMap][y][x];

			if (objectNo >= 0)
			{
				DrawGraph(
					x * m_chipSize + offsetX,
					y * m_chipSize + offsetY,
					m_ghChip[objectNo],
					TRUE);
			}
		}
	}
}
void Map::DrawNextMap(int offsetX, int offsetY)
{
	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			int tileNo = m_workmap[m_nextmap][y][x];

			DrawGraph(
				x * m_chipSize + offsetX,
				y * m_chipSize + offsetY,
				m_ghChip[tileNo],
				FALSE);

			int objectNo = m_objectmap[m_nextmap][y][x];

			if (objectNo >= 0)
			{
				DrawGraph(
					x * m_chipSize + offsetX,
					y * m_chipSize + offsetY,
					m_ghChip[objectNo],
					TRUE);
			}
		}
	}
}
void Map::Finalize()
{

}

void Map::LoadMapChip(const wchar_t* fileName, int mapData[MAP_NUM][MAP_HEIGHT][MAP_WIDTH])
{
	std::ifstream      ifs;//ファイルストリーム
	std::string       line;//1行のデータ
	std::istringstream iss;//文字列ストリーム

	//ファイルオープン
	ifs.open(fileName);
	assert(ifs.is_open() && "CSV Open Error");

	for (int map = 0; map < MAP_NUM; map++)
	{
		for (int y = 0; y < MAP_HEIGHT; y++)
		{
			std::getline(ifs, line);

			iss.clear();
			iss.str(line);

			for (int x = 0; x < MAP_WIDTH; x++)
			{
				std::string item;
				std::getline(iss, item, ',');
				// 最初のセルだけBOMを除去
				if (map == 0 && y == 0 && x == 0)
				{
					const std::string bom = "\xEF\xBB\xBF";
					if (item.compare(0, bom.size(), bom) == 0)
					{
						item.erase(0, bom.size());
					}
				}
				int tile = 0;

				if (!item.empty())
				{
					tile = std::stoi(item);
				}
				mapData[map][y][x] = tile;

				if (tile >= 0 && tile <= 32)
					m_basemap[map][y][x] = TileType::Floor;
				else if (tile <= 80)
					m_basemap[map][y][x] = TileType::Wall;
				else
					m_basemap[map][y][x] = TileType::Object;
			}
		}
	}
	ifs.close();
	
	printf("Map0[5][38] = %d\n", m_workmap[0][5][38]);
}

Map::TileType Map::GetTileType(int x, int y)const
{
	if(x < -1 || x >= MAP_WIDTH+1 ||
		y < -1 || y >= MAP_HEIGHT+1)
	{
		return TileType::Wall;   // 範囲外は壁扱い
	}
	return m_basemap[m_currentMap][y][x];
}
int Map::GetTileNo(int x, int y) const
{
	return m_workmap[m_currentMap][y][x];
}

void Map::ChangeMap(int mapNo)
{
	m_currentMap = mapNo;
}
int Map::GetChipSize()const
{
	return m_chipSize;
}