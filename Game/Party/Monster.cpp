#include "pch.h"
#include "Game/Party/Monster.h"

Monster::Monster(Type type)
    :characteRistics{}
    , m_type{type}
    , m_maxHitPoint{ 0 }
    , m_level{1}
{
    switch (type)
    {
    case Type::Slime:
        m_name= L"スライム" ;
        m_maxHitPoint = 20;
        m_currentHitPoint = m_maxHitPoint;
        m_attacks.push_back({ L"たいあたり", 1,CharacteRistics::Normal });
        m_attacks.push_back({ L"スライム液", 5,CharacteRistics::Water });
        m_attacks.push_back({ L"ぼうぎょ"  ,10,CharacteRistics::Defense });
        break;

    case Type::Wolf:
        m_name = L"ウルフ";
        m_maxHitPoint = 30;
        m_currentHitPoint = m_maxHitPoint;
        m_attacks.push_back({ L"かみつく", 2 ,CharacteRistics::Normal });
        m_attacks.push_back({ L"かぜおこし", 5 ,CharacteRistics::Wind });
        m_attacks.push_back({ L"ぼうぎょ"  ,10,CharacteRistics::Defense });

        break;

    case Type::Dragon:
        m_name = L"ドラゴン";
        m_maxHitPoint = 50;
        m_currentHitPoint = m_maxHitPoint;
        m_attacks.push_back({ L"ひのこ",2 ,CharacteRistics::Fire });
        m_attacks.push_back({ L"じならし",3 ,CharacteRistics::Soil });
        m_attacks.push_back({ L"ぼうぎょ"  ,10,CharacteRistics::Defense });

        break;

    case Type::Golem:
        m_name = L"ゴーレム";
        m_maxHitPoint = 50;
        m_currentHitPoint = m_maxHitPoint;
        m_attacks.push_back({ L"とっしん",2 ,CharacteRistics::Fire });
        m_attacks.push_back({ L"じならし",15 ,CharacteRistics::Soil });
        m_attacks.push_back({ L"ぼうぎょ"  ,10,CharacteRistics::Defense });

        break;

    case Type::Fairy:
        m_name = L"フェアリー";
        m_maxHitPoint = 50;
        m_currentHitPoint = m_maxHitPoint;
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

int Monster::GetCurrentHitPoint() const
{
    return m_currentHitPoint;
}

void Monster::Damage(int value)
{
    m_currentHitPoint -= value;
    if (m_currentHitPoint < 0) m_currentHitPoint = 0;
}

