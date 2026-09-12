#include "pch.h"
#include "Game/Party/Monster.h"

Monster::Monster(Type type)
    :characteRistics{}
    , m_type{type}
    , m_maxHitPoint{ 0 }
    , m_currentHitPoint{ 0 }
    , m_level{1}
{
    switch (type)
    {
    case Type::Slime:
        m_name= L"スライム" ;
        m_maxHitPoint = 20;
        m_currentHitPoint = m_maxHitPoint;
        m_attacks.push_back({ L"たいあたり", 8,CharacteRistics::Normal });
        m_attacks.push_back({ L"スライム液", 5,CharacteRistics::Water });
        m_attacks.push_back({ L"ぼうぎょ"  ,10,CharacteRistics::Defense });
        break;

    case Type::Wolf:
        m_name = L"ガル";
        m_maxHitPoint = 30;
        m_currentHitPoint = m_maxHitPoint;
        m_attacks.push_back({ L"かみつく", 6 ,CharacteRistics::Normal });
        m_attacks.push_back({ L"かぜおこし", 4 ,CharacteRistics::Wind });
        m_attacks.push_back({ L"ぼうぎょ"  ,10,CharacteRistics::Defense });
        break;

    case Type::Fairy:
        m_name = L"リーフ";
        m_maxHitPoint = 30;
        m_currentHitPoint = m_maxHitPoint;
        m_attacks.push_back({ L"リーフスラッシュ",2 ,CharacteRistics::Grass});
        m_attacks.push_back({ L"フラワーヒール",10 ,CharacteRistics::GrawGrass});
        m_attacks.push_back({ L"ぼうぎょ"  ,10,CharacteRistics::Defense });
        break;

    case Type::Turtle:
        m_name = L"タート";
        m_maxHitPoint = 50;
        m_currentHitPoint = m_maxHitPoint;
        m_attacks.push_back({ L"マッドブラスト",10 ,CharacteRistics::Soil });
        m_attacks.push_back({ L"アクアショット",5 ,CharacteRistics::Water });
        m_attacks.push_back({ L"ぼうぎょ",20,CharacteRistics::Defense });
        break;

    case Type::Mole:
        m_name = L"モグラード";
        m_maxHitPoint = 20;
        m_currentHitPoint = m_maxHitPoint;
        m_attacks.push_back({ L"とっしん",10,CharacteRistics::Normal });
        m_attacks.push_back({ L"じならし",15,CharacteRistics::Soil });
        m_attacks.push_back({ L"ぼうぎょ",10,CharacteRistics::Defense });
        break;

    case Type::Fox:
        m_name = L"ホムラ";
        m_maxHitPoint = 30;
        m_currentHitPoint = m_maxHitPoint;
        m_attacks.push_back({ L"ファイアブレス",10 ,CharacteRistics::Fire });
        m_attacks.push_back({ L"たいあたり",20 ,CharacteRistics::Normal });
        m_attacks.push_back({ L"ぼうぎょ",10,CharacteRistics::Defense });
        break;

    case Type::Golem:
        m_name = L"ゴーレム";
        m_maxHitPoint = 50;
        m_currentHitPoint = m_maxHitPoint;
        m_attacks.push_back({ L"ロックバレット",8 ,CharacteRistics::Soil });
        m_attacks.push_back({ L"しぜんのいかり",15 ,CharacteRistics::Grass});
        m_attacks.push_back({ L"ぼうぎょ",10,CharacteRistics::Defense });
        break;

    case Type::Phoenix:
        m_name = L"スザク";
        m_maxHitPoint = 80;
        m_currentHitPoint = m_maxHitPoint;
        m_attacks.push_back({ L"かぜおこし",5 ,CharacteRistics::Wind });
        m_attacks.push_back({ L"ねっぷう",10 ,CharacteRistics::Fire });
        m_attacks.push_back({ L"ぼうぎょ",25,CharacteRistics::Defense });
        break;

    case Type::Dragon:
        m_name = L"マグナ";
        m_maxHitPoint = 50;
        m_currentHitPoint = m_maxHitPoint;
        m_attacks.push_back({ L"ファングバイト",25 ,CharacteRistics::Normal });
        m_attacks.push_back({ L"アースブレイカー",15 ,CharacteRistics::Soil });
        m_attacks.push_back({ L"ぼうぎょ"  ,10,CharacteRistics::Defense });
        break;

    case Type::Daemon:
        m_name = L"ディアボロ";
        m_maxHitPoint = 99;
        m_currentHitPoint = m_maxHitPoint;
        m_attacks.push_back({ L"ソウルイーター",40 ,CharacteRistics::Darkness });
        m_attacks.push_back({ L"ナイトメア",10 ,CharacteRistics::Darkness});
        m_attacks.push_back({ L"ぼうぎょ"  ,15,CharacteRistics::Defense });
        break;
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

int Monster::GetMaxHitPoint() const
{
    return m_maxHitPoint;
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

void Monster::Heal(int value)
{
    m_currentHitPoint += value;
    if (m_currentHitPoint >= m_maxHitPoint)m_currentHitPoint = m_maxHitPoint;
}
