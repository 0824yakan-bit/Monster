#include "pch.h"
#include "PlayerManager.h"


PlayerManager::PlayerManager()
	:map(nullptr)
	,m_position{0,0}
	,m_size{0}
	,m_invicible{false}
{

}
PlayerManager::~PlayerManager()
{

}




void PlayerManager::Initialize(Map*map)
{
	this->map = map;

	m_oldposition.x = 0;
	m_oldposition.y = 0;

	 m_size = map->m_chipSize;

	 m_position.x = 12*m_size;
	 m_position.y = 10*m_size;
	playermove.Initialize(map,*this);
}

void PlayerManager::Update(Map*map)
{
	playermove.Update(map,this);

	

}

void PlayerManager::Render(Map* map)
{
	playermove.Render(map,this);


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