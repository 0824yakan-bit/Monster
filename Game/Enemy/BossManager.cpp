#include "pch.h"
#include "Game/Enemy/BossManager.h"

BossManager::BossManager()
    :m_bossDefeated{false}
{
}

void BossManager::DefeatBoss(int bossNo)
{
    if (bossNo < 0 || bossNo >= 4)
    {
        return;
    }

    m_bossDefeated[bossNo] = true;
}

bool BossManager::IsBossDefeated(int bossNo) const
{
    if (bossNo < 0 || bossNo >= 4)
    {
        return false;
    }

    return m_bossDefeated[bossNo];
}

bool BossManager::IsAllBossDefeated() const
{
    for (int i = 0;i < 3;i++)
    {
        if (m_bossDefeated[i] == FALSE)
        {
            return false;
        }
    }
    return true;
}
