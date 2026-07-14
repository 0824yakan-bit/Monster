#pragma once

#include <memory>
#include <vector>

#include "Game/Enemy/Enemy.h"

class Map;

class EnemyManager
{
private:

public:
    std::vector<std::unique_ptr<Enemy>> m_enemies;

public:
    EnemyManager();
    ~EnemyManager();

    void Initialize(Map& map);
    void Update();
    void Render();
    void Finalize();
};
