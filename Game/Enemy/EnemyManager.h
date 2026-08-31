#pragma once

#include <memory>
#include <vector>

#include "Game/ImageManager/ImageManager.h"
#include "Game/Enemy/Enemy.h"


class Map;
class PlayerManager;

struct EnemyData
{
    int mapNo;
    int enemyType;
    int x;
    int y;
    bool isBoss;
    int bossNo;
};
class EnemyManager
{
private:
    ImageManager* m_image=nullptr;

public:

    std::vector<std::unique_ptr<Enemy>> m_enemies;

public:
    EnemyManager();
    ~EnemyManager();

    void Initialize(Map& map);
    void Update(Map&map);
    void Render();
    void Finalize();

    void SetImage(ImageManager* image);

    Enemy* CheckHit(PlayerManager& playermanager);

    void RemoveEnemy(Enemy* enemy);

    void CreateRandomEnemy(Map& map, int x, int y);
    Enemy* CreateBattleEnemy(Map& map, Enemy::EnemyType type);


    void CreateSlime    (int x, int y, Map& map, bool isBoss, int bossNo);
    void CreateWolf     (int x, int y, Map& map, bool isBoss, int bossNo);
    void CreateFairy    (int x, int y, Map& map, bool isBoss, int bossNo);
    void CreateTurtle   (int x, int y, Map& map, bool isBoss, int bossNo);
    void CreateMole     (int x, int y, Map& map, bool isBoss, int bossNo);
    void CreateFox      (int x, int y, Map& map, bool isBoss, int bossNo);
    void CreateGolem    (int x, int y, Map& map, bool isBoss, int bossNo);
    void CreatePhoenix  (int x, int y, Map& map, bool isBoss, int bossNo);
    void CreateDragon   (int x, int y, Map& map, bool isBoss, int bossNo);
    void CreateDaemon   (int x, int y, Map& map, bool isBoss, int bossNo);
};
