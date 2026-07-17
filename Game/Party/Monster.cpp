#include "pch.h"
#include "Game/Party/Monster.h"

Monster::Monster(Type type)
    : m_type(type)
    , m_hp(100)
    , m_level(1)
{
    switch (type)
    {
    case Type::Slime:
        m_name= L"スライム" ;
        m_attacks.push_back({ L"たいあたり", 1});
        m_attacks.push_back({ L"スライム液", 5 });
        break;

    case Type::Wolf:
        m_name = L"ウルフ";
        m_attacks.push_back({ L"かみつく", 2 });
        m_attacks.push_back({ L"ひっかく", 5 });
        break;

    case Type::Dragon:
        m_name = L"ドラゴン";
        m_attacks.push_back({ L"ひのこ", 20 });
        m_attacks.push_back({ L"じならし",15 });
        break;
    }
}

const std::vector<Attack>& Monster::GetAttacks() const
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

