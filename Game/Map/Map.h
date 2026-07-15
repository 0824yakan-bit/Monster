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
	static constexpr int GH_MAX = 320;

	int m_ghChip[GH_MAX];

	bool m_isTransition;
	int m_nextmap;
	int m_transition;
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
	void Update(InputManager&inputManager,PlayerManager&playerManager);
	void Render();
	void Finalize();

	void LoadMapChip(const wchar_t* fileName, int mapData[MAP_NUM][MAP_HEIGHT][MAP_WIDTH]);

	TileType GetTileType(int x, int y)const;
	int GetTileNo(int x, int y) const;
	void ChangeMap(int mapNo);
	int GetChipSize()const;
	

	void DrawCurrentMap(int offsetX,int offsetY);
	void DrawNextMap(int offsetX,int offsetY);
};

