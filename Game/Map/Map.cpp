#include "pch.h"
#include "Game/Map/Map.h"


#include"Game/InputManager/InputManager.h"
#include"Game/Player/PlayerManager.h"
#include"Game/Screen.h"
#include<fstream>
#include<sstream>
#include<cassert>
Map::Map()
	:m_basemap	 { }
	,m_workmap	 { }
	,m_ghChip	 { }
	,m_chipSize	 { }
	,m_currentMap{ }
	,m_stageNo	 {0}
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
	m_stageNo = 0;

	m_isTransition = false;
	m_transition = 0;
	m_nextmap = 0;

	m_chipSize = 32;
	m_currentMap = 0;
	LoadDivGraph(L"Resources/Textures/testmapchip.png",GH_MAX,16,24,m_chipSize,m_chipSize,m_ghChip);	//横１６個、
	for (auto& gh : m_ghChip)
	{
		assert(gh != (-1) && "タイルマップをロードできませんでした");
	}

	LoadMapChip(L"Resources/CSV/map.csv", m_workmap);
	LoadMapChip(L"Resources/CSV/object.csv",m_objectmap);
}
void Map::Update(InputManager&inputManger,PlayerManager&playerManager)
{
	int startMap = GetStageStartMap();
	int endMap = GetStageEndMap();
	// 右端
	if (!m_isTransition &&
		playerManager.m_position.x + m_chipSize >= Screen::RIGHT)
	{
		m_moveDir = MoveDir::Right;

		int local = m_currentMap - startMap;

		if (local >= 8)
			m_nextmap = m_currentMap - 8; // 8→0, 9→1
		else
			m_nextmap = m_currentMap + 2;

		m_transition = 0;
		m_isTransition = true;
	}
	//左端から
	if (!m_isTransition && playerManager.m_position.x <= Screen::LEFT)
	{
		m_moveDir = MoveDir::Left;

		int local = m_currentMap - startMap;

		if (local < 2)
			m_nextmap = m_currentMap + 8; // 0→8, 1→9
		else
			m_nextmap = m_currentMap - 2;

		m_transition = 0;
		m_isTransition = true;
	}
	//上から
	if (!m_isTransition && playerManager.m_position.y < Screen::TOP)
	{
		m_moveDir = MoveDir::Up;

		if (m_currentMap <= startMap)
			m_nextmap = endMap;
		else
			m_nextmap = m_currentMap - 1;

		m_transition = 0;
		m_isTransition = true;
	}
	// 下端
	if (!m_isTransition &&
		playerManager.m_position.y + m_chipSize >= Screen::BOTTOM)
	{
		m_moveDir = MoveDir::Down;

		if (m_currentMap >= endMap)
			m_nextmap = startMap;
		else
			m_nextmap = m_currentMap + 1;

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
				playerManager.m_position.x = 0+m_chipSize;
			else if (m_moveDir == MoveDir::Left)
				playerManager.m_position.x = Screen::RIGHT - m_chipSize*2;
			else if (m_moveDir == MoveDir::Up)
				playerManager.m_position.y = Screen::BOTTOM - m_chipSize-16;
			else if (m_moveDir == MoveDir::Down)
				playerManager.m_position.y = 0+m_chipSize;
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

			DrawGraph(x * m_chipSize + offsetX,	y * m_chipSize + offsetY,m_ghChip[tileNo],FALSE);

			int objectNo = m_objectmap[m_currentMap][y][x];

			if (objectNo >= 0)
			{
				DrawGraph(x * m_chipSize + offsetX,y * m_chipSize + offsetY,m_ghChip[objectNo],	TRUE);
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

			DrawGraph(x * m_chipSize + offsetX,	y * m_chipSize + offsetY,m_ghChip[tileNo],FALSE);

			int objectNo = m_objectmap[m_nextmap][y][x];

			if (objectNo >= 0)
			{
				DrawGraph(x * m_chipSize + offsetX,y * m_chipSize + offsetY,m_ghChip[objectNo],	TRUE);
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

				if (tile >= 0 && tile < 32)
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
int Map::GetCurrentMap()const
{
	return m_currentMap;
}

int Map::GetStageStartMap() const
{
	return m_stageNo * MAPS_PER_STAGE;
}

int Map::GetStageEndMap() const
{
	return GetStageStartMap() + MAPS_PER_STAGE - 1;
}

void Map::ChangeStage(int stageNo)
{
	m_stageNo = stageNo;
	m_currentMap = GetStageStartMap();
}

/**
 * @brief 指定範囲のオブジェクトとタイルを置き換える
 * @param centerX 範囲の中心X座標
 * @param centerY 範囲の中心Y座標
 * @param left    中心から左方向の範囲
 * @param right   中心から右方向の範囲
 * @param top     中心から上方向の範囲
 * @param bottom  中心から下方向の範囲
 * @param targetObject 置換対象のオブジェクトID（負値で全対象）
 * @param replaceObject 置換後のオブジェクトID
 * @param replaceType   置換後のタイル種別
 */
void Map::BreakArea(int centerX, int centerY,int left, int right,int top, int bottom,int targetObject,int replaceObject,TileType replaceType)
{

	for (int y = top; y <= bottom; ++y)
	{
		for (int x = left; x <= right; ++x)
		{
			int tx = centerX + x;
			int ty = centerY + y;

			// 範囲外防止
			if (tx < 0 || tx >= MAP_WIDTH ||ty < 0 || ty >= MAP_HEIGHT)
			{
				continue;
			}

			if (targetObject < 0 ||m_objectmap[m_currentMap][ty][tx] == targetObject)
			{
				m_objectmap[m_currentMap][ty][tx] = replaceObject;
				m_basemap[m_currentMap][ty][tx] = replaceType;
			}
		}
	}
}

		void Map::NormalBreak(PlayerManager& player)
		{
			printfDx(L"NormalBleak called");
			Vector2 position = player.GetPosition();

			int tileX = static_cast<int>(position.x) / m_chipSize;
			int tileY = static_cast<int>(position.y) / m_chipSize;

			BreakArea(tileX, tileY, 0, 1, 0, 0, 40, 1, TileType::Floor);//右１マスを削る
		}

		void Map::FireBreak(PlayerManager& player)
		{
			printfDx(L"FireBreak called\n");

			Vector2 position = player.GetPosition();

			int tileX = static_cast<int>(position.x) / m_chipSize;
			int tileY = static_cast<int>(position.y) / m_chipSize;

			BreakArea(tileX, tileY, -5, 5, -5, 5,57, 1, TileType::Floor);//枯れ木（５７）を燃やす
		}

		void Map::WaterBreak(PlayerManager& player)
		{
			printfDx(L"WaterBreak called");
			Vector2 position = player.GetPosition();

			int tileX = static_cast<int>(position.x) / m_chipSize;
			int tileY = static_cast<int>(position.y) / m_chipSize;

			BreakArea(tileX,tileY, 0, 0, 0, 0,40, 1, TileType::Floor);
		}

		void Map::GrassBreak(PlayerManager& player)
		{
			printfDx(L"GrassBreak called");
			Vector2 position = player.GetPosition();

			int tileX = static_cast<int>(position.x) / m_chipSize;
			int tileY = static_cast<int>(position.y) / m_chipSize;

			BreakArea(tileX, tileY, -5, 5, -5, 5, 3, 92, TileType::Floor);//土（３）を草（９２）に変える
		}

		void Map::SoilBreak(PlayerManager& player)
		{
			printfDx(L"SoilBreak called");
			Vector2 position = player.GetPosition();

			int tileX = static_cast<int>(position.x) / m_chipSize;
			int tileY = static_cast<int>(position.y) / m_chipSize;

			BreakArea(tileX, tileY, -2, 2, -2, 2, 33, 3, TileType::Floor);//水（３３）を土（３）に変える

		}

		void Map::WindBreak(PlayerManager& player)
		{
			printfDx(L"WindBreak called");

			Vector2 position = player.GetPosition();

			int tileX = static_cast<int>(position.x) / m_chipSize;
			int tileY = static_cast<int>(position.y) / m_chipSize;

			BreakArea(tileX, tileY, 0, 3, 0, 0, 40, 57, TileType::Wall);//右３マス分枯れ木に変える
		}

		void Map::ThunderBreak(PlayerManager& player)
		{
		}

////連携技
		void Map::SteamExplosionBreak(PlayerManager& player)
		{
			Vector2 position = player.GetPosition();
			BreakArea(position.x, position.y, -10, 9, -10, 9,-1, 3, TileType::Floor);
		}

		void Map::FloorBreak(PlayerManager& player)
		{
			Vector2 position = player.GetPosition();

			BreakArea(position.x + 5, position.y, 0, 0, 0, 0,-1, 200, TileType::NextFloor);
		}

		void Map::WaterFlowsBreak(PlayerManager& player)
		{
			for (int y = 0; y < MAP_HEIGHT; ++y)
			{
				for (int x = 0; x < MAP_WIDTH; ++x)
				{
					if (m_objectmap[m_currentMap][y][x] == 200)
					{
						m_objectmap[m_currentMap][y][x] = 33;
						m_basemap[m_currentMap][y][x] = TileType::Wall;
					}
				}
			}
		}

		void Map::GrowGrassBreak(PlayerManager& player)
		{
		}

		void Map::VolcazationBreak(PlayerManager& player)
		{
		}






