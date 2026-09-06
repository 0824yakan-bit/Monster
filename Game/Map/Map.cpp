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
	,m_beforChange	{}
	,m_afterChange	{}
	,m_difference	{false}
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
	m_level = 0;
	m_breakLevel = 0;
	for (int i = 0; i < MAP_NUM; i++)
	{
		m_leftChangeX[i] = 0;
		m_leftChangeY[i] = MAP_HEIGHT - 1;

		m_rightChangeX[i] = MAP_WIDTH - 1;
		m_rightChangeY[i] = MAP_HEIGHT - 1;
	}
	m_changeMap = 9;

	m_changeTimer = 0;
	m_changeInterval = 5;

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

	LoadMapChip(L"Resources/CSV/map.csv"	,m_workmap		);
	LoadMapChip(L"Resources/CSV/object.csv"	,m_objectmap	);
	LoadMapChip(L"Resources/CSV/object.csv"	,m_beforChange	);//変更前情報保存


	for (int map = 0;map < MAP_NUM;++map)
	{
		for (int y = 0;y < MAP_HEIGHT;++y)
		{
			for (int x = 0;x < MAP_WIDTH;++x)
			{
				m_fog[map][y][x] = true;//デフォルトで有効状態
				m_difference[map][y][x] = false;//デフォルトで無効状態
			}
		}
	}
}
void Map::Update(InputManager&inputManger,PlayerManager&playerManager)
{
	//デバッグ用キー
	if (CheckHitKey(KEY_INPUT_Q))m_level = 0;
	if (CheckHitKey(KEY_INPUT_W))m_level = 1;
	if (CheckHitKey(KEY_INPUT_E))m_level = 2;
	if (CheckHitKey(KEY_INPUT_R))m_level = 3;
	if (CheckHitKey(KEY_INPUT_T))m_level = 4;

	if (CheckHitKey(KEY_INPUT_Y))m_bossManager.DefeatBoss(0);//boss1撃破
	if (CheckHitKey(KEY_INPUT_U))m_bossManager.DefeatBoss(1);//boss2撃破
	if (CheckHitKey(KEY_INPUT_I))m_bossManager.DefeatBoss(2);//boss3撃破////一回でも戦闘しないと移動してはいけない
	if (CheckHitKey(KEY_INPUT_O))m_bossManager.DefeatBoss(3);//boss4撃破ラスボス

	if (CheckHitKey(KEY_INPUT_1))m_currentMap = 0;//ステージ１に移動
	if (CheckHitKey(KEY_INPUT_2))m_currentMap = 1;//ステージ２に移動
	if (CheckHitKey(KEY_INPUT_3))m_currentMap = 2;//ステージ３に移動
	if (CheckHitKey(KEY_INPUT_4))m_currentMap = 3;//ステージ４に移動
	if (CheckHitKey(KEY_INPUT_5))m_currentMap = 4;//ステージ５に移動
	if (CheckHitKey(KEY_INPUT_6))m_currentMap = 5;//ステージ６に移動
	if (CheckHitKey(KEY_INPUT_7))m_currentMap = 6;//ステージ７に移動
	if (CheckHitKey(KEY_INPUT_8))m_currentMap = 7;//ステージ８に移動
	if (CheckHitKey(KEY_INPUT_9))m_currentMap = 8;//ステージ９に移動
	if (CheckHitKey(KEY_INPUT_0))m_currentMap = 9;//ステージ１０に移動
	//デバッグ用キー　終了

	m_fogdensity = m_level * 150;
	if (m_currentMap == 9)m_fogdensity = 0;
	int startMap = GetStageStartMap();
	int endMap = GetStageEndMap();

	int playerW = playerManager.m_size.x;
	int playerH = playerManager.m_size.y;

	if (m_bossManager.IsAllBossDefeated())
	{
		// 9番マップへの道を開く
		OpenBossArea();

	}

	if (m_bossManager.IsBossDefeated(3))//ラスボス撃破後
	{
		if (m_difference[GAME_CLEAR_MAP][GAME_CLEAR_Y][GAME_CLEAR_X] == false)
		{
			//printfDx(L"MapBreak");
			MapBreak();
		}

		LastBossDefeated();
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
		if (!m_isTransition &&playerManager.m_position.y + playerH >= Screen::BOTTOM)
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
// 崩してはいけない範囲
bool Map::IsSafeArea(int map, int x, int y)
{
	if (map == 0)
	{
		if (x >= 0 && x <= 15 &&y >= 0 && y <= 9)
		{
			return true;
		}
	}

	return false;
}
void Map::LastBossDefeated()
{
	m_changeTimer++;

	if (m_changeTimer < m_changeInterval)return;
	m_changeTimer = 0;

	int map = m_currentMap;
	// 左側
	int leftX = m_leftChangeX[map];
	int leftY =m_leftChangeY[map] + GetRand(3) - 1;

	if (leftY < 0)leftY = 0;
	if (leftY >= MAP_HEIGHT)leftY = MAP_HEIGHT - 1;

	if (!IsSafeArea(map, leftX, leftY))
	{
		m_basemap[map][leftY][leftX] = TileType::Fall;
		m_workmap[map][leftY][leftX] = 291;
		m_objectmap[map][leftY][leftX] = -1;
	}

	// 左側は1～3マス進む
	m_leftChangeX[map] += GetRand(2) + 1;
	// Yも少し変化
	m_leftChangeY[map] += GetRand(3) - 1;
	if (m_leftChangeY[map] < 0)m_leftChangeY[map] = 0;
	if (m_leftChangeY[map] >= MAP_HEIGHT)m_leftChangeY[map] = MAP_HEIGHT - 1;
	
	// 右側
	int rightX = m_rightChangeX[map];
	int rightY =m_rightChangeY[map] + GetRand(3) - 1;

	if (rightY < 0)rightY = 0;
	if (rightY >= MAP_HEIGHT)rightY = MAP_HEIGHT - 1;

	if (!IsSafeArea(map, rightX, rightY))
	{
		m_basemap[map][rightY][rightX] = TileType::Fall;
		m_workmap[map][rightY][rightX] = 291;
		m_objectmap[map][rightY][rightX] = -1;
	}

	// 右側も独立して進む
	m_rightChangeX[map] -= GetRand(2) + 1;
	// Yも左とは別に変化
	m_rightChangeY[map] += GetRand(3) - 1;

	if (m_rightChangeY[map] < 0)m_rightChangeY[map] = 0;
	if (m_rightChangeY[map] >= MAP_HEIGHT)m_rightChangeY[map] = MAP_HEIGHT - 1;

	// 崩壊タイミング
	m_changeInterval = GetRand(20) + 10;
}

void Map::MapBreak()
{
	for (int y = 0;y < MAP_HEIGHT;y++)
	{
		for (int x = 0;x < MAP_WIDTH;x++)
		{
			for (int map = 0;map <MAP_NUM ;map++)
			{
				if (m_objectmap[map][y][x] != m_beforChange[map][y][x])
				{
					m_difference[map][y][x] = true;
					m_basemap	[map][y][x] = TileType::Fall;
					m_objectmap	[map][y][x] = 291;
				}
			}
		}
	}
	m_difference[GAME_CLEAR_MAP][GAME_CLEAR_Y][GAME_CLEAR_X] = true;
	m_basemap	[GAME_CLEAR_MAP][GAME_CLEAR_Y][GAME_CLEAR_X] = TileType::GameClear;
	m_objectmap	[GAME_CLEAR_MAP][GAME_CLEAR_Y][GAME_CLEAR_X] = 289;
}

bool Map::IsWallRect(int px, int py, int width, int height) const
{
	int left	= px / m_chipSize;
	int right	= (px + width - 1) / m_chipSize;
	int top		= py / m_chipSize;
	int bottom	= (py + height - 1) / m_chipSize;

	return	GetTileType(left, top)		== TileType::Wall ||
			GetTileType(right, top)		== TileType::Wall ||
			GetTileType(left, bottom)	== TileType::Wall ||
			GetTileType(right, bottom)	== TileType::Wall;
}
bool Map::IsTreasureRect(int px, int py, int width, int height) const
{
	int left	= px / m_chipSize;
	int right	= (px + width - 1) / m_chipSize;
	int top		= py / m_chipSize;
	int bottom	= (py + height - 1) / m_chipSize;

	return	GetTileType	(left, top)		== TileType::Treasure ||
			GetTileType	(right, top)	== TileType::Treasure ||
			GetTileType	(left, bottom)	== TileType::Treasure ||
			GetTileType	(right, bottom)	== TileType::Treasure;
}

bool Map::IsNextFloorRect(int px, int py, int width, int height) const
{
	int left	= px / m_chipSize;
	int right	= (px + width - 1) / m_chipSize;
	int top		= py / m_chipSize;
	int bottom	= (py + height - 1) / m_chipSize;

	return	GetTileType(left, top)		== TileType::NextFloor ||
			GetTileType(right, top)		== TileType::NextFloor ||
			GetTileType(left, bottom)	== TileType::NextFloor ||
			GetTileType(right, bottom)	== TileType::NextFloor;
}

bool Map::IsFallRect(int px, int py, int width, int height) const
{
	int left = px / m_chipSize;
	int right = (px + width - 1) / m_chipSize;
	int top = py / m_chipSize;
	int bottom = (py + height - 1) / m_chipSize;

	return	GetTileType(left, top)		== TileType::Fall ||
			GetTileType(right, top)		== TileType::Fall ||
			GetTileType(left, bottom)	== TileType::Fall ||
			GetTileType(right, bottom)	== TileType::Fall;
}

bool Map::IsSignboardRect(int px, int py, int width, int height) const
{
	int left = px / m_chipSize;
	int right = (px + width - 1) / m_chipSize;
	int top = py / m_chipSize;
	int bottom = (py + height - 1) / m_chipSize;

	return	GetTileType(left, top)		== TileType::Signboard ||
			GetTileType(right, top)		== TileType::Signboard ||
			GetTileType(left, bottom)	== TileType::Signboard ||
			GetTileType(right, bottom)	== TileType::Signboard;
}

bool Map::IsSlimeRect(int px, int py, int width, int height) const
{
	int left = px / m_chipSize;
	int right = (px + width - 1) / m_chipSize;
	int top = py / m_chipSize;
	int bottom = (py + height - 1) / m_chipSize;

	// プレイヤーが (5,5) のマスに触れているか
	if (m_currentMap == 0)
	{
		if (left <= 5 && 5 <= right && top <= 5 && 5 <= bottom)
		{
			return true;
		}
	}
	return false;
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
		m_objectmap	[8][Y_1][X]	= 6;
		m_objectmap	[8][Y_2][X]	= 6;
		m_basemap	[8][Y_1][X]	= TileType::Floor;
		m_basemap	[8][Y_2][X]	= TileType::Floor;

		m_isbossAreaOpen = true;//変更済みにする
	}
}
void Map::EnterBossArea()
{
	m_currentMap = 9;
}
void Map::ExitBossArea()
{
	m_currentMap = 8;
}
int Map::GetBreakLevel()const
{
	return m_level;
}

bool Map::PrepareBreakArea(int centerX,int centerY,int x,int y,int& tx,int& ty)
{
	tx = centerX + x;
	ty = centerY + y;

	if (tx < 0 || tx >= MAP_WIDTH ||ty < 0 || ty >= MAP_HEIGHT)
	{
		return false;
	}

	RevealArea(tx, ty, 2);

	return true;
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
void Map::BreakArea(int centerX,int centerY,int left,int right,int top,int bottom,int targetObject,int replaceObject,TileType replaceType,int dangerAdd)
{
	// 技1回分のBreakLevel上昇
	m_breakLevel = std::min(40, m_breakLevel + dangerAdd);
	m_level = m_breakLevel / 10;

	for (int y = top; y <= bottom; ++y)
	{
		for (int x = left; x <= right; ++x)
		{
			int tx;
			int ty;

			// 座標チェック＋霧解除
			if (!PrepareBreakArea(centerX, centerY,x, y,tx,ty))
			{
				continue;
			}

			// 対象オブジェクトなら変更
			if (targetObject < 0 ||
				m_objectmap[m_currentMap][ty][tx] == targetObject)
			{
				m_objectmap[m_currentMap][ty][tx] = replaceObject;
				m_basemap[m_currentMap][ty][tx] = replaceType;
			}
		}
	}
}
void Map::BreakAreaByGroup(int centerX,int centerY,int left,int right,int top,int bottom,TileGroup group,int dangerAdd)
{
	const auto& breakTiles =m_tileRole.GetBreakTiles(group);

	// 技1回分のBreakLevel上昇
	m_breakLevel = std::min(40, m_breakLevel + dangerAdd);
	m_level = m_breakLevel / 10;

	for (int y = top; y <= bottom; ++y)
	{
		for (int x = left; x <= right; ++x)
		{
			int tx;
			int ty;

			// 座標チェック＋霧解除
			if (!PrepareBreakArea(centerX, centerY,x, y,tx, ty))
			{
				continue;
			}

			int objectNo =m_objectmap[m_currentMap][ty][tx];

			// TileGroupに登録されたタイルを探す
			for (const auto& breakTile : breakTiles)
			{
				if (objectNo == breakTile.targetTile)
				{
					m_objectmap[m_currentMap][ty][tx]= breakTile.replaceTile;
					m_basemap[m_currentMap][ty][tx]= breakTile.replaceType;
					break;
				}
			}
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

			if (tx < 0 || tx >= MAP_WIDTH ||ty < 0 || ty >= MAP_HEIGHT)
				continue;

			m_fog[m_currentMap][ty][tx] = false;
		}
	}
}

		void Map::UsedTreasure(PlayerManager& player)
		{
			Vector2 pos = player.GetPosition();

			int tx = static_cast<int>(pos.x) / m_chipSize;
			int ty = static_cast<int>(pos.y) / m_chipSize;

			BreakArea(tx, ty, 0, 1, 0, 1, 96, 1, TileType::Floor, 0);//プレイヤー範囲内宝箱を使用済みにする
			BreakArea(tx, ty, 0, 1, 0, 1, 97, 1, TileType::Floor, 0);
		}


		void Map::NormalBreak(PlayerManager& player)
		{
			Accessory::UpgradeAccessory nomal = m_accessory.GetAccessory(Accessory::ElementType::NOMAL);

			Vector2 pos = player.GetPosition();

			int tx = static_cast<int>(pos.x) / m_chipSize;
			int ty = static_cast<int>(pos.y) / m_chipSize;

			BreakAreaByGroup(tx, ty, -1, 2, -1, 2,TileGroup::Normal,0);
		}

		void Map::FireBreak(PlayerManager& player)
		{
			Vector2 pos = player.GetPosition();

			int tx = static_cast<int>(pos.x) / m_chipSize;
			int ty = static_cast<int>(pos.y) / m_chipSize;

			BreakAreaByGroup(tx, ty, -2, 3, -2, 3, TileGroup::Fire, 3);


		}

		void Map::WaterBreak(PlayerManager& player)
		{
			Vector2 pos = player.GetPosition();

			int tx = static_cast<int>(pos.x) / m_chipSize;
			int ty = static_cast<int>(pos.y) / m_chipSize;

			BreakAreaByGroup(tx, ty, -3, 4, -3, 4, TileGroup::Water, 1);

		}

		void Map::GrassBreak(PlayerManager& player)
		{
			Vector2 pos = player.GetPosition();

			int tx = static_cast<int>(pos.x) / m_chipSize;
			int ty = static_cast<int>(pos.y) / m_chipSize;

			BreakAreaByGroup(tx, ty, -3, 4, -3, 4, TileGroup::Grass, 0);


		}

		void Map::SoilBreak(PlayerManager& player)
		{
			Vector2 pos = player.GetPosition();

			int tx = static_cast<int>(pos.x) / m_chipSize;
			int ty = static_cast<int>(pos.y) / m_chipSize;

			BreakAreaByGroup(tx, ty, -3, 4, -3, 4, TileGroup::Soil, 5);

		}

		void Map::WindBreak(PlayerManager& player)
		{
			Vector2 pos = player.GetPosition();

			int tx = static_cast<int>(pos.x) / m_chipSize;
			int ty = static_cast<int>(pos.y) / m_chipSize;

			PlayerManager::Direction dir = player.GetDirection();

			const int length = 6; // 風の長さ

			switch (dir)
			{
			case PlayerManager::Direction::Right:
				// → 右へ一直線
				BreakAreaByGroup(tx + 1, ty, 0, length, 0, 1,TileGroup::Wind, 2);
				break;

			case PlayerManager::Direction::Left:
				// ← 左へ一直線
				BreakAreaByGroup(tx - 1, ty, -length, 0, 0, 1, TileGroup::Wind, 2);
				break;

			case PlayerManager::Direction::Up:
				// ↑ 上へ一直線
				BreakAreaByGroup(tx, ty - 1, 0, 1, -length, 0,TileGroup::Wind, 2);
				break;

			case PlayerManager::Direction::Down:
				// ↓ 下へ一直線
				BreakAreaByGroup(tx, ty + 1, 0, 1, 0, length,TileGroup::Wind, 2);
				break;
			}
		}

		void Map::DarknessBreak(PlayerManager& player)
		{
			Vector2 pos = player.GetPosition();

			int tx = static_cast<int>(pos.x) / m_chipSize;
			int ty = static_cast<int>(pos.y) / m_chipSize;

			BreakAreaByGroup(tx, ty, -6, 7, -6, 7, TileGroup::Darkness, 5);

		}

////連携技
		void Map::SteamExplosionBreak(PlayerManager& player)
		{
			Vector2 pos = player.GetPosition();
			int tx = static_cast<int>(pos.x) / m_chipSize;
			int ty = static_cast<int>(pos.y) / m_chipSize;

			BreakArea(tx, ty, -2, 3, -2, 3, -1, 3, TileType::Floor, 5);
		}

		void Map::FloorBreak(PlayerManager& player)
		{
			Vector2 pos = player.GetPosition();
			int tx = static_cast<int>(pos.x) / m_chipSize;
			int ty = static_cast<int>(pos.y) / m_chipSize;

			BreakArea(tx , ty, -6, 7, -6, 7, -1, 291, TileType::Fall, 10);
			BreakArea(tx, ty, -2, 3, -2, 3, -1, 1, TileType::Floor, 0);
		}

		void Map::WaterFlowsBreak(PlayerManager& player)
		{
			for (int y = 0; y < MAP_HEIGHT; ++y)
			{
				for (int x = 0; x < MAP_WIDTH; ++x)
				{
					if (m_objectmap[m_currentMap][y][x] == 134)
					{
						m_objectmap[m_currentMap][y][x] = 3;
						m_basemap[m_currentMap][y][x] = TileType::Floor;
					}
				}
			}
		}

		void Map::GrowGrassBreak(PlayerManager& player)
		{
			Vector2 pos = player.GetPosition();
			int tx = static_cast<int>(pos.x) / m_chipSize;
			int ty = static_cast<int>(pos.y) / m_chipSize;

			BreakArea(tx, ty, -2, 3, -2, 3, -1, 91, TileType::GrassLounge, 0);
		}

		void Map::VolcazationBreak(PlayerManager& player)
		{
			Vector2 pos = player.GetPosition();
			int tx = static_cast<int>(pos.x) / m_chipSize;
			int ty = static_cast<int>(pos.y) / m_chipSize;

			BreakArea(tx, ty, -2, 3, -2, 3, 32, 3, TileType::Floor, 10);
		}