#pragma once

enum class TileType
{
	Floor,//通り抜け可能
	Wall,//通り抜け不可
	Object,//通り抜け可能な動作物体
	Treasure,//宝箱
	Lounge,//回復スポット
	NextFloor
};
class TileRole
{
public:
	TileRole();

	void Initialize();

	TileType GetType(int tileNo) const;
	bool IsWall(int tileNo) const;

private:
	std::unordered_map<int, TileType> m_roles;
};

