#pragma once
class Map
{
public:
	enum class TileType
	{
		Floor,
		Wall,
		Object,
	};
private:
	

	static constexpr int MAP_WIDTH = 40;
	static constexpr int MAP_HEIGHT = 25;
	static constexpr int GH_MAX = 320;

	int m_ghChip[GH_MAX];
	
public:
	static constexpr int MAP_NUM = 40;
	TileType m_basemap[MAP_NUM][MAP_HEIGHT][MAP_WIDTH];//当たり判定用のマップデータ
	int m_workmap[MAP_NUM][MAP_HEIGHT][MAP_WIDTH];//描画用のマップデータ

	int m_currentMap;
	int m_chipSize;
public:
	Map();
	~Map();
	void Initialize(const wchar_t* fileName);
	void Update();
	void Render();
	void Finalize();

	void LoadMapChip(const wchar_t* fileName);

	TileType GetTileType(int x, int y)const;
	int GetTileNo(int x, int y) const;
	void ChangeMap(int mapNo);
	int GetChipSize()const;
	
};

