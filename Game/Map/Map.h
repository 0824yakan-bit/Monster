#pragma once

#include "Game/Party/Accessory.h"
#include "Game/TileRole/TileRole.h"

class InputManager;
class PlayerManager;
class BossManager;

class Map
{
public:
	// マップ移動方向
	enum class MoveDir
	{
		None,
		Left,
		Right,
		Up,
		Down,
	};

	MoveDir m_moveDir;


public:
	// マップサイズ
	static constexpr int MAP_WIDTH	= 40;
	static constexpr int MAP_HEIGHT = 25;
private:
	static constexpr int GH_MAX		= 384;	// 24 * 16

	// マップ移動
	bool m_isTransition;
	int	 m_nextmap;
	int	 m_transition;

	Accessory	& m_accessory;
	BossManager	& m_bossManager;

	// タイル情報
	TileRole m_tileRole;

	// ステージ情報
	int m_stageNo = 0;					// 0 = ステージ1、1 = ステージ2 ...
	static constexpr int MAPS_PER_STAGE = 10;

	//エリア開放
	bool m_isbossAreaOpen;

	//ボスエリアまでの階段の座標
	static constexpr int X_LEFT		= 20;//左
	static constexpr int X_RIGHT	= 21;//右
	static constexpr int Y_TOP		= 10;//上
	static constexpr int Y_BOTTOM	= 11;//下

	// ブレイク情報
	int dangerAdd = 1;
	int m_breakLevel;	// 地形破壊回数(ブレイクカウント)
	int m_level;		//ブレイクレベル

	//段々崩れる
	int m_changeX;
	int m_changeY;
	int m_changeMap;

	int m_changeTimer;
	static const int CHANGE_INTERVAL = 5;

	// グラフィックハンドル
	int m_ghChip[GH_MAX];

	// 地形破壊
	bool PrepareBreakArea(int centerX, int centerY, int x, int y, int& tx, int& ty);
	void BreakArea(
		int centerX,int centerY,			//中心
		int left,int right,					//左右
		int top,int bottom,					//上下
		int targetObject,int replaceObject,	//元チップ:変更後チップ
		TileType replaceType,int dangerAdd);//チップの状態判定変更:ブレイクカウント追加	// 指定範囲の地形を破壊・変更
	void BreakAreaByGroup(int centerX, int centerY, int left, int right, int top, int bottom, TileGroup group, int dangerAdd);

public:
	// マップデータ
	static constexpr int GAME_CLEAR_MAP = 0;
	static constexpr int GAME_CLEAR_X = 2;
	static constexpr int GAME_CLEAR_Y = 2;

	static constexpr int MAP_NUM = 10;

	TileType	m_basemap	[MAP_NUM][MAP_HEIGHT][MAP_WIDTH];// 当たり判定用
	int			m_workmap	[MAP_NUM][MAP_HEIGHT][MAP_WIDTH];// 地面描画用
	int			m_objectmap	[MAP_NUM][MAP_HEIGHT][MAP_WIDTH];// オブジェクト描画用
	bool		m_fog		[MAP_NUM][MAP_HEIGHT][MAP_WIDTH];// 霧の有無

	//ラスボス撃破時使用
	int		m_beforChange	[MAP_NUM][MAP_HEIGHT][MAP_WIDTH];//csv書き換え前
	int		m_afterChange	[MAP_NUM][MAP_HEIGHT][MAP_WIDTH];//csv書き換え後
	bool	m_difference	[MAP_NUM][MAP_HEIGHT][MAP_WIDTH];//変更されていた場所だけTRUE

	int m_fogdensity;	// 霧濃度：薄い0 ～ 濃い400

	int m_currentMap;	// 現在のマップ番号
	int m_chipSize;		// マップチップのサイズ

public:
	// コンストラクタ・デストラクタ
	Map(Accessory& accessory,BossManager&bossManager);
	~Map();

	// 初期化・更新・描画
	void Initialize	(const wchar_t* fileName);	// マップの初期化
	void Update		(InputManager& inputManager, PlayerManager& playerManager);	// マップの更新
	void Render		();							// マップの描画
	void Finalize	();							// マップの終了処理

	// マップデータ読み込み
	void LoadMapChip(const wchar_t* fileName,int mapData[MAP_NUM][MAP_HEIGHT][MAP_WIDTH]);	// CSVからマップデータを読み込む

	//ラスボス撃破後
	void LastBossDefeated();//ラスボス撃破後常に呼ぶ
	void MapBreak();//撃破後一回だけ呼ぶ

	// 当たり判定
	bool IsWallRect		(int px,int py,int width,int height) const;	// 壁との当たり判定
	bool IsTreasureRect(int px, int py, int width, int height) const;	// 宝箱との当たり判定
	bool IsNextFloorRect	(int px,int py,int width,int height) const;	// 階段との当たり判定
	bool IsFallRect(int px, int py, int width, int height) const;	// 穴との当たり判定
	bool IsSignboardRect(int px, int py, int width, int height) const;	// 看板との当たり判定
	bool IsSlimeRect(int px, int py, int width, int height) const;	// 看板との当たり判定
	

	TileType GetTileType	(int x, int y) const;	// 指定座標のタイル種類を取得
	int		 GetTileNo		(int x, int y) const;	// 指定座標のオブジェクト番号を取得

	// マップ移動
	void	ChangeMap		(int mapNo);	// マップを変更
	int		GetChipSize		() const;		// マップチップサイズを取得
	int		GetCurrentMap	() const;		// 現在のマップ番号を取得

	// ステージ管理
	int		GetStageStartMap() const;		// ステージ内の1エリア目を取得
	int		GetStageEndMap	() const;		// ステージ内の10エリア目を取得
	void	ChangeStage		(int stageNo);	// ステージを変更

	//エリア開放
	void	OpenBossArea	();				//ラスボスエリア開放
	//エリア移動
	void	EnterBossArea	();				//ラスボスエリア侵入
	void	ExitBossArea	();				//ラスボスエリア脱出

	// マップ描画
	void	DrawCurrentMap	(int offsetX, int offsetY);		// 現在のマップを描画
	void	DrawNextMap		(int offsetX, int offsetY);		// 次のマップを描画

	// ブレイクレベル
	int	GetBreakLevel() const;	// 地形破壊回数を取得

	// 霧
	void RevealArea(int centerX,int centerY,int radius);	// 指定範囲の霧を晴らす

	// 宝箱
	void UsedTreasure		(PlayerManager& player);	// 宝箱を使用済みにする

	// 属性単体
	void NormalBreak		(PlayerManager& player);// 無属性の地形破壊
	void FireBreak			(PlayerManager& player);// 火属性の地形破壊
	void WaterBreak			(PlayerManager& player);// 水属性の地形破壊
	void GrassBreak			(PlayerManager& player);// 草属性の地形破壊
	void SoilBreak			(PlayerManager& player);// 土属性の地形破壊
	void WindBreak			(PlayerManager& player);// 風属性の地形破壊
	void DarknessBreak		(PlayerManager& player);// 闇属性の地形破壊

	// 複合属性
	void SteamExplosionBreak(PlayerManager& player);// 火＋水
	void FloorBreak			(PlayerManager& player);// 水＋土
	void WaterFlowsBreak	(PlayerManager& player);// 水＋風
	void GrowGrassBreak		(PlayerManager& player);// 水＋草
	void VolcazationBreak	(PlayerManager& player);// 土＋火
};