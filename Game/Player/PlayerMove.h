#pragma once
#include"Game/Maths/Vector2.h"
#include"Game/InputManager/InputManager.h"
#include"Game/Map/Map.h"
#include"Game/TileRole/TileRole.h"
class PlayerManager;
class PlayerMove
{
private:
	Vector2 m_maponposition;
	Map::TileType type;
	Map::TileType nexttile;
	InputManager m_inputManager;
	TileRole m_tileRole;

	int m_mapX;
	int m_mapY;
	int m_nextmapX;
	int m_nextmapY;

	int m_speed;
	int m_movetimer;
	int m_chipsize;
public:

public:
	PlayerMove();
	~PlayerMove();

	void Initialize(Map*map,PlayerManager& palayermanager);
	void Update(Map*map, PlayerManager* playermanager);
	void Render(Map*map, PlayerManager* playermanager);
	void Finalize();
};

