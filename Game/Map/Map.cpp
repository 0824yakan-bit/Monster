#include "pch.h"
#include "Game/Map/Map.h"


#include"Game/InputManager/InputManager.h"
#include"Game/Player/PlayerManager.h"
#include"Game/Enemy/BossManager.h"
#include"Game/Screen.h"
#include<fstream>
#include<sstream>
#include<cassert>
Map::Map(Accessory& accessory,BossManager& bossManager)
	:m_accessory	{ accessory }
	,m_bossManager	{ bossManager}
	,m_basemap		{ }
	,m_workmap		{ }
	,m_fog			{false}
	,m_ghChip		{ }
	,m_chipSize		{ }
	,m_currentMap	{ }
	,m_stageNo		{0}
	,m_isbossAreaOpen{false}
	,m_breakLevel	{0}
	,m_transition	{ }
	,m_objectmap	{ }
	,m_nextmap		{ }
	,m_moveDir		{ }
	,m_level		{ }
	,m_isTransition	{ }
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
	m_tileRole.Initialize();

	m_moveDir = MoveDir::None;
	m_stageNo = 0;
	m_isbossAreaOpen = false;
	m_breakLevel = 0;


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

	for (int map = 0;map < MAP_NUM;++map)
	{
		for (int y = 0;y < MAP_HEIGHT;++y)
		{
			for (int x = 0;x < MAP_WIDTH;++x)
			{
				m_fog[map][y][x] = true;
			}
		}
	}
}
void Map::Update(InputManager&inputManger,PlayerManager&playerManager)
{
	//デバッグ用キー
	if (CheckHitKey(KEY_INPUT_0))m_level = 0;
	if (CheckHitKey(KEY_INPUT_1))m_level = 1;
	if (CheckHitKey(KEY_INPUT_2))m_level = 2;
	if (CheckHitKey(KEY_INPUT_3))m_level = 3;
	if (CheckHitKey(KEY_INPUT_4))m_level = 4
		;
	if (CheckHitKey(KEY_INPUT_5))m_bossManager.DefeatBoss(0);//boss1撃破
	if (CheckHitKey(KEY_INPUT_6))m_bossManager.DefeatBoss(1);//boss2撃破
	if (CheckHitKey(KEY_INPUT_7))m_bossManager.DefeatBoss(2);//boss3撃破
	//デバッグ用キー　終了

	m_fogdensity = m_level * 150;
	int startMap = GetStageStartMap();
	int endMap = GetStageEndMap();

	int playerW = playerManager.m_size.x;
	int playerH = playerManager.m_size.y;

	if (m_bossManager.IsAllBossDefeated())
	{
		// 9番マップへの道を開く
		OpenBossArea();

	}

	if (m_currentMap != 9)//9の画面端は移動不可
	{
		// 右端
		if (!m_isTransition && playerManager.m_position.x + playerW >= Screen::RIGHT)
		{
			m_moveDir = MoveDir::Right;

			int local = m_currentMap - startMap;

			if (local >= 8)
				m_nextmap = m_currentMap - 8; // 8→0, 9→1
			else
				m_nextmap = m_currentMap + 2;
			// 9番マップには通常移動できない
			if (m_nextmap == 9)
				m_nextmap = 0;

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
			// 9番マップには通常移動できない
			if (m_nextmap == 9)
				m_nextmap = 8;

			m_transition = 0;
			m_isTransition = true;
		}
		//上から
		if (!m_isTransition && playerManager.m_position.y <= Screen::TOP)
		{
			m_moveDir = MoveDir::Up;

			if (m_currentMap <= startMap)
				m_nextmap = endMap;
			else
				m_nextmap = m_currentMap - 1;
			// 9番マップには通常移動できない
			if (m_nextmap == 9)
				m_nextmap = 8;

			m_transition = 0;
			m_isTransition = true;
		}
		// 下端
		if (!m_isTransition &&
			playerManager.m_position.y + playerH >= Screen::BOTTOM)
		{
			m_moveDir = MoveDir::Down;

			if (m_currentMap >= endMap)
				m_nextmap = startMap;
			else
				m_nextmap = m_currentMap + 1;
			// 9番マップには通常移動できない
			if (m_nextmap == 9)
				m_nextmap = 0;

			m_transition = 0;
			m_isTransition = true;
		}
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
				playerManager.m_position.x = Screen::RIGHT - playerW-m_chipSize;
			else if (m_moveDir == MoveDir::Up)
				playerManager.m_position.y = Screen::BOTTOM - playerH-m_chipSize-16;
			else if (m_moveDir == MoveDir::Down)
				playerManager.m_position.y = 0+m_chipSize;
			m_moveDir = MoveDir::None;
		}
	}
	int px = playerManager.m_position.x / m_chipSize;
	int py = playerManager.m_position.y / m_chipSize;

	RevealArea(px, py, 4);
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
		///霧描画
			if (m_fog[m_currentMap][y][x])
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fogdensity);
				DrawGraph(x * m_chipSize + offsetX, y * m_chipSize + offsetY, m_ghChip[290], TRUE);
				//DrawBox(x * m_chipSize + offsetX,y * m_chipSize + offsetY,x * m_chipSize + offsetX + m_chipSize,y * m_chipSize + offsetY + m_chipSize,GetColor(0, 0, 0),TRUE);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
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

		///霧描画
			if (m_fog[m_nextmap][y][x])
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fogdensity);
				DrawGraph(x * m_chipSize + offsetX, y * m_chipSize + offsetY, m_ghChip[290], TRUE);
				//DrawBox(x * m_chipSize + offsetX,y * m_chipSize + offsetY,x * m_chipSize + offsetX + m_chipSize,y * m_chipSize + offsetY + m_chipSize,GetColor(0, 0, 0),TRUE);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
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
				// 画像ID保存
				mapData[map][y][x] = tile;

				// 役割をTileRoleに問い合わせて保存
				m_basemap[map][y][x] = m_tileRole.GetType(tile);
			}
		}
	}
	ifs.close();
}

bool Map::IsWallRect(int px, int py, int width, int height) const
{
	int left = px / m_chipSize;
	int right = (px + width - 1) / m_chipSize;
	int top = py / m_chipSize;
	int bottom = (py + height - 1) / m_chipSize;

	return GetTileType(left, top) == TileType::Wall ||
		GetTileType(right, top) == TileType::Wall ||
		GetTileType(left, bottom) == TileType::Wall ||
		GetTileType(right, bottom) == TileType::Wall;
}
bool Map::IsTreasureRect(int px, int py, int width, int height) const
{
	int left = px / m_chipSize;
	int right = (px + width - 1) / m_chipSize;
	int top = py / m_chipSize;
	int bottom = (py + height - 1) / m_chipSize;

	return GetTileType(left, top) == TileType::Treasure ||
		GetTileType(right, top) == TileType::Treasure ||
		GetTileType(left, bottom) == TileType::Treasure ||
		GetTileType(right, bottom) == TileType::Treasure;
}

TileType Map::GetTileType(int x, int y) const
{
	if (x < 0 || x >= MAP_WIDTH ||
		y < 0 || y >= MAP_HEIGHT)
	{
		return TileType::Wall;
	}

	return m_basemap[m_currentMap][y][x];
}
int Map::GetTileNo(int x, int y) const
{
	return m_objectmap[m_currentMap][y][x];
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

void Map::OpenBossArea()
{
	if (!m_isbossAreaOpen)//マップチップを変更したか
	{
		m_objectmap[8][10][30] = 288;
		m_objectmap[8][11][30] = 288;
		m_objectmap[8][10][31] = 288;
		m_objectmap[8][11][31] = 288;
		m_basemap[8][10][30] = TileType::NextFloor;
		m_basemap[8][11][30] = TileType::NextFloor;
		m_basemap[8][10][31] = TileType::NextFloor;
		m_basemap[8][11][31] = TileType::NextFloor;

		m_isbossAreaOpen = true;//変更済みにする
	}
}

int Map::GetBreakLevel()const
{
	return m_level;
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
 * @param dangerAdd   属性ごとの上昇量
 */
void Map::BreakArea(int centerX, int centerY,int left, int right,int top, int bottom,int targetObject,int replaceObject,TileType replaceType, int dangerAdd)
{
	m_breakLevel = std::min(40, m_breakLevel + dangerAdd);
	m_level = m_breakLevel / 10;
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
			RevealArea(tx, ty, 2);
		}
	}
}
void Map::RevealArea(int centerX, int centerY, int radius)
{
	for (int y = -radius; y <= radius; ++y)
	{
		for (int x = -radius; x <= radius; ++x)
		{
			int tx = centerX + x;
			int ty = centerY + y;

			if (tx < 0 || tx >= MAP_WIDTH ||
				ty < 0 || ty >= MAP_HEIGHT)
				continue;

			m_fog[m_currentMap][ty][tx] = false;
		}
	}
}

		void Map::UsedTreasure(PlayerManager& player)
		{
			printfDx(L"UsedTreasure called");

			Vector2 pos = player.GetPosition();

			int tx = static_cast<int>(pos.x) / m_chipSize;
			int ty = static_cast<int>(pos.y) / m_chipSize;

			BreakArea(tx, ty, 0, 1, 0, 1, 96, 1, TileType::Floor, 0);//プレイヤー範囲内宝箱を使用済みにする
			BreakArea(tx, ty, 0, 1, 0, 1, 97, 1, TileType::Floor, 0);
		}


		void Map::NormalBreak(PlayerManager& player)
		{
			printfDx(L"NormalBleak called");

			Accessory::UpgradeAccessory nomal = m_accessory.GetAccessory(Accessory::ElementType::NOMAL);

			Vector2 pos = player.GetPosition();

			int tx = static_cast<int>(pos.x) / m_chipSize;
			int ty = static_cast<int>(pos.y) / m_chipSize;

			BreakArea(tx, ty, 0, 1, 0, 0, 40, 1, TileType::Floor,0);//右１マスを削る
			BreakArea(tx, ty, -1, 1, -1, 1, -1, 1, TileType::Floor,1);
		}

		void Map::FireBreak(PlayerManager& player)
		{
			printfDx(L"FireBreak called\n");

			Vector2 pos = player.GetPosition();

			int tx = static_cast<int>(pos.x) / m_chipSize;
			int ty = static_cast<int>(pos.y) / m_chipSize;

			BreakArea(tx, ty, -5, 5, -5, 5,57, 1, TileType::Floor,3);//枯れ木（５７）を燃やす

		}

		void Map::WaterBreak(PlayerManager& player)
		{
			printfDx(L"WaterBreak called");
			Vector2 pos = player.GetPosition();

			int tx = static_cast<int>(pos.x) / m_chipSize;
			int ty = static_cast<int>(pos.y) / m_chipSize;

			BreakArea(tx, ty, 0, 0, 0, 0,40, 1, TileType::Floor,1);
		}

		void Map::GrassBreak(PlayerManager& player)
		{
			printfDx(L"GrassBreak called");
			Vector2 pos = player.GetPosition();

			int tx = static_cast<int>(pos.x) / m_chipSize;
			int ty = static_cast<int>(pos.y) / m_chipSize;

			BreakArea(tx, ty, -5, 5, -5, 5, 3, 92, TileType::Floor,0);//土（３）を草（９２）に変える

		}

		void Map::SoilBreak(PlayerManager& player)
		{
			printfDx(L"SoilBreak called");
			Vector2 pos = player.GetPosition();

			int tx = static_cast<int>(pos.x) / m_chipSize;
			int ty = static_cast<int>(pos.y) / m_chipSize;

			BreakArea(tx, ty, -2, 2, -2, 2, 33, 3, TileType::Floor,2);//水（３３）を土（３）に変える
		}

		void Map::WindBreak(PlayerManager& player)
		{
			printfDx(L"WindBreak called");

			Vector2 pos = player.GetPosition();

			int tx = static_cast<int>(pos.x) / m_chipSize;
			int ty = static_cast<int>(pos.y) / m_chipSize;

			PlayerManager::Direction dir = player.GetDirection();

			const int length = 6; // 風の長さ

			switch (dir)
			{
			case PlayerManager::Direction::Right:
				// → 右へ一直線
				BreakArea(tx + 1, ty, 0, length, 0, 1,40, 57, TileType::Wall, 2);
				BreakArea(tx + 1, ty, 0, length, 0, 1,41, 57, TileType::Wall, 2);
				break;

			case PlayerManager::Direction::Left:
				// ← 左へ一直線
				BreakArea(tx - 1, ty, -length, 0, 0, 1,40, 57, TileType::Wall, 2);
				BreakArea(tx - 1, ty, -length, 0, 0, 1,41, 57, TileType::Wall, 2);
				break;

			case PlayerManager::Direction::Up:
				// ↑ 上へ一直線
				BreakArea(tx, ty - 1, 0, 1, -length, 0,40, 57, TileType::Wall, 2);
				BreakArea(tx, ty - 1, 0, 1, -length, 0,41, 57, TileType::Wall, 2);
				break;

			case PlayerManager::Direction::Down:
				// ↓ 下へ一直線
				BreakArea(tx, ty + 1, 0, 1, 0, length,40, 57, TileType::Wall, 2);
				BreakArea(tx, ty + 1, 0, 1, 0, length,41, 57, TileType::Wall, 2);
				break;
			}
		}

		void Map::ThunderBreak(PlayerManager& player)
		{
			Vector2 pos = player.GetPosition();
			int tx = static_cast<int>(pos.x) / m_chipSize;
			int ty = static_cast<int>(pos.y) / m_chipSize;


			// 隠し壁(250)を可視化
			for (int y = -10; y <= 10; ++y)
			{
				for (int x = -10; x <= 10; ++x)
				{
					int nx = tx + x;
					int ny = ty + y;

					if (nx < 0 || nx >= MAP_WIDTH ||
						ny < 0 || ny >= MAP_HEIGHT)
						continue;

					if (m_objectmap[m_currentMap][ny][nx] == 250)
					{
						m_objectmap[m_currentMap][ny][nx] = 251;
					}
				}
			}

			m_breakLevel = std::min(40, m_breakLevel + 1);
		}

////連携技
		void Map::SteamExplosionBreak(PlayerManager& player)
		{
			Vector2 pos = player.GetPosition();
			int tx = static_cast<int>(pos.x) / m_chipSize;
			int ty = static_cast<int>(pos.y) / m_chipSize;

			BreakArea(tx, ty, -6, 6, -6, 6, -1, 3, TileType::Floor, 5);
		}

		void Map::FloorBreak(PlayerManager& player)
		{
			Vector2 pos = player.GetPosition();
			int tx = static_cast<int>(pos.x) / m_chipSize;
			int ty = static_cast<int>(pos.y) / m_chipSize;

			BreakArea(tx + 3, ty, -1, 1, -1, 1, -1, 200, TileType::NextFloor, 20);
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






