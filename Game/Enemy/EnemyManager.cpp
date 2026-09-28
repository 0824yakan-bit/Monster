#include"pch.h"
#include"Game/Enemy/EnemyManager.h"

#include"Game/Maths/Collisionall.h"
#include"Game/Enemy/Enemy.h"
#include"Game/Enemy/Slime.h"
#include"Game/Enemy/Wolf.h"
#include"Game/Enemy/Fairy.h"
#include"Game/Enemy/Turtle.h"
#include"Game/Enemy/Mole.h"
#include"Game/Enemy/Fox.h"
#include"Game/Enemy/Golem.h"
#include"Game/Enemy/Phoenix.h"
#include"Game/Enemy/Dragon.h"
#include"Game/Enemy/Daemon.h"
EnemyManager::EnemyManager()
{
}

EnemyManager::~EnemyManager()
{
}

namespace
{
	const  std::vector<EnemyData>enemyData =
	{
		/*
		*structEnemyData
		{
		int mapNo;
		int enemyType;
		int x;
		int y;
		boolisBoss;
		int bossNo;
		};
		Slime,//0
		Wolf,//1
		Fairy,//2
		Turtle,//3
		Mole,//4
		Fox,//5
		Golem,//6//ボス１
		Phoenix,//7//ボス２
		Dragon,//8//ボス３
		Daemon,//9//ボス４
		*/
		//map0
		{1,0,0,13,6,false,-1},
		{2,0,0,33,7,false,-1},

		//map1
		{3,1,1,8,8,false,-1},
		{4,1,2,18,14,false,-1},
		{5,1,1,30,18,false,-1},
		//map2
		{6,2,6,9,7,true,0},//ボス１ゴーレム
		{7,2,4,33,13,false,-1},
		{8,2,1,33,19,false,-1},
		{9,2,1,3,17,false,-1},
		//map3
		{10,3,7,18,13,true,1},//ボス２フェニックス
		{11,3,5,33,4,false,-1},
		{12,3,5,17,8,false,-1},
		//map4
		{13,4,3,14,3,false,-1},
		{14,4,0,12,19,false,-1},
		{15,4,2,27,18,false,-1},
		{16,4,3,26,8,false,-1},
		{17,4,1,13,11,false,-1},

		//map5
		{18,5,5,26,13,false,-1},
		{19,5,5,12,5,false,-1},
		//map6
		{20,6,5,6,10,false,-1},
		{21,6,1,31,11,false,-1},
		{22,6,3,7,5,false,-1},

		//map7
		{23,7,8,15,4,true,2},//ボス3ドラゴン
		{24,7,3,14,14,false,-1},
		{25,7,4,29,12,false,-1},
		{26,7,0,33,6,false,-1},
		//map8
		{27,8,5,5,5,false,-1},
		{28,8,4,9,12,false,-1},
		{29,8,3,11,19,false,-1},
		{30,8,1,21,17,false,-1},

		//map9ボス専用マップ
		{31,9,9,15,0,true,3}//ボス4
	};
}


void EnemyManager::Initialize(Map& map, Party& party)
{

	m_enemies.clear();

	int mapNo = map.GetCurrentMap();

	for (const auto& data : enemyData)
	{
		if (data.mapNo != mapNo)
			continue;

		//すでに倒している敵なら生成しない
		if (m_defeatedEnemies.find(data.id) != m_defeatedEnemies.end())
			continue;
		switch (data.enemyType)
		{
		case 0:
			CreateSlime(data.x, data.y, map, data.isBoss, data.bossNo, party);
			break;

		case 1:
			CreateWolf(data.x, data.y, map, data.isBoss, data.bossNo, party);
			break;

		case 2:
			CreateFairy(data.x, data.y, map, data.isBoss, data.bossNo, party);
			break;

		case 3:
			CreateTurtle(data.x, data.y, map, data.isBoss, data.bossNo, party);
			break;

		case 4:
			CreateMole(data.x, data.y, map, data.isBoss, data.bossNo, party);
			break;

		case 5:
			CreateFox(data.x, data.y, map, data.isBoss, data.bossNo, party);
			break;

		case 6:
			CreateGolem(data.x, data.y, map, data.isBoss, data.bossNo, party);
			break;

		case 7:
			CreatePhoenix(data.x, data.y, map, data.isBoss, data.bossNo, party);
			break;

		case 8:
			CreateDragon(data.x, data.y, map, data.isBoss, data.bossNo, party);
			break;

		case 9:
			CreateDaemon(data.x, data.y, map, data.isBoss, data.bossNo, party);
			break;
		}

		//作成した敵にデータを設定
		if (!m_enemies.empty())
		{
			Enemy* enemy = m_enemies.back().get();

			enemy->SetEnemyId(data.id);

			//技名
			SetEnemyAttackNames(enemy);
		}
	}
}

void EnemyManager::Update(Map&map)
{
for(auto it=m_enemies.begin();it!=m_enemies.end();)
{
Enemy*enemy=it->get();

//バトル中の敵は処理しない
if(enemy->IsBattleEnemy())
{
++it;
continue;
}

//Bossは絶対に落ちない
if(enemy->IsBoss())
{
enemy->Update(map);

++it;
continue;
}

//すでに落下中の場合
if(enemy->IsFalling())
{
if(ReductionEnemy(*enemy))
{
int enemyId=enemy->GetEnemyId();

if(enemyId>=0)
{
m_defeatedEnemies.insert(enemyId);
}

it=m_enemies.erase(it);
}
else 
{
++it;
}

continue;
}

//通常の敵を更新
enemy->Update(map);

//Fallが1マスでもあれば落下開始
if(map.IsFallRect(
enemy->GetPosition().x,
enemy->GetPosition().y,
enemy->GetSize().x,
enemy->GetSize().y))
{
ReductionEnemy(*enemy);
}

++it;
}
}
void EnemyManager::Render(Map& map)
{
for(auto &enemy:m_enemies)
{
//Battle専用敵はFieldSceneでは描画しない
if(enemy->IsBattleEnemy())
{
continue;
}
enemy->Render(map);
}
}

void EnemyManager::Finalize()
{
for(auto &enemy:m_enemies)
{
enemy->Finalize();
}
}

void EnemyManager::SetImage(ImageManager*image)
{
m_image=image;
}

Enemy*EnemyManager::CheckHit(PlayerManager&playermanager)
{
for(auto &enemy:m_enemies)
{
//Battle専用敵はFieldSceneでは当たり判定しない
if(enemy->IsBattleEnemy())
{
continue;
}

if(Collisionall::HitCharacter(playermanager,enemy.get()))
{
enemy->OnHit(playermanager);

return enemy.get();
}
}

return nullptr;
}
void EnemyManager::RemoveEnemy(Enemy* enemy)
{
	if (enemy == nullptr)
		return;

	//撃破済みとして記録
	int enemyId = enemy->GetEnemyId();

	if (enemyId >= 0)
	{
		m_defeatedEnemies.insert(enemyId);
	}

	//リストから削除
	auto it = std::remove_if(
		m_enemies.begin(),
		m_enemies.end(),
		[enemy](const std::unique_ptr<Enemy>& e)
		{
			return e.get() == enemy;
		});

	m_enemies.erase(it, m_enemies.end());
}

void EnemyManager::CreateRandomEnemy(Map& map, Party& party, int x, int y)
{
	int type = GetRand(5);

	switch (type)
	{
	case 0:
		CreateSlime(x, y, map, false, -1, party);
		break;

	case 1:
		CreateWolf(x, y, map, false, -1, party);
		break;

	case 2:
		CreateFairy(x, y, map, false, -1, party);
		break;

	case 3:
		CreateTurtle(x, y, map, false, -1, party);
		break;

	case 4:
		CreateMole(x, y, map, false, -1, party);
		break;

	case 5:
		CreateFox(x, y, map, false, -1, party);
		break;
	}
	if (!m_enemies.empty())
	{
		Enemy* enemy = m_enemies.back().get();
		m_enemies.back()->SetEnemyId(-1);
		SetEnemyAttackNames(enemy);
	}
}

Enemy* EnemyManager::CreateBattleEnemy(Map& map, Party& party, Enemy::EnemyType type)
{
	switch (type)
	{
	case Enemy::EnemyType::Slime:
		CreateSlime(0, 0, map, false, -1, party);
		break;

	case Enemy::EnemyType::Wolf:
		CreateWolf(0, 0, map, false, -1, party);
		break;


	case Enemy::EnemyType::Fairy:
		CreateFairy(0, 0, map, false, -1, party);
		break;

	case Enemy::EnemyType::Turtle:
		CreateTurtle(0, 0, map, false, -1, party);
		break;

	case Enemy::EnemyType::Mole:
		CreateMole(0, 0, map, false, -1, party);
		break;

	case Enemy::EnemyType::Fox:
		CreateFox(0, 0, map, false, -1, party);
		break;

	case Enemy::EnemyType::Golem:
		CreateGolem(0, 0, map, false, -1, party);
		break;

	case Enemy::EnemyType::Phoenix:
		CreatePhoenix(0, 0, map, false, -1, party);
		break;

	case Enemy::EnemyType::Dragon:
		CreateDragon(0, 0, map, false, -1, party);
		break;

	case Enemy::EnemyType::Daemon:
		CreateDaemon(0, 0, map, false, -1, party);
		break;

	default:
		return nullptr;
	}

	if (m_enemies.empty())
	{
		return nullptr;
	}

	Enemy* enemy = m_enemies.back().get();
	SetEnemyAttackNames(enemy);

	//Battle専用敵であることを設定
	enemy->SetBattleEnemy(true);
	//BattleEnemyは撃破記録対象外
	enemy->SetEnemyId(-1);
	return enemy;
}
void EnemyManager::CreateSlime(int x, int y, Map& map, bool isBoss, int bossNo, Party& party)
{
	auto slime = std::make_unique<Slime>();

	slime->SetImage(m_image);

	slime->Initialize(map, party, x, y, isBoss);
	if (isBoss)
	{
		slime->SetBoss(bossNo);
	}
	m_enemies.push_back(std::move(slime));
}

void EnemyManager::CreateWolf(int x, int y, Map& map, bool isBoss, int bossNo, Party& party)
{
	auto wolf = std::make_unique<Wolf>();

	wolf->SetImage(m_image);

	wolf->Initialize(map, party, x, y, isBoss);
	if (isBoss)
	{
		wolf->SetBoss(bossNo);
	}
	m_enemies.push_back(std::move(wolf));
}


void EnemyManager::CreateFairy(int x, int y, Map& map, bool isBoss, int bossNo, Party& party)
{
	auto fairy = std::make_unique<Fairy>();

	fairy->SetImage(m_image);

	fairy->Initialize(map, party, x, y, isBoss);
	if (isBoss)
	{
		fairy->SetBoss(bossNo);
	}
	m_enemies.push_back(std::move(fairy));
}

void EnemyManager::CreateTurtle(int x, int y, Map& map, bool isBoss, int bossNo, Party& party)
{
	auto turtle = std::make_unique<Turtle>();

	turtle->SetImage(m_image);

	turtle->Initialize(map, party, x, y, isBoss);
	if (isBoss)
	{
		turtle->SetBoss(bossNo);
	}
	m_enemies.push_back(std::move(turtle));
}

void EnemyManager::CreateMole(int x, int y, Map& map, bool isBoss, int bossNo, Party& party)
{
	auto mole = std::make_unique<Mole>();

	mole->SetImage(m_image);

	mole->Initialize(map, party, x, y, isBoss);
	if (isBoss)
	{
		mole->SetBoss(bossNo);
	}
	m_enemies.push_back(std::move(mole));
}

void EnemyManager::CreateFox(int x, int y, Map& map, bool isBoss, int bossNo, Party& party)
{
	auto fox = std::make_unique<Fox>();

	fox->SetImage(m_image);

	fox->Initialize(map, party, x, y, isBoss);
	if (isBoss)
	{
		fox->SetBoss(bossNo);
	}
	m_enemies.push_back(std::move(fox));
}


void EnemyManager::CreateGolem(int x, int y, Map& map, bool isBoss, int bossNo, Party& party)
{
	auto golem = std::make_unique<Golem>();

	golem->SetImage(m_image);

	golem->Initialize(map, party, x, y, isBoss);
	if (isBoss)
	{
		golem->SetBoss(bossNo);
	}
	m_enemies.push_back(std::move(golem));
}

void EnemyManager::CreatePhoenix(int x, int y, Map& map, bool isBoss, int bossNo, Party& party)
{
	auto phoenix = std::make_unique<Phoenix>();

	phoenix->SetImage(m_image);

	phoenix->Initialize(map, party, x, y, isBoss);
	if (isBoss)
	{
		phoenix->SetBoss(bossNo);
	}
	m_enemies.push_back(std::move(phoenix));
}

void EnemyManager::CreateDragon(int x, int y, Map& map, bool isBoss, int bossNo, Party& party)
{
	auto dragon = std::make_unique<Dragon>();

	dragon->SetImage(m_image);

	dragon->Initialize(map, party, x, y, isBoss);

	if (isBoss)
	{
		dragon->SetBoss(bossNo);
	}
	m_enemies.push_back(std::move(dragon));
}

void EnemyManager::CreateDaemon(int x, int y, Map& map, bool isBoss, int bossNo, Party& party)
{
	auto daemon = std::make_unique<Daemon>();

	daemon->SetImage(m_image);

	daemon->Initialize(map, party, x, y, isBoss);

	if (isBoss)
	{
		daemon->SetBoss(bossNo);
	}
	m_enemies.push_back(std::move(daemon));
}

bool EnemyManager::ReductionEnemy(Enemy& enemy)
{
	//初めて落下した瞬間
	if (!enemy.m_isFalling)
	{
		enemy.m_isFalling = true;
		enemy.m_fallTimer = 0;
		enemy.m_fallScale = 100;
	}

	enemy.m_fallTimer++;

	//徐々に小さくする
	enemy.m_fallScale -= 3;

	if (enemy.m_fallScale < 0)
	{
		enemy.m_fallScale = 0;
	}

	//描画倍率
	enemy.m_drawScale = enemy.m_fallScale;

	//下に落ちる
	enemy.position.y += 2;

	//30フレーム後に削除する
	if (enemy.m_fallTimer >= 30)
	{
		return true;
	}

	return false;
}
void EnemyManager::SetEnemyAttackNames(Enemy* enemy)
{
	if (enemy == nullptr)
	{
		return;
	}

	switch (enemy->type)
	{
	case Enemy::EnemyType::Slime:
		enemy->SetAttackNames(L"たいあたり", L"スライムシャワー");

		enemy->SetAttackElements(Monster::CharacteRistics::Normal, Monster::CharacteRistics::Water);
		break;

	case Enemy::EnemyType::Wolf:
		enemy->SetAttackNames(L"ひっかき", L"なぎはらい");

		enemy->SetAttackElements(Monster::CharacteRistics::Normal, Monster::CharacteRistics::Wind);
		break;
	case Enemy::EnemyType::Fairy:
		enemy->SetAttackNames(L"かぜおこし", L"もうどくのきり");

		enemy->SetAttackElements(Monster::CharacteRistics::Wind, Monster::CharacteRistics::Grass);
		break;

	case Enemy::EnemyType::Turtle:
		enemy->SetAttackNames(L"たいあたり", L"じしん");

		enemy->SetAttackElements(Monster::CharacteRistics::Normal, Monster::CharacteRistics::Soil);
		break;

	case Enemy::EnemyType::Mole:
		enemy->SetAttackNames(L"つちほり", L"じわれ");

		enemy->SetAttackElements(Monster::CharacteRistics::Soil, Monster::CharacteRistics::Soil);
		break;
	case Enemy::EnemyType::Fox:
		enemy->SetAttackNames(L"かみつき", L"きつねび");

		enemy->SetAttackElements(Monster::CharacteRistics::Normal, Monster::CharacteRistics::Fire);
		break;
	case Enemy::EnemyType::Golem:
		enemy->SetAttackNames(L"パンチ", L"いわなげ");

		enemy->SetAttackElements(Monster::CharacteRistics::Normal, Monster::CharacteRistics::Soil);
		break;
	case Enemy::EnemyType::Phoenix:
		enemy->SetAttackNames(L"つばさでうつ", L"ほのおのうず");

		enemy->SetAttackElements(Monster::CharacteRistics::Wind, Monster::CharacteRistics::Fire);
		break;
	case Enemy::EnemyType::Dragon:
		enemy->SetAttackNames(L"かみつき", L"ドラゴンブレス");
		enemy->SetAttackElements(Monster::CharacteRistics::Normal, Monster::CharacteRistics::Fire);
		break;

	case Enemy::EnemyType::Daemon:
		enemy->SetAttackNames(L"ひっかき", L"ダークネス");

		enemy->SetAttackElements(Monster::CharacteRistics::Normal, Monster::CharacteRistics::Darkness);
		break;
	}
}