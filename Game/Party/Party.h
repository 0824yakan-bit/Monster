#pragma once

#include <vector>
#include <memory>

#include "Game/Party/Monster.h"

class Party
{
public:

    // 仲間を追加
    void AddMonster(std::unique_ptr<Monster> monster);

    // 仲間を削除
    void RemoveMonster(int index);

    // 仲間取得
    Monster* GetMonster(int index);

    // 仲間人数
    int GetMonsterCount() const;

    // パーティ最大人数
    static constexpr int MAX_PARTY = 4;

private:

    std::vector<std::unique_ptr<Monster>> m_monsters;
};