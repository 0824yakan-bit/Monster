#include "pch.h"
#include "BattleScene.h"

#include"Game/Scene/SceneManager.h"
#include"Game/Scene/FieldScene.h"
#include"Game/Player/PlayerManager.h"
#include"Game/Enemy/Enemy.h"
#include"Game/Enemy/EnemyManager.h"

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

BattleScene::BattleScene()
	:m_isFieldRequested{false}
	,m_isReplaceSelect{false}
	,m_battleWin{false}
	,m_enemy{nullptr}
	,m_battle(new Battle())
	,m_teamjoin{}
	,m_receponsTimer{}
	,m_joinSelect{}
	,m_isJoinRequested{false}
	,m_isTitleRequested{false}
	,m_scenemanager{}
	,m_player{}
{
}

BattleScene::~BattleScene()
{
}

void BattleScene::Initialize(InputManager& inputmanager,SceneManager&sceneManager, Map& map,Party&party)
{

	m_battle->SetParty(&party);
	m_battle->Initialize(&sceneManager);

	m_isJoinRequested = false;
	m_isReplaceSelect = false;
	m_isFieldRequested = false;
	m_isTitleRequested = false;
	m_battleWin = false;

	m_receponsTimer = 0;
}

void BattleScene::Update(InputManager& inputManager,SceneManager&sceneManager,FieldScene&fieldScene, GameOver& gameOver, EnemyManager& enemyManager,Map&map, Party& party,PlayerManager&player)
{
	inputManager.Update();
	m_receponsTimer++;
	//printfDx(L"Field=%d Join=%d HP=%d\n",m_isFieldRequested,m_isJoinRequested,m_enemy ? m_enemy->GetHp() : -1);

	if (m_isReplaceSelect)
	{
		if (m_receponsTimer > 15)
		{
			int select = -1;

			if (CheckHitKey(KEY_INPUT_1)) select = 0;
			else if (CheckHitKey(KEY_INPUT_2)) select = 1;
			else if (CheckHitKey(KEY_INPUT_3)) select = 2;
			else if (CheckHitKey(KEY_INPUT_4)) select = 3;

			if (select != -1)
			{
				Monster* learnedMonster = m_pendingMonster.get();

				party.RemoveMonster(select);
				party.AddMonster(std::move(m_pendingMonster));

				for (const auto& atk : learnedMonster->GetAttacks())
				{
					fieldScene.LearnSkill(ToCooperatList(atk.ristics));
				}
			}

		}
		return;
	}


	

	if (m_battle->IsEnemyRequested() && m_enemy && m_enemy->GetHp() <= 0)
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
	if (!m_isJoinRequested && !m_isReplaceSelect && m_enemy)
	{
		m_battle->Update(inputManager ,&sceneManager,gameOver,map,player);
		// GameOverからタイトル要求が来たら
		if (gameOver.IsTitleRequest())
		{
			m_isTitleRequested = true;
			return;
		}
	}

	if (m_battle->IsFieldRequested())
	{
		// 戦闘前の1マス前へ戻す
		if (m_player)
		{
			m_player->m_position = m_player->m_oldposition;
			m_player->m_invicible = true;
		}

		m_isFieldRequested = true;
	}
		if (m_isJoinRequested)
		{
			m_battle->SetJoinWindow(true);

			if (m_receponsTimer > 30)
			{
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
				
				else if (CheckHitKey(KEY_INPUT_RETURN))
				{

					if (m_joinSelect == 0)
					{
						// 仲間にする
						std::unique_ptr<Monster> monster;

						switch (m_enemy->type)
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

						

						if (party.GetMonsterCount() < 4)
						{
							// 追加前に生ポインタを保持
							Monster* learnedMonster = monster.get();

							party.AddMonster(std::move(monster));

							// モンスターが持つ技から属性を習得
							for (const auto& atk : learnedMonster->GetAttacks())
							{
								printfDx(L"Learn %d\n", (int)atk.ristics);
								fieldScene.LearnSkill(ToCooperatList(atk.ristics));
							}

							enemyManager.RemoveEnemy(m_enemy);
							m_enemy = nullptr;

							m_isJoinRequested = false;
							m_isFieldRequested = true;
						}
						else
						{
							m_pendingMonster = std::move(monster);

							m_isJoinRequested = false;
							m_isReplaceSelect = true;

						}
					}
					else//m_joinSelect=1
					{
						// 仲間にしない
						enemyManager.RemoveEnemy(m_enemy);
						m_enemy = nullptr;

						m_isJoinRequested = false;
						m_isFieldRequested = true;

					}
					

					m_receponsTimer = 0;

					return;
				}
			}
		}
		






		/////
		if (CheckHitKey(KEY_INPUT_D))
		{
			if (party.GetMonsterCount() > 0)
			{
				for (int i = 0; i < party.GetMonsterCount(); i++)
				{
					Monster* monster = party.GetMonster(i);

					const wchar_t* name = L"";

					switch (monster->GetType())
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

					//printfDx(L"[%d] %s\n", i, name);

					auto& attacks = monster->GetAttacks();
					for (int j = 0; j < attacks.size(); j++)
					{
						//printfDx(L"技%d : %s  威力=%d\n",	j,	attacks[j].name,attacks[j].power);
					}
				}
			}
			else
			{
				printfDx(L"仲間はいません\n");
			}
	}

}
void BattleScene::Render(GameOver& gameOver,Party&party)
{
	if (!m_enemy)
	{
		return;
	}
	switch (m_enemy->type)
	{
	case Enemy::EnemyType::Slime:
		DrawBox(0, 0, 1280, 720, GetColor(128, 128, 128), TRUE);
		break;

	case Enemy::EnemyType::Wolf:
		DrawBox(0, 0, 1280, 720, GetColor(255, 0, 0), TRUE);
		break;

	case Enemy::EnemyType::Dragon:
		DrawBox(0, 0, 1280, 720, GetColor(255, 255, 0), TRUE);
		break;
	}

	m_battle->Render(gameOver);

	if (m_battle->GetAnnihilation())
	{
		m_battle->RenderAnnihilation(gameOver);
	}

	if (m_isJoinRequested)
	{
		m_enemyName = m_enemy->GetName();
		DrawBox(40, 530, 1240, 690, GetColor(0, 0, 0), TRUE);
		DrawFormatString(60,550, GetColor(255, 255, 255), L"%lsを",m_enemyName);
		DrawString(60 , 610, L"仲間にしますか？",GetColor(255,255,255), TRUE);
		DrawString(520, 610, L"はい", GetColor(255, 255, 255), TRUE);
		DrawString(720, 610, L"いいえ", GetColor(255, 255, 255), TRUE);
		
		int cursorX = (m_joinSelect == 0) ? 470 : 670;

		DrawString(cursorX, 610, L"▶", GetColor(255, 255, 0), TRUE);
	}
	if (m_isReplaceSelect)
	{
		DrawBox(0,0,1280,1280, GetColor(255, 255, 255), TRUE);
		DrawString(500, 400, L"交換する仲間を選んでください", GetColor(0, 0, 0));
		printfDx(L"交換する仲間を選んでください\n");

		for (int i = 0; i < party.GetMonsterCount(); i++)
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

			DrawFormatString(450,450 + i * 40,GetColor(0, 0, 0),L"%d : %s",i + 1,name);
		}

	}
	

}

void BattleScene::Finalize()
{
	SetFontSize(20);
	m_battle->Finalize();
}

bool BattleScene::IsFieldRequested()const
{
	return m_isFieldRequested;
}

bool BattleScene::IsJoinRequested()const
{
	return m_isJoinRequested;
}


void BattleScene::SetPlayer(PlayerManager* player)
{
	m_player = player;
	m_battle->SetPlayer(player);
}

void BattleScene::SetEnemy(Enemy* enemy)
{
	m_enemy = enemy;
	m_battle->SetEnemy(enemy);
}

bool BattleScene::IsTitleRequested()const
{
	return m_isTitleRequested;
}
void BattleScene::ResetTitleRequest()
{
	m_isTitleRequested = false;
}

const std::vector<Battle::UsedAttackInfo>&BattleScene::GetUsedAttackOrder() const
{
	return m_battle->GetUsedAttackOrder();
}

void BattleScene::ClearUsedAttackOrder()
{
	m_battle->ClearUsedAttackOrder();
}