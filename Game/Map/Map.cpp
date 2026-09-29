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
	m_fogdensity = 0;
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

	m_speedrand = 10;
	m_leftDirection = 1;
	m_rightDirection = -1;
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
void Map::Update(InputManager& inputManger, PlayerManager& playerManager)
{
	//// デバッグ用キー
	//if (CheckHitKey(KEY_INPUT_Q)) m_level = 0;
	//if (CheckHitKey(KEY_INPUT_W)) m_level = 1;
	//if (CheckHitKey(KEY_INPUT_E)) m_level = 2;
	//if (CheckHitKey(KEY_INPUT_R)) m_level = 3;
	//if (CheckHitKey(KEY_INPUT_T)) m_level = 4;

	//if (CheckHitKey(KEY_INPUT_Y)) m_bossManager.DefeatBoss(0);
	//if (CheckHitKey(KEY_INPUT_U)) m_bossManager.DefeatBoss(1);
	//if (CheckHitKey(KEY_INPUT_I)) m_bossManager.DefeatBoss(2);
	//if (CheckHitKey(KEY_INPUT_O)) m_bossManager.DefeatBoss(3);

	//if (CheckHitKey(KEY_INPUT_1)) m_currentMap = 0;
	//if (CheckHitKey(KEY_INPUT_2)) m_currentMap = 1;
	//if (CheckHitKey(KEY_INPUT_3)) m_currentMap = 2;
	//if (CheckHitKey(KEY_INPUT_4)) m_currentMap = 3;
	//if (CheckHitKey(KEY_INPUT_5)) m_currentMap = 4;
	//if (CheckHitKey(KEY_INPUT_6)) m_currentMap = 5;
	//if (CheckHitKey(KEY_INPUT_7)) m_currentMap = 6;
	//if (CheckHitKey(KEY_INPUT_8)) m_currentMap = 7;
	//if (CheckHitKey(KEY_INPUT_9)) m_currentMap = 8;
	//if (CheckHitKey(KEY_INPUT_0)) m_currentMap = 9;

	//// デバッグ用キー終了


	m_fogdensity = m_level * 150;

	if (m_currentMap == 9)
		m_fogdensity = 0;

	int startMap = GetStageStartMap();
	int endMap = GetStageEndMap();


	//==================================================
	// プレイヤーサイズ
	//
	// m_size は「マス数」
	//==================================================

	int playerW = playerManager.m_size.x;
	int playerH = playerManager.m_size.y;


	//==================================================
	// 全ボス撃破
	//==================================================

	if (m_bossManager.IsAllBossDefeated())
	{
		// 9番マップへの道を開く
		OpenBossArea();
	}


	//==================================================
	// ラスボス撃破後
	//==================================================

	if (m_bossManager.IsBossDefeated(3))
	{
		if (m_difference[GAME_CLEAR_MAP][GAME_CLEAR_Y][GAME_CLEAR_X] == false)
		{
			MapBreak();
		}

		LastBossDefeated();
	}


	//==================================================
	// 画面端判定
	//
	// m_position はマス座標なので、
	// 画面座標に変換してから Screen と比較する
	//==================================================

	int playerPixelX =
		static_cast<int>(playerManager.m_position.x * m_chipSize);

	int playerPixelY =
		static_cast<int>(playerManager.m_position.y * m_chipSize);

	int playerPixelW =
		playerW * m_chipSize;

	int playerPixelH =
		playerH * m_chipSize;


	if (m_currentMap != 9)
	{
		//==================================================
		// 右端
		//==================================================

		if (
			!m_isTransition &&
			playerPixelX + playerPixelW >= Screen::RIGHT
			)
		{
			m_moveDir = MoveDir::Right;

			int local = m_currentMap - startMap;

			if (local >= 8)
				m_nextmap = m_currentMap - 8;
			else
				m_nextmap = m_currentMap + 2;

			// 9番マップには通常移動できない
			if (m_nextmap == 9)
				m_nextmap = 0;

			m_transition = 0;
			m_isTransition = true;
		}


		//==================================================
		// 左端
		//==================================================

		if (
			!m_isTransition &&
			playerPixelX <= Screen::LEFT
			)
		{
			m_moveDir = MoveDir::Left;

			int local = m_currentMap - startMap;

			if (local < 2)
				m_nextmap = m_currentMap + 8;
			else
				m_nextmap = m_currentMap - 2;

			// 9番マップには通常移動できない
			if (m_nextmap == 9)
				m_nextmap = 8;

			m_transition = 0;
			m_isTransition = true;
		}


		//==================================================
		// 上端
		//==================================================

		if (
			!m_isTransition &&
			playerPixelY <= Screen::TOP
			)
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


		//==================================================
		// 下端
		//==================================================

		if (
			!m_isTransition &&
			playerPixelY + playerPixelH >= Screen::BOTTOM
			)
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
	
	// マップ遷移
	if (m_isTransition)
	{
		//
		m_transition += 16;
		int limit =(m_moveDir == MoveDir::Up ||m_moveDir == MoveDir::Down)? Screen::HEIGHT: Screen::WIDTH;
		if (m_transition >= limit)
		{
			m_currentMap = m_nextmap;
			m_isTransition = false;
			// 新しいマップに入ったときのプレイヤー位置
			// m_position は「マス座標」
			if (m_moveDir == MoveDir::Right)
			{
				playerManager.SetPosition(Vector2(1, playerManager.m_position.y));
			}
			else if (m_moveDir == MoveDir::Left)
			{
				playerManager.SetPosition(Vector2(MAP_WIDTH - playerW-1, playerManager.m_position.y));
			}
			else if (m_moveDir == MoveDir::Up)
			{
				playerManager.SetPosition(Vector2(playerManager.m_position.x, MAP_HEIGHT - playerH-3));
			}
			else if (m_moveDir == MoveDir::Down)
			{
				playerManager.SetPosition(Vector2(playerManager.m_position.x, 1));
			}
			// マス座標 → 画面座標
			playerManager.m_drawPosition.x =playerManager.m_position.x * m_chipSize;
			playerManager.m_drawPosition.y =playerManager.m_position.y * m_chipSize;
			// PlayerMove側も同期
			playerManager.m_playerMove.SetPosition(playerManager);
			m_moveDir = MoveDir::None;
		}
	}

	// 霧
	int px =static_cast<int>(playerManager.m_position.x);
	int py =static_cast<int>(playerManager.m_position.y);
	RevealArea(px, py, 4);
}
void Map::Render()
{
	//m_transitionは「マップ同士の端つながっている位置」「ここからずらしていく」
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

void Map::DrawFog(int offsetX, int offsetY)
{
	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
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
	// マップ0の左上は絶対に崩さない
	if (map == 0)
	{
		if (x >= 0 && x <= 15 &&
			y >= 0 && y <= 9)
		{
			return true;
		}
	}

	// その他の安全エリア
	if (NextMapSearch(map, x, y))
	{
		return true;
	}

	return false;
}
void Map::LastBossDefeated()
{
	m_changeTimer++;

	if (m_changeTimer < m_changeInterval)
		return;

	m_changeTimer = 0;

	int map = m_currentMap;

	// 左側から崩壊
	int leftX = m_leftChangeX[map];

	// 下から徐々に崩壊範囲を広げる
	int leftMinY = m_leftChangeY[map];

	// 現在の範囲からランダム
	int leftY = leftMinY + GetRand(MAP_HEIGHT - leftMinY - 1);

	if (leftX >= 0 && leftX < MAP_WIDTH &&
		leftY >= 0 && leftY < MAP_HEIGHT)
	{
		if (!IsSafeArea(map, leftX, leftY))
		{
			m_basemap[map][leftY][leftX] = TileType::Fall;
			m_workmap[map][leftY][leftX] = 291;
			m_objectmap[map][leftY][leftX] = -1;
		}
	}

	// 崩壊範囲を少しずつ上へ広げる
	if (m_leftChangeY[map] > 0)
	{
		m_leftChangeY[map]--;
	}

	// Xを移動
	m_leftChangeX[map] += m_leftDirection;

	// 端で反転
	if (m_leftChangeX[map] >= MAP_WIDTH - 1)
	{
		m_leftChangeX[map] = MAP_WIDTH - 1;
		m_leftDirection = -1;
	}
	else if (m_leftChangeX[map] <= 0)
	{
		m_leftChangeX[map] = 0;
		m_leftDirection = 1;
	}

	// 右側から崩壊
	int rightX = m_rightChangeX[map];

	// 下から徐々に崩壊範囲を広げる
	int rightMinY = m_rightChangeY[map];

	// 現在の範囲からランダム
	int rightY = rightMinY + GetRand(MAP_HEIGHT - rightMinY - 1);

	if (rightX >= 0 && rightX < MAP_WIDTH &&
		rightY >= 0 && rightY < MAP_HEIGHT)
	{
		if (!IsSafeArea(map, rightX, rightY))
		{
			m_basemap[map][rightY][rightX] = TileType::Fall;
			m_workmap[map][rightY][rightX] = 291;
			m_objectmap[map][rightY][rightX] = -1;
		}
	}

	// 崩壊範囲を少しずつ上へ広げる
	if (m_rightChangeY[map] > 0)
	{
		m_rightChangeY[map]--;
	}

	// Xを移動
	m_rightChangeX[map] += m_rightDirection;

	// 端で反転
	if (m_rightChangeX[map] >= MAP_WIDTH - 1)
	{
		m_rightChangeX[map] = MAP_WIDTH - 1;
		m_rightDirection = -1;
	}
	else if (m_rightChangeX[map] <= 0)
	{
		m_rightChangeX[map] = 0;
		m_rightDirection = 1;
	}
	// 崩壊タイミング
	m_changeInterval = GetRand(m_speedrand) + 25;
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
	int left	= px;
	int right	= (px + width - 1);
	int top		= py;
	int bottom	= (py + height - 1) ;

	return	GetTileType(left, top)		== TileType::Wall ||
			GetTileType(right, top)		== TileType::Wall ||
			GetTileType(left, bottom)	== TileType::Wall ||
			GetTileType(right, bottom)	== TileType::Wall;
}
bool Map::IsTreasureRect(int px, int py, int width, int height) const
{
	int left	= px ;
	int right	= (px + width - 1) ;
	int top		= py ;
	int bottom	= (py + height - 1) ;

	return	GetTileType	(left, top)		== TileType::Treasure ||
			GetTileType	(right, top)	== TileType::Treasure ||
			GetTileType	(left, bottom)	== TileType::Treasure ||
			GetTileType	(right, bottom)	== TileType::Treasure;
}

bool Map::IsNextFloorRect(int px, int py, int width, int height) const
{
	int left	= px ;
	int right	= (px + width - 1) ;
	int top		= py ;
	int bottom	= (py + height - 1) ;

	return	GetTileType(left, top)		== TileType::NextFloor ||
			GetTileType(right, top)		== TileType::NextFloor ||
			GetTileType(left, bottom)	== TileType::NextFloor ||
			GetTileType(right, bottom)	== TileType::NextFloor;
}

bool Map::IsFallRect(int px, int py, int width, int height) const
{
	int left = px ;
	int right = (px + width - 1) ;
	int top = py ;
	int bottom = (py + height - 1) ;

	return	GetTileType(left, top)		== TileType::Fall ||
			GetTileType(right, top)		== TileType::Fall ||
			GetTileType(left, bottom)	== TileType::Fall ||
			GetTileType(right, bottom)	== TileType::Fall;
}

bool Map::IsSignboardRect(int px, int py, int width, int height) const
{
	int left = px ;
	int right = (px + width - 1) ;
	int top = py ;
	int bottom = (py + height - 1) ;

	return	GetTileType(left, top)		== TileType::Signboard ||
			GetTileType(right, top)		== TileType::Signboard ||
			GetTileType(left, bottom)	== TileType::Signboard ||
			GetTileType(right, bottom)	== TileType::Signboard;
}

bool Map::IsSlimeRect(int px, int py, int width, int height) const
{
	int left = px ;
	int right = (px + width - 1) ;
	int top = py ;
	int bottom = (py + height - 1) ;

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

const std::vector<BreakEffectPosition>& Map::GetBreakEffectPositions() const
{
	return m_breakEffectPositions;
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
void Map::BreakArea(
	int centerX,
	int centerY,
	int left,
	int right,
	int top,
	int bottom,
	int targetObject,
	int replaceObject,
	TileType replaceType,
	int dangerAdd)
{
	bool isChanged = false;

	for (int y = top; y <= bottom; ++y)
	{
		for (int x = left; x <= right; ++x)
		{
			int tx;
			int ty;

			if (!PrepareBreakArea(centerX, centerY, x, y, tx, ty))
			{
				continue;
			}
			if (NextMapSearch(m_currentMap,tx,ty))
			{
				continue;
			}
			if (m_objectmap[m_currentMap][ty][tx] == 41)
			{
				continue;
			}
			if (targetObject < 0 ||m_objectmap[m_currentMap][ty][tx] == targetObject)
			{
				// オブジェクトまたは地形が実際に変わる場合だけ
				if (m_objectmap[m_currentMap][ty][tx] != replaceObject ||
					m_basemap[m_currentMap][ty][tx] != replaceType)
				{
					m_objectmap[m_currentMap][ty][tx] = replaceObject;
					m_basemap[m_currentMap][ty][tx] = replaceType;

					isChanged = true;

					// 実際に変更された場所を記録
					m_breakEffectPositions.push_back(BreakEffectPosition{tx * m_chipSize,ty * m_chipSize});
				}
			}
		}
	}

	// 実際に地形が変更された場合だけBreakLevelを増加
	if (isChanged)
	{
		m_breakLevel = std::min(80, m_breakLevel + dangerAdd);
		m_level = m_breakLevel / 20;
	}
}
void Map::BreakAreaByGroup(
	int centerX,
	int centerY,
	int left,
	int right,
	int top,
	int bottom,
	TileGroup group,
	int dangerAdd)
{
	const auto& breakTiles = m_tileRole.GetBreakTiles(group);

	// 今回、本当にマップが変更されたか
	bool isChanged = false;

	for (int y = top; y <= bottom; ++y)
	{
		for (int x = left; x <= right; ++x)
		{
			// 中心座標 + 相対座標
			int tx = centerX + x;
			int ty = centerY + y;

			// マップ外なら無視
			if (tx < 0 || tx >= MAP_WIDTH ||ty < 0 || ty >= MAP_HEIGHT)
			{
				continue;
			}
			if (NextMapSearch(m_currentMap,tx,ty))
			{
				continue;
			}
			int objectNo = m_objectmap[m_currentMap][ty][tx];

			for (const auto& breakTile : breakTiles)
			{
				if (objectNo == breakTile.targetTile)
				{
					// すでに同じものなら変更扱いにしない
					if (m_objectmap[m_currentMap][ty][tx] != breakTile.replaceTile ||
						m_basemap[m_currentMap][ty][tx] != breakTile.replaceType)
					{
						m_objectmap[m_currentMap][ty][tx] =	breakTile.replaceTile;

						m_basemap[m_currentMap][ty][tx] =breakTile.replaceType;

						isChanged = true;

						m_breakEffectPositions.push_back(BreakEffectPosition{tx * m_chipSize,ty * m_chipSize});
					}

					break;
				}
			}
		}
	}

	// 1マスでも実際に変更された場合だけBreakLevel上昇
	if (isChanged)
	{
		m_breakLevel =
			std::min(80, m_breakLevel + dangerAdd);

		m_level = m_breakLevel / 20;
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
bool Map::NextMapSearch(int map, int x, int y)
{
	if (map < 0 || map >= MAP_NUM ||
		x < 0 || x >= MAP_WIDTH ||
		y < 0 || y >= MAP_HEIGHT)
	{
		return false;
	}

	int objectNo = m_beforChange[map][y][x];

	return objectNo == 288 ||
		objectNo == 289 ||
		objectNo == 304 ||
		objectNo == 305 ||
		objectNo == 320 ||
		objectNo == 321 ||
		objectNo == 322 ||
		objectNo == 323 ||
		objectNo == 336 ||
		objectNo == 337 ||
		objectNo == 352 ||
		objectNo == 353 ||
		objectNo == 368 ||
		objectNo == 369;
}

		void Map::UsedTreasure(PlayerManager& player)
		{
			Vector2 pos = player.GetPosition();

			int tx = static_cast<int>(pos.x) ;
			int ty = static_cast<int>(pos.y) ;

			BreakArea(tx, ty, 0, 1, 0, 1, 96, 1, TileType::Floor, 0);//プレイヤー範囲内宝箱を使用済みにする
			BreakArea(tx, ty, 0, 1, 0, 1, 97, 1, TileType::Floor, 0);
		}


		void Map::NormalBreak(PlayerManager& player)
		{
			m_breakEffectPositions.clear();

			Accessory::UpgradeAccessory nomal = m_accessory.GetAccessory(Accessory::ElementType::NOMAL);

			Vector2 pos = player.GetPosition();

			int tx = static_cast<int>(pos.x) ;
			int ty = static_cast<int>(pos.y) ;

			BreakAreaByGroup(tx, ty, -1, 2, -1, 2,TileGroup::Normal,0);
		}

		void Map::FireBreak(PlayerManager& player)
		{
			m_breakEffectPositions.clear();

			Vector2 pos = player.GetPosition();

			int tx = static_cast<int>(pos.x) ;
			int ty = static_cast<int>(pos.y) ;

			BreakAreaByGroup(tx, ty, -2, 3, -2, 3, TileGroup::Fire, 3);


		}

		void Map::WaterBreak(PlayerManager& player)
		{
			m_breakEffectPositions.clear();

			Vector2 pos = player.GetPosition();

			int tx = static_cast<int>(pos.x) ;
			int ty = static_cast<int>(pos.y) ;

			BreakAreaByGroup(tx, ty, -3, 4, -3, 4, TileGroup::Water, 1);

		}

		void Map::GrassBreak(PlayerManager& player)
		{
			m_breakEffectPositions.clear();

			Vector2 pos = player.GetPosition();

			int tx = static_cast<int>(pos.x) ;
			int ty = static_cast<int>(pos.y) ;

			BreakAreaByGroup(tx, ty, -3, 4, -3, 4, TileGroup::Grass, 0);
		}

		void Map::SoilBreak(PlayerManager& player)
		{
			m_breakEffectPositions.clear();

			Vector2 pos = player.GetPosition();

			int tx = static_cast<int>(pos.x) ;
			int ty = static_cast<int>(pos.y) ;

			BreakAreaByGroup(tx, ty, -3, 4, -3, 4, TileGroup::Soil, 5);

		}

		void Map::WindBreak(PlayerManager& player)
		{
			m_breakEffectPositions.clear();

			Vector2 pos = player.GetPosition();

			int tx = static_cast<int>(pos.x) ;
			int ty = static_cast<int>(pos.y) ;

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
			m_breakEffectPositions.clear();

			Vector2 pos = player.GetPosition();

			int tx = static_cast<int>(pos.x) ;
			int ty = static_cast<int>(pos.y) ;

			BreakAreaByGroup(tx, ty, -6, 7, -6, 7, TileGroup::Darkness, 5);

		}

////連携技
		void Map::SteamExplosionBreak(PlayerManager& player)
		{
			m_breakEffectPositions.clear();

			Vector2 pos = player.GetPosition();
			int tx = static_cast<int>(pos.x) ;
			int ty = static_cast<int>(pos.y) ;

			BreakArea(tx, ty, -2, 3, -2, 3, -1, 3, TileType::Floor, 5);
		}

		void Map::FloorBreak(PlayerManager& player)
		{
			m_breakEffectPositions.clear();

			Vector2 pos = player.GetPosition();
			int tx = static_cast<int>(pos.x) ;
			int ty = static_cast<int>(pos.y) ;

			BreakArea(tx , ty, -6, 7, -6, 7, -1, 291, TileType::Fall, 10);
			BreakArea(tx, ty, -1, 7, -1, 2, -1, 1, TileType::Floor, 0);
			BreakArea(tx, ty, -1, 2, -1, 7, -1, 1, TileType::Floor, 0);
			BreakArea(tx, ty, -6, 0, -1, 2, -1, 1, TileType::Floor, 0);
			BreakArea(tx, ty, 0, 2, -6, 0, -1, 1, TileType::Floor, 0);
		}

		void Map::WaterFlowsBreak(PlayerManager& player)
		{
			m_breakEffectPositions.clear();

			for (int y = 0; y < MAP_HEIGHT; ++y)
			{
				for (int x = 0; x < MAP_WIDTH; ++x)
				{
					if (m_objectmap[m_currentMap][y][x] == 291)
					{
						m_objectmap[m_currentMap][y][x] = 134;
						m_basemap[m_currentMap][y][x] = TileType::Wall;
					}
				}
			}
		}

		void Map::GrowGrassBreak(PlayerManager& player)
		{
			m_breakEffectPositions.clear();

			Vector2 pos = player.GetPosition();
			int tx = static_cast<int>(pos.x) ;
			int ty = static_cast<int>(pos.y) ;

			BreakAreaByGroup(tx, ty, -1, 2, -1, 2,TileGroup::GrowGrass, 0);
		}

		void Map::VolcazationBreak(PlayerManager& player)
		{
			m_breakEffectPositions.clear();

			Vector2 pos = player.GetPosition();
			int tx = static_cast<int>(pos.x) ;
			int ty = static_cast<int>(pos.y) ;
			
			BreakAreaByGroup(tx, ty, -2, 3, -2, 3,TileGroup::Volcazation, 10);
		}