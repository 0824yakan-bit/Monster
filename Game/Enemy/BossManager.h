class BossManager
{
private:
    bool m_bossDefeated[3];//FALSE::生存・TRUE::撃破

public:
    BossManager();

    void DefeatBoss(int bossNo);//倒したボスを撃破(TRUE)にする

    bool IsBossDefeated(int bossNo) const;//指定したボスが撃破(TRUE)されたか

    bool IsAllBossDefeated() const;//すべてのボスが撃破(TRUE)されたか
};
