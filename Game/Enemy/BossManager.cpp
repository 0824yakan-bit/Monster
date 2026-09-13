#include "pch.h"
#include "Game/Enemy/BossManager.h"

BossManager::BossManager()
    :m_bossDefeated{false}
{
}

void BossManager::Initialize()
{
    for (int i = 0;i < 4;i++)
    {
        m_bossDefeated[i]=false;
    }
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
bool BossManager::IsLastBoss(int bossNo) const
{
    return bossNo == 3;
}