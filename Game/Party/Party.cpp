#include "pch.h"
#include "Game/Party/Party.h"

void Party::AddMonster(std::unique_ptr<Monster> monster)
{
    m_monsters.push_back(std::move(monster));
}

void Party::RemoveMonster(int index)
{
	if (index < 0 || index >= m_monsters.size())
	{
		return;
	}

	m_monsters.erase(
		m_monsters.begin() + index
	);
}

Monster* Party::GetMonster(int index)
{
    return m_monsters[index].get();
}

int Party::GetMonsterCount() const
{
    return (int)m_monsters.size();
}

