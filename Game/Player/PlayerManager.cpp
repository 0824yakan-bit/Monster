#include "pch.h"
#include "PlayerManager.h"

#include"Game/ImageManager/ImageManager.h"

PlayerManager::PlayerManager()
	:map{}
	,m_playerMove{}
	,m_position{0,0}
	,m_size{0,0}
	,m_invicible{false}
	,m_direction{}
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

	m_size.x = map->m_chipSize*2;//＊２はプレイヤーのサイズ拡大率
	m_size.y = map->m_chipSize*2;

	m_position.x = 3*map->m_chipSize;
	m_position.y = 6*map->m_chipSize;

	m_drawScale = 1.0f;

	m_playerMove.Initialize(map,*this);

}

void PlayerManager::Update(FieldScene* field, Map*map,Accessory*accessory,Party&party)
{

	m_playerMove.Update(field, map,this,accessory,&party);

}

void PlayerManager::Render(FieldScene* field, Map* map, Accessory* accessory)
{
    Vector2 drawPosition = m_position;
    Vector2 drawSize = m_size;

    // 落下中だけ縮小
    drawSize.x = static_cast<int>(m_size.x * m_drawScale);
    drawSize.y = static_cast<int>(m_size.y * m_drawScale);

    // 中央を維持したまま縮小
    drawPosition.x += (m_size.x - drawSize.x) / 2;
    drawPosition.y += (m_size.y - drawSize.y) / 2;

    switch (m_direction)
    {
    case Direction::Up:
        m_image->DrawPlayer2(drawPosition, drawSize);
        break;

    case Direction::Down:
        m_image->DrawPlayer1(drawPosition, drawSize);
        break;

    case Direction::Left:
        m_image->DrawPlayer3(drawPosition, drawSize);
        break;

    case Direction::Right:
        m_image->DrawPlayer4(drawPosition, drawSize);
        break;
    }

    m_playerMove.Render(field, map, this, accessory);
}

void PlayerManager::Finalize()
{

	m_playerMove.Finalize();

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

PlayerManager::Direction PlayerManager::GetDirection() const
{
	return m_direction;
}
