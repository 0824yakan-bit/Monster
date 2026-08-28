#include "pch.h"
#include "Game/Enemy/EnemyManager.h"

#include"Game/Maths/Collisionall.h"
#include"Game/Enemy/Enemy.h"
#include "Game/Enemy/Slime.h"
#include "Game/Enemy/Wolf.h"
#include"Game/Enemy/Dragon.h"
#include"Game/Enemy/Golem.h"
#include"Game/Enemy/Fairy.h"
EnemyManager::EnemyManager()
{
}

EnemyManager::~EnemyManager()
{
}

namespace
{
    const std::vector<EnemyData> enemyData =
    {
        /*
        Slime   = 0
        Wolf    = 1
        Dragon  = 2
        Golem   
        Fairy
        */
        // map0
        {0, 0,  5,  5, false, -1},
        {0, 1, 10,  8, false, -1},
        {0, 4, 20,  8, false, -1},

        // map1
        {1, 0,  5,  5, false, -1},
        {1, 1, 12, 10, false, -1},
        {1, 4, 18,  6, false, -1},
        {1, 2, 20, 15, true,  0}, // ボス1

        // map2
        {2, 0, 15, 20, false, -1},
        {2, 3, 10, 10, false, -1},
        {2, 1,  5, 15, false, -1},
        {2, 4, 20,  5, false, -1},

        // map3
        {3, 1,  1, 10, false, -1},
        {3, 0,  8,  5, false, -1},
        {3, 3, 15, 12, false, -1},
        {3, 4, 22,  8, false, -1},

        // map4
        {4, 0,  5,  5, false, -1},
        {4, 1, 10, 20, false, -1},
        {4, 3, 20,  8, false, -1},
        {4, 4, 15, 15, true,  1}, // ボス2

        // map5
        {5, 0,  5,  5, false, -1},
        {5, 1, 12, 10, false, -1},
        {5, 2, 20, 15, false, -1},
        {5, 4,  8, 20, false, -1},

        // map6
        {6, 3,  5,  5, false, -1},
        {6, 1, 15,  8, false, -1},
        {6, 2, 20, 18, false, -1},
        {6, 4, 10, 20, false, -1},

        // map7
        {7, 0,  5,  5, false, -1},
        {7, 1, 10, 10, false, -1},
        {7, 2, 20,  5, false, -1},
        {7, 3,  5, 20, false, -1},
        {7, 4, 15, 15, true,  2}, // ボス3

        // map8
        {8, 0,  5,  5, false, -1},
        {8, 1, 10, 10, false, -1},
        {8, 2, 20,  5, false, -1},
        {8, 3,  5, 20, false, -1},
        {8, 4, 20, 20, false, -1},

        // map9 ボス専用マップ
        {9,2,5,5,true,3}//ボス4
    };
}


void EnemyManager::Initialize(Map& map)
{

    m_enemies.clear();

    int mapNo = map.GetCurrentMap();

    for (auto& data : enemyData)
    {
        if (data.mapNo != mapNo)
            continue;
        switch (data.enemyType)
        {
        case 0:
            CreateSlime(data.x, data.y, map,data.isBoss,data.bossNo);
            break;

        case 1:
            CreateWolf(data.x, data.y, map, data.isBoss, data.bossNo);
            break;

        case 2:
            CreateDragon(data.x, data.y, map, data.isBoss, data.bossNo);
            break;

        case 3:
            CreateGolem(data.x, data.y, map, data.isBoss, data.bossNo);
            break;

        case 4:
            CreateFairy(data.x, data.y, map, data.isBoss, data.bossNo);
            break;
        }
    }


}

void EnemyManager::Update(Map& map)
{
    for (auto& enemy : m_enemies)
    {
        if (enemy->IsBattleEnemy())
        {
            continue;
        }

        enemy->Update(map);
    }
}
void EnemyManager::Render()
{
    for (auto& enemy : m_enemies)
    {
        // Battle専用敵はFieldSceneでは描画しない
        if (enemy->IsBattleEnemy())
        {
            continue;
        }

        enemy->Render();
    }
}

void EnemyManager::Finalize()
{
    for (auto& enemy : m_enemies)
    {
        enemy->Finalize();
    }
}

void EnemyManager::SetImage(ImageManager* image)
{
    m_image = image;
}

Enemy* EnemyManager::CheckHit(PlayerManager& playermanager)
{
    for (auto& enemy : m_enemies)
    {
        // Battle専用敵はFieldSceneでは当たり判定しない
        if (enemy->IsBattleEnemy())
        {
            continue;
        }

        if (Collisionall::HitCharacter(playermanager, enemy.get()))
        {
            enemy->OnHit(playermanager);

            return enemy.get();
        }
    }

    return nullptr;
}
void EnemyManager::RemoveEnemy(Enemy* enemy)
{
    auto it = std::remove_if(
        m_enemies.begin(),
        m_enemies.end(),
        [enemy](const std::unique_ptr<Enemy>& e)
        {
            return e.get() == enemy;
        });

    m_enemies.erase(it, m_enemies.end());
}

void EnemyManager::CreateRandomEnemy(Map& map, int x, int y)
{
    int type = GetRand(4);

    switch (type)
    {
    case 0:
        CreateSlime(x, y, map,false,-1);
        break;

    case 1:
        CreateWolf(x, y, map, false, -1);
        break;

    case 2:
        CreateDragon(x, y, map, false, -1);
        break;

    case 3:
        CreateGolem(x, y, map, false, -1);
        break;

    case 4:
        CreateFairy(x, y, map, false, -1);
        break;
    }
}
Enemy* EnemyManager::CreateBattleEnemy(Map& map,Enemy::EnemyType type)
{
    switch (type)
    {
    case Enemy::EnemyType::Slime:
        CreateSlime(0, 0, map,false,-1);
        break;

    case Enemy::EnemyType::Wolf:
        CreateWolf(0, 0, map, false, -1);
        break;

    case Enemy::EnemyType::Dragon:
        CreateDragon(0, 0, map, false, -1);
        break;

    case Enemy::EnemyType::Golem:
        CreateGolem(0, 0, map, false, -1);
        break;

    case Enemy::EnemyType::Fairy:
        CreateFairy(0, 0, map, false, -1);
        break;

    default:
        return nullptr;
    }

    if (m_enemies.empty())
    {
        return nullptr;
    }

    Enemy* enemy = m_enemies.back().get();

    //Battle専用敵であることを設定
    enemy->SetBattleEnemy(true);

    return enemy;
}
void EnemyManager::CreateSlime(int x, int y, Map& map, bool isBoss, int bossNo)
{
    auto slime = std::make_unique<Slime>();

    slime->SetImage(m_image);

    slime->Initialize(map, x, y);
    if (isBoss)
    {
        slime->SetBoss(bossNo);
    }
    m_enemies.push_back(std::move(slime));
}

void EnemyManager::CreateWolf(int x, int y, Map& map, bool isBoss, int bossNo)
{
    auto wolf = std::make_unique<Wolf>();

    wolf->SetImage(m_image);

    wolf->Initialize(map, x, y);
    if (isBoss)
    {
        wolf->SetBoss(bossNo);
    }
    m_enemies.push_back(std::move(wolf));
}

void EnemyManager::CreateDragon(int x, int y, Map& map, bool isBoss, int bossNo)
{
    auto dragon = std::make_unique<Dragon>();

    dragon->SetImage(m_image);

    dragon->Initialize(map, x, y);

    if (isBoss)
    {
        dragon->SetBoss(bossNo);
    }


    m_enemies.push_back(std::move(dragon));
}

void EnemyManager::CreateGolem(int x, int y, Map& map, bool isBoss, int bossNo)
{
    auto golem = std::make_unique<Golem>();

    golem->SetImage(m_image);

    golem->Initialize(map, x, y);
    if (isBoss)
    {
        golem->SetBoss(bossNo);
    }
    m_enemies.push_back(std::move(golem));
}

void EnemyManager::CreateFairy(int x, int y, Map& map, bool isBoss, int bossNo)
{
    auto fairy = std::make_unique<Fairy>();

    fairy->SetImage(m_image);

    fairy->Initialize(map, x, y);
    if (isBoss)
    {
        fairy->SetBoss(bossNo);
    }
    m_enemies.push_back(std::move(fairy));
}