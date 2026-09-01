#pragma once
#include"Game/Maths/Vector2.h"
#include"Game/InputManager/InputManager.h"
#include"Game/Map/Map.h"
#include"Game/TileRole/TileRole.h"
#include"Game/Party/Party.h"
class FieldScene;
class PlayerManager;
class Battle;
class PlayerMove
{
private:
	Vector2 m_maponposition;
	TileType m_type;
	TileType m_nexttile;
	InputManager m_inputManager;
	TileRole m_tileRole;

	int m_mapX;
	int m_mapY;
	int m_nextmapX;
	int m_nextmapY;

	int m_speed;
	int m_movetimer;
	int m_chipsize;

	bool m_hitTreasure;
	bool m_hitFall;
	bool m_hitNextFloor;
	bool m_hitSignboard;
	bool m_hitSlime;

	bool m_isFalling;
	float m_fallScale;
	int m_fallTimer;
	Vector2 m_fallPosition;
public:

public:
	PlayerMove();
	~PlayerMove();

	void Initialize(Map*map,PlayerManager& palayermanager);
	void Update(FieldScene* field,Map*map, PlayerManager* playermanager,Accessory*accessory,Party*party);
	void Render(FieldScene* field, Map*map, PlayerManager* playermanager,Accessory*accessory);
	void Finalize();

	void ReductionPlayer(PlayerManager& playermanager, Map* map);
	Vector2 FindSafePosition(Map* map, PlayerManager& playermanager);
};

