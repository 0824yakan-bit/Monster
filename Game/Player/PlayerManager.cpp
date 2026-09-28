#include "pch.h"
#include "PlayerManager.h"

#include "Game/ImageManager/ImageManager.h"

PlayerManager::PlayerManager()
    : map{}
    , m_playerMove{}
    , m_position{ 0,0 }
    , m_oldposition{ 0,0 }
    , m_drawPosition{ 0,0 }
    , m_size{ 0,0 }
    , m_drawSize{ 0,0 }
    , m_drawScale{ 1.0f }
    , m_invicible{ false }
    , m_direction{}
{
}

PlayerManager::~PlayerManager()
{
}

void PlayerManager::Initialize(Map* map)
{
    m_direction = Direction::Right;

    this->map = map;

    // 配列上の位置
    m_position.x = 3;
    m_position.y = 6;

    m_oldposition = m_position;

    // プレイヤーのサイズ（2×2マス）
    m_size = { 2, 2 };

    // 画面上での表示サイズ
    m_drawSize.x = m_size.x * map->m_chipSize;
    m_drawSize.y = m_size.y * map->m_chipSize;

    // 配列位置 → 画面位置へ変換
    m_drawPosition.x = m_position.x * map->m_chipSize;
    m_drawPosition.y = m_position.y * map->m_chipSize;

    m_drawScale = 1.0f;

    m_playerMove.Initialize(map, *this);
}

void PlayerManager::Update(
    FieldScene* field,
    Map* map,
    BossManager* bossManager,
    Accessory* accessory,
    Party& party)
{
    m_playerMove.Update(
        field,
        map,
        this,
        bossManager,
        accessory,
        &party
    );
}

void PlayerManager::Render(
    FieldScene* field,
    Map* map,
    Accessory* accessory)
{
    Vector2 drawPosition = m_drawPosition;

    // 通常時は2×2マス分のサイズ
    Vector2 drawSize = m_drawSize;

    // 落下中だけ縮小
    drawSize.x = static_cast<int>(m_drawSize.x * m_drawScale);
    drawSize.y = static_cast<int>(m_drawSize.y * m_drawScale);

    // 中央を維持したまま縮小
    drawPosition.x += (m_drawSize.x - drawSize.x) / 2;
    drawPosition.y += (m_drawSize.y - drawSize.y) / 2;

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

int PlayerManager::GetSpeed()
{
    // 1マス分の移動速度
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

void PlayerManager::SetPosition(Vector2 position)
{
    m_position = position;

    // マス座標 → 画面座標
    m_drawPosition.x =m_position.x * map->m_chipSize;

    m_drawPosition.y =m_position.y * map->m_chipSize;

    m_playerMove.SetPosition(*this);
}

void PlayerManager::SetPosition(int x, int y)
{
    SetPosition(Vector2{ x,y });
}
