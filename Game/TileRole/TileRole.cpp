#include "pch.h"
#include "Game/TileRole/TileRole.h"

TileRole::TileRole()
	:map{}
	,m_currentTileNo{}
{
}

TileRole::~TileRole()
{
}

void TileRole::Initialize(const Map&map)
{
	m_chipsize = map.GetChipSize();
}

void TileRole::Update(const Map&map,Vector2 position)
{
	int mapX = static_cast<int>(position.x) / m_chipsize;
	int mapY = static_cast<int>(position.y) / m_chipsize;
	m_currentTileNo = map.GetTileNo(mapX,mapY);
}

void TileRole::Render()
{
}

void TileRole::Finalize()
{
}
