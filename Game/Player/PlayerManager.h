#pragma once

#include"Game/Player/PlayerMove.h"
#include"Game/Maths/Vector2.h"

class ImageManager;
class Party;
class PlayerManager
{
private:
	
	Map* map;
	ImageManager* m_image = nullptr;

	static constexpr int M_SPEED = 32;

public:
	PlayerMove m_playerMove;

	Vector2 m_oldposition;//現在から前のポジション
	Vector2 m_position;//現在のポジション
	Vector2 m_drawPosition;//表示用ポジション
	Vector2 m_currentposition;
	Vector2 m_size;
	Vector2 m_drawSize;
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
	void SetPosition(Vector2 position);
	void SetPosition(int x, int y);
};

