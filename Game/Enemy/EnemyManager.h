#pragma once

#include <memory>
#include <vector>
#include<unordered_set>
#include "Game/ImageManager/ImageManager.h"
#include "Game/Enemy/Enemy.h"


class Map;
class PlayerManager;

struct EnemyData
{
    int id;
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
    std::unordered_set<int> m_defeatedEnemies;
public:
    EnemyManager();
    ~EnemyManager();

    void Initialize(Map& map, Party& party);
    void Update(Map&map);
    void Render();
    void Finalize();

    void SetImage(ImageManager* image);

    Enemy* CheckHit(PlayerManager& playermanager);

    void RemoveEnemy(Enemy* enemy);

    void CreateRandomEnemy(Map& map, Party& party, int x, int y);
    Enemy* CreateBattleEnemy(Map& map, Party& party, Enemy::EnemyType type);


    void CreateSlime    (int x, int y, Map& map, bool isBoss, int bossNo,Party&party);
    void CreateWolf     (int x, int y, Map& map, bool isBoss, int bossNo,Party&party);
    void CreateFairy    (int x, int y, Map& map, bool isBoss, int bossNo,Party&party);
    void CreateTurtle   (int x, int y, Map& map, bool isBoss, int bossNo,Party&party);
    void CreateMole     (int x, int y, Map& map, bool isBoss, int bossNo,Party&party);
    void CreateFox      (int x, int y, Map& map, bool isBoss, int bossNo,Party&party);
    void CreateGolem    (int x, int y, Map& map, bool isBoss, int bossNo,Party&party);
    void CreatePhoenix  (int x, int y, Map& map, bool isBoss, int bossNo,Party&party);
    void CreateDragon   (int x, int y, Map& map, bool isBoss, int bossNo,Party&party);
    void CreateDaemon   (int x, int y, Map& map, bool isBoss, int bossNo,Party&party);

    bool ReductionEnemy(Enemy& enemy);
    void SetEnemyAttackNames(Enemy* enemy);
};
