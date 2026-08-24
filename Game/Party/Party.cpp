#include "pch.h"
#include "Game/Party/Party.h"


void Party::AddMonster(std::unique_ptr<Monster> monster)
{
    if (monster == nullptr)
    {
        return;
    }

    // パーティが満員なら追加しない
    if (m_monsters.size() >= MAX_PARTY)
    {
        return;
    }

    m_monsters.push_back(std::move(monster));
}


void Party::RemoveMonster(int index)
{
    if (index < 0 || index >= static_cast<int>(m_monsters.size()))
    {
        return;
    }

    m_monsters.erase(m_monsters.begin() + index);
}


Monster* Party::GetMonster(int index)
{
    if (index < 0 || index >= static_cast<int>(m_monsters.size()))
    {
        return nullptr;
    }

    return m_monsters[index].get();
}


int Party::GetMonsterCount() const
{
    return static_cast<int>(m_monsters.size());
}