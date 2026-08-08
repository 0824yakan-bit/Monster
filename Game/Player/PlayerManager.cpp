#include "pch.h"
#include "PlayerManager.h"

#include"Game/ImageManager/ImageManager.h"

PlayerManager::PlayerManager()
	:map(nullptr)
	,m_position{0,0}
	,m_size{0,0}
	,m_invicible{false}
{

}
PlayerManager::~PlayerManager()
{

}




void PlayerManager::Initialize(Map*map)
{
	m_direction = Direction::Right;
	this->map = map;

	m_oldposition.x = 0;
	m_oldposition.y = 0;

	m_size.x = map->m_chipSize*2;
	m_size.y = map->m_chipSize*2;

	 m_position.x = 5*m_size.x;
	 m_position.y = 5*m_size.y;
	playermove.Initialize(map,*this);
}

void PlayerManager::Update(Map*map)
{
	playermove.Update(map,this);

	

}

void PlayerManager::Render(Map* map)
{
	playermove.Render(map,this);
	switch(m_direction)
	{
	case Direction::Up:
		m_image->DrawPlayer2(m_position, m_size);
		break;
	case Direction::Down:
		m_image->DrawPlayer1(m_position, m_size);
		break;
	case Direction::Left:
		m_image->DrawPlayer3(m_position, m_size);
		break;
	case Direction::Right:
		m_image->DrawPlayer4(m_position, m_size);
		break;
	}
	

}

void PlayerManager::Finalize()
{
	playermove.Finalize();

}

Vector2 PlayerManager::GetPosition()
{
	return m_position;
}

int PlayerManager::GetSpeed()//１マス分移動
{
	return M_SPEED;
}
void PlayerManager::SetImage(ImageManager* image)
{
	m_image = image;
}