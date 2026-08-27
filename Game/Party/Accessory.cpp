#include "pch.h"
#include "Game/Party/Accessory.h"

Accessory::Accessory()
{
}

Accessory::~Accessory()
{
}

void Accessory::Initialize()
{
    m_accessory[ElementType::NOMAL] =
    {
        ElementType::NOMAL,
        1,
        1,
        0,
        0,
        1,
        1
    };

    m_accessory[ElementType::FIRE] =
    {
        ElementType::FIRE,
        1,
        1,
        5,
        5,
        5,
        5
    };

    m_accessory[ElementType::WATER] =
    {
        ElementType::WATER,
        1,
        1,
        1,
        1,
        1,
        1
    };

    m_accessory[ElementType::GRASS] =
    {
        ElementType::GRASS,
        1,
        1,
        5,
        5,
        5,
        5
    };

    m_accessory[ElementType::SOIL] =
    {
        ElementType::SOIL,
        1,
        1,
        2,
        2,
        2,
        2
    };

    m_accessory[ElementType::THUNDER] =
    {
        ElementType::THUNDER,
        1,
        1,
        10,
        10,
        10,
        10
    };

    m_accessory[ElementType::WIND] =
    {
        ElementType::WIND,
        1,
        1,
        6,
        6,
        6,
        6
    };
}

void Accessory::Upgrade(ElementType type)
{
    auto& accessory = m_accessory[static_cast<int>(type)];

    accessory.level++;
    accessory.damage++;

    accessory.top++;
    accessory.under++;
    accessory.left++;
    accessory.right++;
}

Accessory::UpgradeAccessory Accessory::GetAccessory(ElementType type)const
{
    return m_accessory[type];
}

const wchar_t* Accessory::GetElementName(ElementType type)
{
    switch (type)
    {
    case ElementType::NOMAL:    return L"NORMAL";
    case ElementType::FIRE:     return L"FIRE";
    case ElementType::WATER:    return L"WATER";
    case ElementType::GRASS:    return L"GRASS";
    case ElementType::SOIL:     return L"SOIL";
    case ElementType::WIND:     return L"WIND";
    case ElementType::THUNDER:  return L"THUNDER";
    }

    return L"UNKNOWN";
}
