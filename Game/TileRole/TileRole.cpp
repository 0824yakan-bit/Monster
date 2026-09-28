#include "pch.h"
#include "Game/TileRole/TileRole.h"

TileRole::TileRole()
{
}

void TileRole::Initialize()////タイル効果未追加
{
    // 床
    for (int i = 0; i <= 31; ++i)
        m_roles[i] = TileType::Floor;

    // 壁
    m_roles[9] = TileType::Wall;
    m_roles[10] = TileType::Wall;
    for (int i = 16;i <= 26;i++)
        m_roles[i] = TileType::Wall;
    for (int i = 32; i <= 79; ++i)
        m_roles[i] = TileType::Wall;
    for(int i = 102;i<=207;++i)
        m_roles[i] = TileType::Wall;
    for(int i = 256;i<=263;i++)
        m_roles[i] = TileType::Wall;
    for (int i = 272;i <= 279;i++)
        m_roles[i] = TileType::Wall;

    m_roles[22] = TileType::Floor;
    // 宝箱
    m_roles[96] = TileType::Treasure;
    m_roles[97] = TileType::Treasure;

    // 回復
    for(int i= 86;i<=90;++i)
    m_roles[i] = TileType::Lounge;

    // 階段
    m_roles[288] = TileType::NextFloor;
    m_roles[289] = TileType::NextFloor;
    m_roles[304] = TileType::NextFloor;
    m_roles[305] = TileType::NextFloor;
    m_roles[320] = TileType::NextFloor;
    m_roles[321] = TileType::NextFloor;
    m_roles[322] = TileType::NextFloor;
    m_roles[323] = TileType::NextFloor;
    m_roles[336] = TileType::NextFloor;
    m_roles[337] = TileType::NextFloor;
    m_roles[352] = TileType::NextFloor;
    m_roles[353] = TileType::NextFloor;
    m_roles[368] = TileType::NextFloor;
    m_roles[369] = TileType::NextFloor;

    //穴
    m_roles[291] = TileType::Fall;
    //看板
    m_roles[292] = TileType::Signboard;


//属性とオブジェクトごとの変化・・・{変更前,変更後,タイルの状態}
    //無
    m_breakGroups[TileGroup::Normal] =
    {
        {256,-1,TileType::Floor},//壁256
        {272,-1,TileType::Floor},//壁272
    };

    // 火
    m_breakGroups[TileGroup::Fire] =
    {
        {57, -1, TileType::Floor},//枯れ木57
    };

    // 水
    m_breakGroups[TileGroup::Water] =
    {
        {291, 134, TileType::Wall},//穴291→水134
        {134, 3, TileType::Floor},//水134→土3
    };

    // 草
    m_breakGroups[TileGroup::Grass] =
    {
        {3,1,TileType::Floor},//土3→草1
        {4,1,TileType::Floor},//土4→草1
    };

    // 土
    m_breakGroups[TileGroup::Soil] =
    {
        {134,4,TileType::Floor},//水134→土4
    };

    // 風
    m_breakGroups[TileGroup::Wind] =
    {
        {40,57,TileType::Wall},//木40→枯れ木57
        {102,-1,TileType::Floor},//落ち葉102
    };
    m_breakGroups[TileGroup::Darkness] =
    {
        {41,-1,TileType::Floor},//コワセナイカベ41
    };
    m_breakGroups[TileGroup::GrowGrass] =
    {
        {1,91,TileType::GrassLounge},
    };
    m_breakGroups[TileGroup::Volcazation] =
    {
        {32,3,TileType::Floor},
    };
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

const std::vector<BreakTile>&TileRole::GetBreakTiles(TileGroup group) const
{
    static const std::vector<BreakTile> empty;

    auto it = m_breakGroups.find(group);

    if (it != m_breakGroups.end())
        return it->second;

    return empty;
}