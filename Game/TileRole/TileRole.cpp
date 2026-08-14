#include "pch.h"
#include "Game/TileRole/TileRole.h"

TileRole::TileRole()
{
}

void TileRole::Initialize()
{
    // 床
    for (int i = 0; i < 32; ++i)
        m_roles[i] = TileType::Floor;

    // 壁
    for (int i = 32; i <= 80; ++i)
        m_roles[i] = TileType::Wall;

    // 宝箱
    m_roles[96] = TileType::Treasure;

    // 回復
    m_roles[150] = TileType::Lounge;

    // 階段
    m_roles[200] = TileType::NextFloor;
}

TileType TileRole::GetType(int tileNo) const
{
    auto it = m_roles.find(tileNo);

    if (it != m_roles.end())
        return it->second;

    return TileType::Object;
}

bool TileRole::IsWall(int tileNo) const
{
    return GetType(tileNo) == TileType::Wall;
}