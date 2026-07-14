#include "pch.h"
#include "Game/Map/Map.h"

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
	m_chipSize = 32;
	m_currentMap = 0;
	LoadDivGraph(L"Resources/Textures/testmapchip.png",GH_MAX,16,20,m_chipSize,m_chipSize,m_ghChip);	//横１６個、
	for (auto& gh : m_ghChip)
	{
		assert(gh != (-1) && "タイルマップをロードできませんでした");
	}

	LoadMapChip(L"Resources/CSV/map.csv");
}
void Map::Update()
{

}
void Map::Render()
{
	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			int tileNo = m_workmap[m_currentMap][y][x];

			DrawGraph(x * m_chipSize,y * m_chipSize,m_ghChip[tileNo],FALSE);
		}
	}
}
void Map::Finalize()
{

}

void Map::LoadMapChip(const wchar_t* fileName)
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
				m_workmap[map][y][x] = tile;

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
	if(x < 0 || x >= MAP_WIDTH ||
		y < 0 || y >= MAP_HEIGHT)
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