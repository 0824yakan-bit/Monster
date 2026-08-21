#include "pch.h"
#include "Game/Enemy/BossManager.h"

BossManager::BossManager()
{
}

void BossManager::DefeatBoss(int bossNo)
{
    if (bossNo < 0 || bossNo >= 3)
        return;

    m_bossDefeated[bossNo] = true;
}

bool BossManager::IsBossDefeated(int bossNo) const
{
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
