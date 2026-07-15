#include "pch.h"
#include "BattleScene.h"
#include"Game/Enemy/Enemy.h"
#include"Game/Enemy/EnemyManager.h"

BattleScene::BattleScene()
	:m_isFieldRequested{false}
	,m_battleWin{false}
	,m_enemy{nullptr}
{
}

BattleScene::~BattleScene()
{
}

void BattleScene::Initialize(InputManager& inputmanager, Map& map)
{
	if (m_enemy != nullptr)
	{
		m_enemy->Initialize(map);
	}

	m_isFieldRequested = false;
	m_battleWin = false;
}

void BattleScene::Update(InputManager& inputmanager,EnemyManager&enemyManager)
{
	if (CheckHitKey(KEY_INPUT_A))
	{
	
		if (m_battleWin)
		{
			//party.AddMember(m_enemy);      // 仲間にする
			enemyManager.RemoveEnemy(m_enemy); // フィールドから消す
			m_isFieldRequested = true;
		}
	}
	switch (m_enemy->type)
	{
	case Enemy::EnemyType::Slime:
		m_battleWin = true;
			break;

	case Enemy::EnemyType::Wolf:
		m_battleWin = true;
			break;
	}
	
}

void BattleScene::Render()
{
	switch (m_enemy->type)
	{
	case Enemy::EnemyType::Slime:
		DrawBox(0, 0, 1280, 720, GetColor(128, 128, 128), TRUE);
		break;

	case Enemy::EnemyType::Wolf:
		DrawBox(0, 0, 1280, 720, GetColor(255, 255, 255), TRUE);
		break;
	}
}

void BattleScene::Finalize()
{

}

bool BattleScene::IsFieldRequested()const
{
	return m_isFieldRequested;
}

void BattleScene::SetEnemy(Enemy* enemy)
{
	m_enemy = enemy;
}