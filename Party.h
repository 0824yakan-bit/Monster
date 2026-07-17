#pragma once

#include <memory>
#include <vector>
#include "Game/Party/Monster.h"

class Party
{
private:
    std::vector<std::unique_ptr<Monster>> m_monsters;

public:
    void AddMonster(std::unique_ptr<Monster> monster);

    Monster* GetMonster(int index);

    int GetMonsterCount() const;

    wchar_t* GetMonsterName() const;

};