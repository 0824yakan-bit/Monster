#include "pch.h"
#include "BattleScene.h"

#include "Game/Scene/SceneManager.h"
#include "Game/Scene/FieldScene.h"

#include "Game/Player/PlayerManager.h"

#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/EnemyManager.h"

#include "Game/Enemy/Slime.h"
#include "Game/Enemy/Wolf.h"
#include "Game/Enemy/Dragon.h"
#include "Game/Enemy/Golem.h"
#include "Game/Enemy/Fairy.h"


// コンストラクタ / デストラクタ

BattleScene::BattleScene(BossManager& bossManager)
	: m_teamjoin{ TeamJoin::Join }
	, m_receponsTimer{ 0 }
	, m_joinSelect{ 0 }
	, m_isJoinRequested{ false }
	, m_image{ nullptr }
	, m_battle{ new Battle(bossManager) }
	, m_player{ nullptr }
	, m_enemy{ nullptr }
	, m_enemyName{ L"" }
	, m_scenemanager{ nullptr }
	, m_isReplaceSelect{ false }
	, m_battleEnemies{}
	, m_pendingMonster{ nullptr }
	, m_isFieldRequested{ false }
	, m_isTitleRequested{ false }
	, m_enemyManager{ nullptr }
	, m_runEnemyBonus{ 0 }
	, m_battleWin{ false }
{
}

BattleScene::~BattleScene()
{
}


// 初期化

void BattleScene::Initialize(InputManager& inputmanager,SceneManager& sceneManager,Map& map,Party& party)
{
	// BattleへPartyを設定
	m_battle->SetParty(&party);
	
	// Battle初期化
	m_battle->Initialize(&sceneManager);

	// 背景サイズ
	drawBgPosition.x = 0;
	drawBgPosition.y = 0;

	drawBgSize.x = 1280;
	drawBgSize.y = 720;

	// 仲間にするかボックス
	drawCommandBoxPosition.x = 120;
	drawCommandBoxPosition.y = 440;

	drawCommandBoxSize.x = 410;
	drawCommandBoxSize.y = 270;

	// 状態リセット
	m_isJoinRequested = false;
	m_isReplaceSelect = false;
	m_isFieldRequested = false;
	m_isTitleRequested = false;
	m_battleWin = false;

	m_receponsTimer = 0;

	// 前回の保留モンスターをリセット
	m_pendingMonster.reset();


	// 戦闘用敵リスト作成
	//
	// [0]追加敵
	// [1]エンカウント敵
	// [2]追加敵

	m_battleEnemies.clear();

	// 追加敵を生成
	CreateBattleEnemies(map);

	m_runEnemyBonus = 0;


	// エンカウント敵を中央へ追加
	if (m_enemy != nullptr)
	{
		if (m_battleEnemies.empty())
		{
			// 追加敵がいない場合
			m_battleEnemies.push_back(m_enemy);
		}
		else
		{
			// 追加敵がある場合はindex1に入れる
			size_t insertIndex =
				std::min<size_t>(1, m_battleEnemies.size());

			m_battleEnemies.insert(
				m_battleEnemies.begin() + insertIndex,
				m_enemy
			);
		}
	}


	// 敵の位置設定
	SetBattleEnemyPositions();

	// Battleへ敵を渡す
	SetBattleEnemies();
}


// 更新
void BattleScene::Update(InputManager& inputManager,SceneManager& sceneManager,FieldScene& fieldScene,GameOver& gameOver,EnemyManager& enemyManager,Map& map,Party& party,PlayerManager& player)
{
	inputManager.Update();

	m_receponsTimer++;

	// モンスター交換選択中
	/*
		仲間になったMonsterはFieldSceneへ渡し、
		パーティへの追加・交換・技属性取得はFieldScene側で行う。
	*/
	if (m_isReplaceSelect)
	{
		return;
	}

	// 戦闘終了後の仲間加入処理
	if (m_battle->IsEnemyRequested() &&m_battle->AreAllEnemiesDead())
	{
		if (!m_isJoinRequested && !m_isReplaceSelect)
		{
			m_isJoinRequested = true;
			m_isReplaceSelect = false;

			m_joinSelect = 0;
			m_receponsTimer = 0;

			return;
		}
	}

	// 通常のBattle更新
	if (!m_isJoinRequested && !m_isReplaceSelect)
	{
		m_battle->Update(inputManager,&sceneManager,gameOver,map,player);

		// GameOverからタイトル要求
		if (gameOver.IsTitleRequest())
		{
			m_isTitleRequested = true;
			return;
		}
	}

	// フィールドへ戻る要求
	if (m_battle->IsFieldRequested())
	{
		// 逃走成功なら次の戦闘の敵を+1
		if (m_battle->IsRunSuccess())
		{
			// 次の戦闘だけ敵を1体増やす
			m_runEnemyBonus = 1;

			m_battle->ResetRunSuccess();
		}

		// 戦闘前の位置へ戻す
		if (m_player != nullptr)
		{
			m_player->m_position =
				m_player->m_oldposition;

			m_player->m_invicible = true;
		}

		m_isFieldRequested = true;
	}

	// 仲間加入選択
	if (m_isJoinRequested)
	{
		m_battle->SetJoinWindow(true);


		if (m_receponsTimer > 30)
		{
			// 左右選択
			if (CheckHitKey(KEY_INPUT_LEFT))
			{
				m_joinSelect = 0;
				m_receponsTimer = 0;
			}
			else if (CheckHitKey(KEY_INPUT_RIGHT))
			{
				m_joinSelect = 1;
				m_receponsTimer = 0;
			}


			// 決定
			else if (CheckHitKey(KEY_INPUT_RETURN))
			{
				// 倒したエンカウント敵
				Enemy* targetEnemy = m_enemy;


				if (targetEnemy == nullptr)
				{
					return;
				}


				// 仲間にする
				if (m_joinSelect == 0)
				{
					std::unique_ptr<Monster> monster;


					// 敵の種類からMonsterを作成
					switch (targetEnemy->type)
					{
					case Enemy::EnemyType::Slime:
						monster =std::make_unique<Monster>(Monster::Type::Slime);
						break;

					case Enemy::EnemyType::Wolf:
						monster =std::make_unique<Monster>(Monster::Type::Wolf);
						break;

					case Enemy::EnemyType::Fairy:
						monster =std::make_unique<Monster>(Monster::Type::Fairy);
						break;

					case Enemy::EnemyType::Turtle:
						monster = std::make_unique<Monster>(Monster::Type::Turtle);
						break;

					case Enemy::EnemyType::Mole:
						monster = std::make_unique<Monster>(Monster::Type::Mole);
						break;

					case Enemy::EnemyType::Fox:
						monster = std::make_unique<Monster>(Monster::Type::Fox);
						break;

					case Enemy::EnemyType::Golem:
						monster =std::make_unique<Monster>(Monster::Type::Golem);
						break;

					case Enemy::EnemyType::Phoenix:
						monster = std::make_unique<Monster>(Monster::Type::Phoenix);
						break;

					case Enemy::EnemyType::Dragon:
						monster =std::make_unique<Monster>(Monster::Type::Dragon);
						break;

					case Enemy::EnemyType::Daemon:
						monster = std::make_unique<Monster>(Monster::Type::Daemon);
						break;
					}


					// Monsterが生成できていない場合
					if (monster == nullptr)
					{
						return;
					}

					fieldScene.ReceiveJoinedMonster(std::move(monster));

					// Battleから敵を削除
					m_battle->RemoveEnemy(targetEnemy);

					// EnemyManagerから敵本体を削除
					enemyManager.RemoveEnemy(targetEnemy);

					m_enemy = nullptr;

					// 加入選択終了
					m_isJoinRequested = false;

					// Fieldへ戻る
					m_isFieldRequested = true;

					m_receponsTimer = 0;

					return;
				}


				// 仲間にしない
				else
				{
					m_battle->RemoveEnemy(targetEnemy);

					enemyManager.RemoveEnemy(targetEnemy);

					m_enemy = nullptr;

					m_isJoinRequested = false;

					m_isFieldRequested = true;

					m_receponsTimer = 0;

					return;
				}
			}
		}
	}
}


// 描画
void BattleScene::Render(GameOver& gameOver,Party& party,Map& map)
{
	// タイトル要求中は描画しない
	if (m_isTitleRequested)
	{
		return;
	}

	// 敵が存在しない場合は描画しない
	if (!m_enemy)
	{
		return;
	}

	// 背景
	switch (map.GetCurrentMap())
	{
	case 0:

		m_image->DrawForest(drawBgPosition,drawBgSize);

		break;


	case 1:

		m_image->DrawPlain(drawBgPosition,drawBgSize);

		break;


	case 2:

		m_image->DrawDesrt(drawBgPosition,drawBgSize);

		break;


	case 3:

		m_image->DrawVolcano(drawBgPosition,drawBgSize);

		break;


	case 4:

		m_image->DrawCastle(drawBgPosition,drawBgSize);

		break;


	case 5:
	case 6:
	case 7:
	case 8:
	case 9:

		m_image->DrawForest(drawBgPosition,drawBgSize);

		break;
	}


	// 現在の攻撃対象カーソル
	Enemy* target =m_battle->GetTargetEnemy();


	if (target != nullptr &&target->GetHp() > 0)
	{
		int cursorX =static_cast<int>(target->renderPosition.x +target->renderSize.x / 2);

		int cursorY =static_cast<int>(target->renderPosition.y) - 40;

		DrawString(cursorX,cursorY,L"▼",GetColor(255, 255, 0),TRUE);////
	}

	// 全滅
	if (m_battle->GetAnnihilation() &&!gameOver.IsTitleRequest())
	{
		m_battle->RenderAnnihilation(gameOver);
	}
	// Battle描画
	m_battle->Render(gameOver, map);
	// 仲間加入画面
	if (m_isJoinRequested)
	{
		m_enemy->RenderBattle();
		m_enemyName = m_enemy->GetName();

		m_image->DrawCommandbox1(drawCommandBoxPosition,drawCommandBoxSize);

		m_battle->RenderCurrentHp();


		if (m_enemy != nullptr)
		{
			m_enemyName =m_enemy->GetName();
			DrawFormatString(180,480,GetColor(255, 255, 255),L"%lsを\n仲間にしますか？",m_enemyName);
		}
		DrawString(200,610,L"はい",GetColor(255, 255, 255),TRUE);
		DrawString(300,610,L"いいえ",GetColor(255, 255, 255),TRUE);
		
		int cursorX =(m_joinSelect == 0)
			? 180
			: 280;
		DrawString(cursorX,610,L"▶",GetColor(255, 255, 0),TRUE);////
	}

}


// 終了処理
void BattleScene::Finalize()
{
	SetFontSize(20);

	m_battle->Finalize();
}

// 状態取得
bool BattleScene::IsFieldRequested() const
{
	return m_isFieldRequested;
}

bool BattleScene::IsJoinRequested() const
{
	return m_isJoinRequested;
}

bool BattleScene::IsTitleRequested() const
{
	return m_isTitleRequested;
}

// 敵生成
void BattleScene::CreateBattleEnemies(Map& map)
{
	m_battleEnemies.clear();


	if (m_enemyManager == nullptr)
	{
		return;
	}


	// ブレイクレベルによって追加敵の数を決める
	int breakLevel =map.GetBreakLevel();

	int addCount = 0;


	if (breakLevel < 2)
	{
		// ブレイクレベル0～19
		// 追加敵なし

		addCount = 0;
	}
	else if (breakLevel < 4)
	{
		// ブレイクレベル20～39
		// 追加敵1体

		addCount = 1;
	}
	else
	{
		// ブレイクレベル40以上
		// 追加敵2体

		addCount = 2;
	}

	// 逃走成功による追加
	addCount += m_runEnemyBonus;

	// エンカウント敵1体を含めて最大3体
	if (addCount > 2)
	{
		addCount = 2;
	}

	// 追加敵生成
	for (int i = 0; i < addCount; i++)
	{
		Enemy::EnemyType type =static_cast<Enemy::EnemyType>(GetRand(4));
		Enemy* enemy =m_enemyManager->CreateBattleEnemy(map,type);

		if (enemy != nullptr)
		{
			m_battleEnemies.push_back(enemy);
		}
	}
}


// Battleへ敵を設定
void BattleScene::SetBattleEnemies()
{
	m_battle->SetEnemies(m_battleEnemies);
}


// 戦闘中の敵位置設定
void BattleScene::SetBattleEnemyPositions()
{
	if (m_battleEnemies.empty())
	{
		return;
	}

	const int y = 100;

	const int width = 300;
	const int height = 300;

	// 敵の数
	int enemyCount = 0;

	for (Enemy* enemy : m_battleEnemies)
	{
		if (enemy != nullptr)
		{
			enemyCount++;
		}
	}

	if (enemyCount == 0)
	{
		return;
	}

	// 敵の数によって配置を変更
	std::vector<int> positions;

	if (enemyCount == 1)
	{
		// 1体：中央
		positions = { 440 };
	}
	else if (enemyCount == 2)
	{
		// 2体：左右均等
		positions = { 200, 680 };
	}
	else
	{
		// 3体：左・中央・右
		positions = { 70, 490, 910 };
	}


	// 敵を配置
	int positionIndex = 0;

	for (Enemy* enemy : m_battleEnemies)
	{
		if (enemy == nullptr)
		{
			continue;
		}

		if (positionIndex >=static_cast<int>(positions.size()))
		{
			break;
		}


		if (enemy->type == Enemy::EnemyType::Daemon)
		{
			enemy->renderPosition.x = positions[positionIndex]-300;
			enemy->renderPosition.y = y-400;

			enemy->renderSize.x = 1000;
			enemy->renderSize.y = 1000;
		}
		else
		{
			enemy->renderPosition.x = positions[positionIndex];
			enemy->renderPosition.y = y;

			enemy->renderSize.x = width;
			enemy->renderSize.y = height;
		}
		positionIndex++;
	}
}


// ターゲット取得
Enemy* BattleScene::GetTargetEnemy() const
{
	if (m_battle == nullptr)
	{
		return nullptr;
	}

	return m_battle->GetTargetEnemy();
}


// Setter
void BattleScene::SetFieldScene(FieldScene* fieldScene)
{
	m_battle->SetFieldScene(fieldScene);
}

void BattleScene::SetImage(ImageManager* image)
{
	m_image = image;
	m_battle->SetImage(image);
}


void BattleScene::SetPlayer(PlayerManager* player)
{
	m_player = player;
	m_battle->SetPlayer(player);
}


void BattleScene::SetEnemyManager(EnemyManager* enemyManager)
{
	m_enemyManager = enemyManager;
}


void BattleScene::SetEnemy(Enemy* enemy)
{
	m_enemy = enemy;
}


// タイトル要求
void BattleScene::ResetTitleRequest()
{
	m_isTitleRequested = false;
}


// 攻撃履歴
const std::vector<Battle::UsedAttackInfo>&BattleScene::GetUsedAttackOrder() const
{
	return m_battle->GetUsedAttackOrder();
}


void BattleScene::ClearUsedAttackOrder()
{
	m_battle->ClearUsedAttackOrder();
}