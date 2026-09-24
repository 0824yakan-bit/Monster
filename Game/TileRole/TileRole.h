#pragma once

enum class TileType
{
	Floor,//通り抜け可能
	Wall,//通り抜け不可
	Object,//通り抜け可能な動作物体
	Treasure,//宝箱
	Lounge,//回復スポット
	GrassLounge,//草で回復
	Signboard,
	NextFloor,
	Fall,//落ちる
	GameClear,//ゲームクリア
};
enum class TileGroup
{
	Normal,
	Fire,
	Water,
	Grass,
	Soil,
	Wind,
	Darkness,
	GrowGrass,
	Volcazation,
};
struct BreakTile
{
	int targetTile;       // 壊す前のタイル番号
	int replaceTile;      // 壊した後のタイル番号
	TileType replaceType; // 壊した後のTileType
};

class TileRole
{
private:
	// タイル番号 → TileType
	std::unordered_map<int, TileType> m_roles;

	// 属性 → 壊すタイル一覧
	std::unordered_map<TileGroup, std::vector<BreakTile>> m_breakGroups;
public:
	TileRole();

	void Initialize();

	TileType GetType(int tileNo) const;
	bool IsWall(int tileNo) const;
	// グループに登録された壊す情報を取得
	const std::vector<BreakTile>&GetBreakTiles(TileGroup group) const;
};

