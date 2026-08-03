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
    void Update();
    void Render();
    void Finalize();

    void SetImage(ImageManager* image);

    Enemy* CheckHit(PlayerManager& playermanager);

    void RemoveEnemy(Enemy* enemy);

    void CreateSlime(int x, int y, Map& map);
    void CreateWolf(int x, int y, Map& map);
    void CreateDragon(int x, int y, Map& map);
    void CreateGolem(int x, int y, Map& map);
    void CreateFairy(int x, int y, Map& map);
};
