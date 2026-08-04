#include "pch.h"
#include "Game/Party/Monster.h"

Monster::Monster(Type type)
    :characteRistics{}
    , m_type{type}
    , m_hitpoint{ 0 }
    , m_level{1}
{
    switch (type)
    {
    case Type::Slime:
        m_name= L"スライム" ;
        m_hitpoint = 20;
        m_attacks.push_back({ L"たいあたり", 1,CharacteRistics::Normal });
        m_attacks.push_back({ L"スライム液", 5,CharacteRistics::Water });
        m_attacks.push_back({ L"ぼうぎょ"  ,10,CharacteRistics::Defense });
        break;

    case Type::Wolf:
        m_name = L"ウルフ";
        m_hitpoint = 30;
        m_attacks.push_back({ L"かみつく", 2 ,CharacteRistics::Normal });
        m_attacks.push_back({ L"かぜおこし", 5 ,CharacteRistics::Wind });
        m_attacks.push_back({ L"ぼうぎょ"  ,10,CharacteRistics::Defense });

        break;

    case Type::Dragon:
        m_name = L"ドラゴン";
        m_hitpoint = 50;
        m_attacks.push_back({ L"ひのこ",2 ,CharacteRistics::Fire });
        m_attacks.push_back({ L"じならし",15 ,CharacteRistics::Soil });
        m_attacks.push_back({ L"ぼうぎょ"  ,10,CharacteRistics::Defense });

        break;

    case Type::Golem:
        m_name = L"ゴーレム";
        m_hitpoint = 50;
        m_attacks.push_back({ L"とっしん",2 ,CharacteRistics::Fire });
        m_attacks.push_back({ L"じならし",15 ,CharacteRistics::Soil });
        m_attacks.push_back({ L"ぼうぎょ"  ,10,CharacteRistics::Defense });

        break;

    case Type::Fairy:
        m_name = L"フェアリー";
        m_hitpoint = 50;
        m_attacks.push_back({ L"とっしん",2 ,CharacteRistics::Fire });
        m_attacks.push_back({ L"じならし",15 ,CharacteRistics::Soil });
        m_attacks.push_back({ L"ぼうぎょ"  ,10,CharacteRistics::Defense });

    }
}

const std::vector<Monster::Attack>& Monster::GetAttacks() const
{
    return m_attacks;
}

Monster::Type Monster::GetType() const
{
    return m_type;
}

std::wstring Monster::GetName() const
{
    return m_name;
}

int Monster::GetHitPoint() const
{
    return m_hitpoint;
}

