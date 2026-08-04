#pragma once
class InputManager;
class PlayerManager;
class Map
{
public:
	enum class TileType
	{
		Floor,
		Wall,
		Object,
		NextFloor
	};
	enum class MoveDir
	{
		None,
		Left,
		Right,
		Up,
		Down,
	};

	MoveDir m_moveDir;
private:
	static constexpr int MAP_WIDTH = 40;
	static constexpr int MAP_HEIGHT = 25;
	static constexpr int GH_MAX = 384;//24*16

	int m_ghChip[GH_MAX];

	bool m_isTransition;
	int m_nextmap;
	int m_transition;

private:
	int m_stageNo = 0;   // 0=ステージ1, 1=ステージ2 ...
	static constexpr int MAPS_PER_STAGE = 10;

	void BreakArea(int centerX, int centerY,int left, int right,int top, int bottom,int targetObject,int replaceObject,TileType replaceType);
	

public:
	static constexpr int MAP_NUM = 40;
	TileType m_basemap[MAP_NUM][MAP_HEIGHT][MAP_WIDTH];//当たり判定用のマップデータ
	int m_workmap[MAP_NUM][MAP_HEIGHT][MAP_WIDTH];//描画用のマップデータ
	int m_objectmap[MAP_NUM][MAP_HEIGHT][MAP_WIDTH];//m_workmapの上から物体を表示



	int m_currentMap;
	int m_chipSize;
public:
	Map();
	~Map();
	void Initialize(const wchar_t* fileName);
	void Update(InputManager& inputManager, PlayerManager& playerManager);
	void Render();
	void Finalize();

	void LoadMapChip(const wchar_t* fileName, int mapData[MAP_NUM][MAP_HEIGHT][MAP_WIDTH]);

	TileType GetTileType(int x, int y)const;
	int GetTileNo(int x, int y) const;
	void ChangeMap(int mapNo);
	int GetChipSize()const;
	int GetCurrentMap()const;

	int GetStageStartMap() const;//階層の中の１エリア目

	int GetStageEndMap() const;//階層の中の１０エリア目

	void DrawCurrentMap(int offsetX, int offsetY);
	void DrawNextMap(int offsetX, int offsetY);

	void ChangeStage(int stageNo);//階層変更関数

	//地形破壊

	//属性単体
	void NormalBreak (PlayerManager& player);//無属性
	void FireBreak   (PlayerManager& player);//火属性
	void WaterBreak  (PlayerManager& player);//水属性
	void GrassBreak  (PlayerManager& player);//草属性
	void SoilBreak   (PlayerManager& player);//土属性
	void WindBreak   (PlayerManager& player);//風属性
	void ThunderBreak(PlayerManager& player);//雷属性

	//複合属性
	void SteamExplosionBreak(PlayerManager& player);//火＋水
	void FloorBreak(PlayerManager& player);//水＋土
	void WaterFlowsBreak(PlayerManager& player);//水＋風
	void GrowGrassBreak(PlayerManager& player);//水＋草
	void VolcazationBreak(PlayerManager& player);//土＋火

};

