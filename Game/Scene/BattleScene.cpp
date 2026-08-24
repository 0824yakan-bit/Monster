#include"pch.h"
#include"BattleScene.h"

#include"Game/Scene/SceneManager.h"
#include"Game/Scene/FieldScene.h"

#include"Game/Player/PlayerManager.h"

#include"Game/Enemy/Enemy.h"
#include"Game/Enemy/EnemyManager.h"

#include"Game/Enemy/Slime.h"
#include"Game/Enemy/Wolf.h"
#include"Game/Enemy/Dragon.h"
#include"Game/Enemy/Golem.h"
#include"Game/Enemy/Fairy.h"



//補助関数
static FieldScene::CooperatList ToCooperatList(Monster::CharacteRistics type)
{
	switch (type)
	{
		case Monster::CharacteRistics::Fire:
		return FieldScene::CooperatList::Fire;

		case Monster::CharacteRistics::Water:
		return FieldScene::CooperatList::Water;

		case Monster::CharacteRistics::Grass:
		return FieldScene::CooperatList::Grass;

		case Monster::CharacteRistics::Soil:
		return FieldScene::CooperatList::Soil;

		case Monster::CharacteRistics::Wind:
		return FieldScene::CooperatList::Wind;

		case Monster::CharacteRistics::Thunder:
		return FieldScene::CooperatList::Thunder;

	default:
		return FieldScene::CooperatList::None;
	}
}



//コンストラクタ/デストラクタ


BattleScene::BattleScene(BossManager&bossManager)
: m_teamjoin		{}
, m_receponsTimer	{ 0 }
, m_joinSelect		{ 0 }
, m_isJoinRequested	{ false }
, m_image			{ nullptr }
, m_battle			{ new Battle(bossManager) }
, m_player			{ nullptr }
, m_enemy			{ nullptr }
, m_enemyName		{ L"" }
, m_scenemanager	{ nullptr }
, m_isReplaceSelect	{ false }
, m_battleWin		{ false }
,m_battleEnemies	{}
, m_pendingMonster	{ nullptr }
, m_isFieldRequested{ false }
, m_isTitleRequested{ false }
,m_enemyManager		{}
,m_runEnemyBonus	{}
{
}

BattleScene::~BattleScene()
{
}



//初期化
void BattleScene::Initialize(InputManager& inputmanager, SceneManager& sceneManager, Map& map, Party& party)
{
	//BattleへPartyを設定
	m_battle->SetParty(&party);

	//Battle初期化
	m_battle->Initialize(&sceneManager);

	//背景サイズ
	drawBgPosition.x = 0;
	drawBgPosition.y = 0;
	drawBgSize.x = 1280;
	drawBgSize.y = 720;

	//仲間にするかボックス
	drawCommandBoxPosition.x = 120;
	drawCommandBoxPosition.y = 440;
	drawCommandBoxSize.x = 410;
	drawCommandBoxSize.y = 270;

	//状態リセット
	m_isJoinRequested = false;
	m_isReplaceSelect = false;
	m_isFieldRequested = false;
	m_isTitleRequested = false;
	m_battleWin = false;

	m_receponsTimer = 0;


	//戦闘用敵リスト作成
	//
	//[0]追加敵
	//[1]エンカウント敵
	//[2]追加敵
	m_battleEnemies.clear();

	//追加敵を生成
	CreateBattleEnemies(map);
	m_runEnemyBonus = 0;

	//エンカウント敵を中央へ追加
	if (m_enemy != nullptr)
	{
		if (m_battleEnemies.empty())
		{
			//追加敵がいない場合
			m_battleEnemies.push_back(m_enemy);
		}
		else 
		{
			//追加敵がある場合はindex1に入れる
			size_t insertIndex =std::min<size_t>(1, m_battleEnemies.size());

			m_battleEnemies.insert(m_battleEnemies.begin() + insertIndex,m_enemy);
		}
	}

	//敵の位置設定
	SetBattleEnemyPositions();

	//Battleへ敵を渡す
	SetBattleEnemies();
}


//更新
void BattleScene::Update(InputManager& inputManager, SceneManager& sceneManager, FieldScene& fieldScene, GameOver& gameOver, EnemyManager& enemyManager, Map& map, Party& party, PlayerManager& player)
{
	inputManager.Update();

	m_receponsTimer++;


	//モンスター交換選択中

	if (m_isReplaceSelect)
	{
		if (m_receponsTimer > 15)
		{
			int select = -1;

			if (CheckHitKey(KEY_INPUT_1))
			{
				select = 0;
			}
			else if (CheckHitKey(KEY_INPUT_2))
			{
				select = 1;
			}
			else if (CheckHitKey(KEY_INPUT_3))
			{
				select = 2;
			}
			else if (CheckHitKey(KEY_INPUT_4))
			{
				select = 3;
			}

			if (select != -1)
			{
				//パーティの範囲外なら何もしない
				if (select >= party.GetMonsterCount())
				{
					return;
				}
				//交換するモンスターが持っている技属性を保存
				std::vector<FieldScene::CooperatList>learnedSkills;

				if (m_pendingMonster != nullptr)
				{
					for (const auto& atk : m_pendingMonster->GetAttacks())
					{
						learnedSkills.push_back(ToCooperatList(atk.ristics)
						);
					}
				}

				//既存モンスター削除
				party.RemoveMonster(select);


				//新しいモンスター追加
				party.AddMonster(std::move(m_pendingMonster));


				//保存しておいた技属性を習得
				for (auto skill : learnedSkills)
				{
					if (skill != FieldScene::CooperatList::None)
					{
						fieldScene.LearnSkill(skill);
					}
				}


				//状態リセット
				m_isReplaceSelect = false;
				m_isFieldRequested = true;
				m_receponsTimer = 0;
			}
		}

		return;
	}

	//戦闘終了後の仲間加入処理
	if (m_battle->IsEnemyRequested() && m_battle->AreAllEnemiesDead())
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


	//通常のBattle更新
	if (!m_isJoinRequested && !m_isReplaceSelect)
	{
		m_battle->Update(inputManager, &sceneManager, gameOver, map, player);

		//GameOverからタイトル要求
		if (gameOver.IsTitleRequest())
		{
			m_isTitleRequested = true;
			return;
		}
	}



	//フィールドへ戻る要求
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
			m_player->m_position = m_player->m_oldposition;
			m_player->m_invicible = true;
		}

		m_isFieldRequested = true;
	}



	//仲間加入選択
	if (m_isJoinRequested)
	{
		m_battle->SetJoinWindow(true);

		if (m_receponsTimer > 30)
		{

			//左右選択
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


			//決定
			else if (CheckHitKey(KEY_INPUT_RETURN))
			{
				//倒したエンカウント敵
				Enemy* targetEnemy = m_enemy;

				if (targetEnemy == nullptr)
				{
					return;
				}



				//仲間にする
				if (m_joinSelect == 0)
				{
					std::unique_ptr<Monster>monster;

					switch (targetEnemy->type)
					{
					case Enemy::EnemyType::Slime:
						monster = std::make_unique<Monster>(Monster::Type::Slime);
						break;

					case Enemy::EnemyType::Wolf:
						monster = std::make_unique<Monster>(Monster::Type::Wolf);
						break;

					case Enemy::EnemyType::Dragon:
						monster = std::make_unique<Monster>(Monster::Type::Dragon);
						break;

					case Enemy::EnemyType::Golem:
						monster = std::make_unique<Monster>(Monster::Type::Golem);
						break;

					case Enemy::EnemyType::Fairy:
						monster = std::make_unique<Monster>(Monster::Type::Fairy);
						break;
					}



					//パーティに空きがある
					if (party.GetMonsterCount() < 4)
					{
						//AddMonster前にポインタを保存
						Monster* learnedMonster = monster.get();

						party.AddMonster(std::move(monster));

						//モンスターの技属性を習得
						for (const auto& atk : learnedMonster->GetAttacks())
						{
							fieldScene.LearnSkill(ToCooperatList(atk.ristics));
						}

						//敵削除
						m_battle->RemoveEnemy(targetEnemy);
						enemyManager.RemoveEnemy(targetEnemy);

						m_enemy = nullptr;

						//状態変更
						m_isJoinRequested = false;
						m_isFieldRequested = true;
					}



					//パーティがいっぱい
					else
					{
						m_pendingMonster = std::move(monster);

						m_isJoinRequested = false;
						m_isReplaceSelect = true;
					}
				}



				//仲間にしない
				else
				{
					m_battle->RemoveEnemy(targetEnemy);
					enemyManager.RemoveEnemy(targetEnemy);

					m_enemy = nullptr;

					m_isJoinRequested = false;
					m_isFieldRequested = true;
				}

				m_receponsTimer = 0;

				return;
			}
		}
	}
}



//描画
void BattleScene::Render(GameOver& gameOver, Party& party, Map& map)
{
	//タイトル要求中は描画しない
	if (m_isTitleRequested)
	{
		return;
	}

	//敵が存在しない場合は描画しない
	if (!m_enemy)
	{
		return;
	}



	//背景
	switch (map.GetCurrentMap())
	{
	case 0:
		m_image->DrawForest(drawBgPosition, drawBgSize);
		break;

	case 1:
		m_image->DrawPlain(drawBgPosition, drawBgSize);
		break;

	case 2:
		m_image->DrawDesrt(drawBgPosition, drawBgSize);
		break;

	case 3:
		m_image->DrawVolcano(drawBgPosition, drawBgSize);
		break;

	case 4:
		m_image->DrawCastle(drawBgPosition, drawBgSize);
		break;

	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		m_image->DrawForest(drawBgPosition, drawBgSize);
		break;
	}



	//戦闘画面の暗幕
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120);

	DrawBox(0, 0, 1280, 720, GetColor(128, 128, 128), TRUE);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);



	//Battle描画
	m_battle->Render(gameOver, map);



	//現在の攻撃対象カーソル
	Enemy* target = m_battle->GetTargetEnemy();

	if (target != nullptr &&target->GetHp() > 0)
	{
		int cursorX = static_cast<int>(target->renderPosition.x)+50;

		int cursorY = static_cast<int>(target->renderPosition.y) - 40;

		DrawString(cursorX, cursorY, L"▼", GetColor(255, 255, 0), TRUE);
	}



	//全滅
	if (m_battle->GetAnnihilation() && !gameOver.IsTitleRequest())
	{
		m_battle->RenderAnnihilation(gameOver);
	}



	//仲間加入画面
	if (m_isJoinRequested)
	{
		m_enemyName = m_enemy->GetName();

		m_image->DrawCommandbox1(drawCommandBoxPosition, drawCommandBoxSize);
		m_battle->RenderCurrentHp();
		if (m_enemy != nullptr)
		{
			m_enemy->RenderBattle();

			m_enemyName = m_enemy->GetName();

			DrawFormatString(180, 480, GetColor(255, 255, 255), L"%lsを\n仲間にしますか？", m_enemyName);
		}

		DrawString(200, 610, L"はい", GetColor(255, 255, 255), TRUE);

		DrawString(300, 610, L"いいえ", GetColor(255, 255, 255), TRUE);

		int cursorX = (m_joinSelect == 0)
			? 180
			: 280;

		DrawString(cursorX, 610, L"▶", GetColor(255, 255, 0), TRUE);
	}



	//モンスター交換画面
	if (m_isReplaceSelect)
	{
		DrawBox(0, 0, 1280, 1280, GetColor(255, 255, 255), TRUE);

		DrawString(500, 400, L"交換する仲間を選んでください", GetColor(0, 0, 0));

		for (int i = 0;i < party.GetMonsterCount();i++)
		{
			const wchar_t* name = L"";

			switch (party.GetMonster(i)->GetType())
			{
			case Monster::Type::Slime:
				name = L"Slime";
				break;

			case Monster::Type::Wolf:
				name = L"Wolf";
				break;

			case Monster::Type::Dragon:
				name = L"Dragon";
				break;
			}

			DrawFormatString(450, 450 + i * 40, GetColor(0, 0, 0), L"%d:%s", i + 1, name);
		}
	}
}



//終了処理
void BattleScene::Finalize()
{
	SetFontSize(20);

	m_battle->Finalize();
}



//状態取得
bool BattleScene::IsFieldRequested()const
{
	return m_isFieldRequested;
}

bool BattleScene::IsJoinRequested()const 
{
	return m_isJoinRequested;
}

bool BattleScene::IsTitleRequested()const 
{
	return m_isTitleRequested;
}



//敵生成
void BattleScene::CreateBattleEnemies(Map& map)
{
	m_battleEnemies.clear();

	if (m_enemyManager == nullptr)
	{
		return;
	}

	//ブレイクレベルによって追加敵の数を決める

	int breakLevel = map.GetBreakLevel();

	int addCount = 0;

	if (breakLevel < 2)
	{
		//ブレイクレベル0～19
		//追加敵なし
		addCount = 0;
	}
	else if (breakLevel < 4)
	{
		//ブレイクレベル20～39
		//追加敵1体
		addCount = 1;
	}
	else
	{
		//ブレイクレベル40以上
		//追加敵2体
		addCount = 2;
	}
	// 逃走成功による追加
	addCount += m_runEnemyBonus;

	// エンカウントする敵1体を含めて最大3体
	if (addCount > 2)
	{
		addCount = 2;
	}

	//追加敵生成
	for (int i = 0;i < addCount;i++)
	{
		Enemy::EnemyType type = static_cast<Enemy::EnemyType>(GetRand(4));

		Enemy* enemy = m_enemyManager->CreateBattleEnemy(map, type);

		if (enemy != nullptr)
		{
			m_battleEnemies.push_back(enemy);
		}
	}
}



//Battleへ敵を設定
void BattleScene::SetBattleEnemies()
{
	m_battle->SetEnemies(m_battleEnemies);
}



//戦闘中の敵位置設定
void BattleScene::SetBattleEnemyPositions()
{
	if (m_battleEnemies.empty())
	{
		return;
	}

	const int y = 100;

	const int width = 200;
	const int height = 200;

	//敵の数
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

	//敵の数によって配置を変更
	std::vector<int>positions;

	if (enemyCount == 1)
	{
		//1体：中央
		positions = { 540};
	}
	else if (enemyCount == 2)
	{
		//2体：左右均等
		positions = { 300,780 };
	}
	else
	{
		//3体：左・中央・右を均等配置
		positions = { 120,540,960};
	}

	//敵を配置
	int positionIndex = 0;

	for (Enemy* enemy : m_battleEnemies)
	{
		if (enemy == nullptr)
		{
			continue;
		}

		if (positionIndex >= static_cast<int>(positions.size()))
		{
			break;
		}

		enemy->renderPosition.x = positions[positionIndex];
		enemy->renderPosition.y = y;

		enemy->renderSize.x = width;
		enemy->renderSize.y = height;

		positionIndex++;
	}

}



//ターゲット取得
Enemy* BattleScene::GetTargetEnemy()const
{
	if (m_battle == nullptr)
	{
		return nullptr;
	}

	return m_battle->GetTargetEnemy();
}



//Setter
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



//タイトル要求
void BattleScene::ResetTitleRequest()
{
	m_isTitleRequested = false;
}



//攻撃履歴
const std::vector<Battle::UsedAttackInfo>& BattleScene::GetUsedAttackOrder()const
{
	return m_battle->GetUsedAttackOrder();
}

void BattleScene::ClearUsedAttackOrder()
{
	m_battle->ClearUsedAttackOrder();
}

