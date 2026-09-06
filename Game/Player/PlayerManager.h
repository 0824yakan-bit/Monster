#pragma once

#include"Game/Player/PlayerMove.h"
#include"Game/Maths/Vector2.h"

class ImageManager;
class Party;
class PlayerManager
{
private:
	
	PlayerMove m_playerMove;
	Map* map;
	ImageManager* m_image = nullptr;

	static constexpr int M_SPEED = 32;

public:
	Vector2 m_oldposition;//現在から前のポジション
	Vector2 m_position;//現在のポジション
	Vector2 m_currentposition;
	Vector2 m_size;
	enum Direction
	{
		Up,
		Down,
		Left,
		Right,
	};
	Direction m_direction;

	float m_drawScale;
	bool m_invicible;//にげる選択時のみ
public:
	PlayerManager();
	~PlayerManager();

	void Initialize(Map*map);
	void Update(FieldScene* field, Map*map,BossManager*bossManager,Accessory*accessory, Party& party);
	void Render(FieldScene* field,Map* map, Accessory* accessory);
	void Finalize();

	Vector2 GetPosition();
	int GetSpeed();
	void SetImage(ImageManager* image);
	Direction GetDirection()const;
};

