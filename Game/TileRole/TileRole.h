#pragma once
#include"Game/Map/Map.h"
#include"Game/Maths/Vector2.h"
class TileRole
{
private:
	const Map& map;

	int   m_chipsize;
	float m_currentTileNo;
public:
	TileRole();
		~TileRole();

	void Initialize(const Map&map);
	void Update(const Map&map,Vector2 position);
	void Render();
	void Finalize();
};

