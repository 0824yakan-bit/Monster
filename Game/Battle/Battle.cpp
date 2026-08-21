#include "pch.h"

#include "Game/Battle/Battle.h"

#include "Game/Scene/SceneManager.h"
#include "Game/Party/Party.h"



// Constructor / Destructor


Battle::Battle()
: m_receponsTimer{}
, m_displaytextTimer{}
, m_Window{}
, m_windowWidth{}
, m_windowWidthFront{}
, m_IsActive{}
, m_isJoinWindow{}
, m_select{}
, m_state{}
, m_displayIndex{}
, m_attackSelect{}
, m_monsterSelect{}
, m_monsterhp{}
, m_isRun{}
, m_isFieldRequested{}
, m_isEnemyRequested{}
, m_annihilation{}
{
}

Battle::~Battle()
{
}



// Initialize / Finalize


void Battle::Initialize(SceneManager* sceneManager)
{
	m_usedAttackOrder.clear();

	// 背景
	drawBgPosition.x = 10;
	drawBgPosition.y = 10;

	drawBgSize.x = 1260;
	drawBgSize.y = 500;

	//コマンドボックス
	drawCommandBoxPosition1.x =40;
	drawCommandBoxPosition1.y =530;
	drawCommandBoxSize1.x = 300;
	drawCommandBoxSize1.y = 160;

	drawCommandBoxPosition2.x = 20;
	drawCommandBoxPosition2.y = 480;
	drawCommandBoxSize2.x = 1240;
	drawCommandBoxSize2.y = 230;

	// 選択状態
	m_select = 0;

	m_targetEnemyIndex = -1;
	m_selectedTargetEnemyIndex = -1;

	m_displayIndex = 0;
	m_monsterSelect = 0;
	m_attackSelect = 0;



	// 戦闘状態
	m_state = BattleState::Command;

	m_IsActive = true;
	m_Window = false;
	m_isJoinWindow = false;



	// ウィンドウ
	m_windowWidth = 0;
	m_windowHeight = 0;
	m_windowWidthFront = 0;



	// タイマー
	m_receponsTimer = 10;
	m_displaytextTimer = 0;



	// リクエスト
	m_isFieldRequested = false;
	m_isEnemyRequested = false;



	// 敵死亡演出
	m_enemyDeadMotion = false;
	m_enemyDeadOffsetY = 0;



	// コンボ状態
	state = USED_NONE;



	// 選択した技
	m_selectedAttack.resize(m_party->GetMonsterCount(), -1);



	// 味方HPを保存
	for (int i = 0; i < m_party->GetMonsterCount(); i++)
	{
		Monster* monster = m_party->GetMonster(i);

		m_monsterhp[i] =
			monster->GetCurrentHitPoint();
	}



	// 防御状態をリセット
	for (int i = 0; i < MAX_PARTY; i++)
	{
		m_requestDefense[i] = false;
	}



	// 全滅状態
	m_annihilation = false;
}


void Battle::Finalize()
{
}



// Update


void Battle::Update(InputManager& inputManager, SceneManager* sceneManager, GameOver& gameOver, Map& map, PlayerManager& player)
{

	// 全滅判定


	m_annihilation = true;

	for (int i = 0; i < m_party->GetMonsterCount(); i++)
	{
		if (m_monsterhp[i] != 0)
		{
			m_annihilation = false;
			break;
		}
	}

	if (m_annihilation)
	{
		// 最初のフレームでは状態だけ切り替える
		if (m_state != BattleState::Annihilation)
		{
			m_state = BattleState::Annihilation;
			m_displaytextTimer = 0;

			return;
		}
	}
	else
	{

		// 入力受付タイマー


		m_receponsTimer++;

		if (m_receponsTimer > 15)
		{

			// コマンド選択中の入力


			if (m_state == BattleState::Command && m_IsActive)
			{
				// 右
				if (CheckHitKey(KEY_INPUT_RIGHT))
				{
					m_select++;

					if (m_select >= COMMAND_NUM)
					{
						m_select = 0;
					}

					m_receponsTimer = 0;
				}

				// 左
				if (CheckHitKey(KEY_INPUT_LEFT))
				{
					m_select--;

					if (m_select < 0)
					{
						m_select = COMMAND_NUM - 1;
					}

					m_receponsTimer = 0;
				}

				// 決定
				if (CheckHitKey(KEY_INPUT_RETURN))
				{
					m_receponsTimer = 0;

					m_IsActive = false;
					m_Window = true;

					switch (m_select)
					{
					case 0:
						// 攻撃
						m_windowWidth = 0;
						m_windowWidthFront = 0;
						m_windowHeight = 0;
						m_state = BattleState::AttackSelect;

						// 現在のターゲットをカーソル位置にする
						m_targetEnemyIndex = GetValidTargetIndex(m_targetEnemyIndex);

						break;


					case 1:
						// 道具
						m_windowWidth = 0;
						m_windowWidthFront = 0;
						m_state = BattleState::Tool;

						break;


					case 2:
						// スカウト
						m_windowWidth = 0;
						m_windowWidthFront = 0;
						m_state = BattleState::Scout;

						break;


					case 3:
						// 逃げる
						m_state = BattleState::Run;

						break;


					case 4:
						// 敵ターン
						m_state = BattleState::EnemyTurn;

						break;
					}
				}



				// 戻る処理


				if (!m_IsActive && m_state == BattleState::AttackSelect && CheckHitKey(KEY_INPUT_BACK) && m_monsterSelect == 0)
				{
					m_state = BattleState::Command;

					m_IsActive = true;
					m_Window = false;

					m_windowWidth = 0;
					m_windowWidthFront = 0;
				}


			}
		}
	}



	// State Update
	switch (m_state)
	{
	case BattleState::Command:
		break;


	case BattleState::AttackSelect:
		UpdateAttackSelect();
		break;


	case BattleState::AttackAction:
		UpdateAttackAction(map, player);
		break;


	case BattleState::Tool:
		UpdateTool();
		break;


	case BattleState::Scout:
		UpdateScout();
		break;


	case BattleState::Run:
		UpdateRun();
		break;


	case BattleState::EnemyTurn:
		UpdateEnemyTurn(sceneManager);
		break;


	case BattleState::EnemyDead:
		UpdateEnemyDead();
		break;


	case BattleState::Annihilation:
		UpdateAnnihilation(gameOver, inputManager);
		break;
	}



	// 攻撃エフェクト更新


	if (m_playAttackEffect)
	{
		m_attackEffectTimer++;

		if (m_attackEffectTimer >= ATTACK_EFFECT_DURATION)
		{
			m_playAttackEffect = false;
		}
	}
}


// Render
void Battle::Render(GameOver& gameOver, Map& map)
{

	// 戦闘画面のベース
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 50);

	// 戦闘画面
	DrawBox(10, 10, 1270, 500, GetColor(255, 255, 255), TRUE);

	// 敵配置
	DrawBox(50, 50, 1230, 460, GetColor(255, 0, 0), FALSE);

	// コマンド選択位置
	DrawBox(20, 520, 1260, 700, GetColor(255, 255, 255), TRUE);


	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);



	//// 背景(コマンド選択ボックスの上表示)
	//switch (map.GetCurrentMap())
	//{
	//case 0:
	//	m_image->DrawForest(drawBgPosition, drawBgSize);
	//	break;


	//case 1:
	//	m_image->DrawPlain(drawBgPosition, drawBgSize);
	//	break;


	//case 2:
	//	m_image->DrawRiver(drawBgPosition, drawBgSize);
	//	break;


	//case 3:
	//	m_image->DrawVolcano(drawBgPosition, drawBgSize);
	//	break;


	//case 4:
	//	m_image->DrawCastle(drawBgPosition, drawBgSize);
	//	break;


	//case 5:
	//case 6:
	//case 7:
	//case 8:
	//case 9:
	//	m_image->DrawForest(drawBgPosition, drawBgSize);
	//	break;
	//}



	// パーティHP表示
	for (int i = 0; i < MAX_PARTY; i++)
	{
		DrawFormatString(30 + (i * 200), 30, GetColor(0, 0, 0), L"%d", m_monsterhp[i]);
	}



	// State Render
	switch (m_state)
	{
	case BattleState::Command:
		RenderCommand();
		break;


	case BattleState::AttackSelect:
		RenderAttackSelect();
		break;


	case BattleState::AttackAction:
		RenderAttackAction();
		break;


	case BattleState::Tool:
		RenderTool();
		break;


	case BattleState::Scout:
		RenderScout();
		break;


	case BattleState::Run:
		RenderRun();
		break;


	case BattleState::EnemyTurn:
		RenderEnemyTurn();
		break;


	case BattleState::EnemyDead:
		RenderEnemyDead();
		break;


	case BattleState::Annihilation:
		if (!gameOver.IsTitleRequest())
		{
			RenderAnnihilation(gameOver);
		}
		break;
	}



	// ターゲット取得
	Enemy* enemy = GetTargetEnemy();



	// 敵描画
	for (Enemy* currentEnemy : m_enemies)
	{
		if (currentEnemy == nullptr)
		{
			continue;
		}


		// 死亡演出中
		if (currentEnemy == m_deadEnemy && m_enemyDeadMotion)
		{
			SetDrawArea(0, m_enemyDeadOffsetY, 1280, 720 + m_enemyDeadOffsetY);

			// 倒された敵だけ点滅
			if ((m_displaytextTimer / 5) % 2 == 0)
			{
				currentEnemy->RenderBattle();
			}

			SetDrawArea(0, 0, 1280, 720);

			continue;
		}


		// HP0の敵は通常描画しない
		if (currentEnemy->GetHp() <= 0)
		{
			continue;
		}


		// 生きている敵
		currentEnemy->RenderBattle();
	}



	// 攻撃エフェクト描画
	if (m_playAttackEffect)
	{
		// 残り時間から透明度を計算
		float rate = 1.0f - (float)m_attackEffectTimer / ATTACK_EFFECT_DURATION;
		int alpha = (int)(180 * rate);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);


		switch (m_effectElement)
		{
		case Monster::CharacteRistics::Fire:
			DrawBox(0, 0, 1280, 720, GetColor(255, 80, 0), TRUE);
			break;


		case Monster::CharacteRistics::Water:
			DrawBox(0, 0, 1280, 720, GetColor(0, 120, 255), TRUE);
			break;


		case Monster::CharacteRistics::Grass:
			DrawBox(0, 0, 1280, 720, GetColor(0, 200, 0), TRUE);
			break;


		case Monster::CharacteRistics::Soil:
			DrawBox(0, 0, 1280, 720, GetColor(139, 69, 19), TRUE);
			break;


		case Monster::CharacteRistics::Wind:
			DrawBox(0, 0, 1280, 720, GetColor(180, 180, 180), TRUE);
			break;


		case Monster::CharacteRistics::Thunder:
			DrawBox(0, 0, 1280, 720, GetColor(255, 255, 0), TRUE);
			break;


		default:
			break;
		}


		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}


	// ターゲット敵HP
	if (enemy != nullptr)
	{
		DrawFormatString(500, 120, GetColor(0, 255, 255), L"HP : %d", enemy->GetHp());
	}
	else
	{
		DrawString(500, 120, L"HP : ---", GetColor(128, 128, 128));
	}



	// 使用済み属性表示
	DrawFormatString(20, 300, GetColor(0, 0, 0), L"N:%d F:%d Wa:%d G:%d S:%d T:%d Wi:%d",
		(state & USED_NORMAL) ? 1 : 0,
		(state & USED_FIRE) ? 1 : 0,
		(state & USED_WATER) ? 1 : 0,
		(state & USED_GRASS) ? 1 : 0,
		(state & USED_SOIL) ? 1 : 0,
		(state & USED_THUNDER) ? 1 : 0,
		(state & USED_WIND) ? 1 : 0
	);
}



// Command
void Battle::RenderCommand()
{
	if (!m_IsActive)
	{
		return;
	}
	for (int i = 0;i < 4;i++)
	{
		drawCommandBoxPosition1.x = 40 + 300 * i;
		m_image->DrawCommandbox1(drawCommandBoxPosition1, drawCommandBoxSize1);
	}
	//DrawBox(40, 530, 320, 690, GetColor(0, 0, 0), TRUE);

	//DrawBox(340, 530, 630, 690, GetColor(0, 0, 0), TRUE);

	//DrawBox(650, 530, 930, 690, GetColor(0, 0, 0), TRUE);

	//DrawBox(950, 530, 1240, 690, GetColor(0, 0, 0), TRUE);


	SetFontSize(50);


	for (int i = 0; i < COMMAND_NUM; i++)
	{
		DrawString(100 + i * 300, 580, m_command[i], GetColor(255, 255, 255));
	}


	// カーソル
	DrawString(70 + m_select * 300, 580, L"▶", GetColor(255, 255, 0));
}



// Attack Select


void Battle::UpdateAttackSelect()
{
	if (m_party == nullptr)
	{
		return;
	}

	if (m_party->GetMonsterCount() == 0)
	{
		return;
	}

	if (m_state != BattleState::AttackSelect)
	{
		return;
	}



	// 戻る
	if (m_receponsTimer > 25 && CheckHitKey(KEY_INPUT_BACK))
	{
		m_receponsTimer = 0;



		// 1体目ならコマンド画面へ戻る
		if (m_monsterSelect == 0)
		{
			m_state = BattleState::Command;

			m_IsActive = true;
			m_Window = false;

			m_windowWidth = 0;
			m_windowWidthFront = 0;

			m_attackSelect = 0;


			std::fill(m_selectedAttack.begin(), m_selectedAttack.end(), -1);

			return;
		}



		// 2体目以降なら1体前へ戻る


		do
		{
			m_monsterSelect--;

		} while (m_monsterSelect >= 0 && m_monsterhp[m_monsterSelect] <= 0);


		if (m_monsterSelect >= 0)
		{
			m_selectedAttack[m_monsterSelect] = -1;
		}
		else
		{
			m_monsterSelect = 0;
		}


		m_attackSelect = 0;

		return;
	}



	// 選択終了
	if (m_monsterSelect >= m_party->GetMonsterCount())
	{
		m_displayIndex = 0;
		m_displaytextTimer = 0;

		m_state = BattleState::AttackAction;

		return;
	}



	// HP0の仲間を自動で飛ばす


	while (m_monsterSelect < m_party->GetMonsterCount() && m_monsterhp[m_monsterSelect] <= 0)
	{
		m_monsterSelect++;
	}



	// 全員選択済み


	if (m_monsterSelect >= m_party->GetMonsterCount())
	{
		m_displayIndex = 0;
		m_displaytextTimer = 0;

		m_state = BattleState::AttackAction;

		return;
	}



	// 現在選択中モンスターの技
	auto& attacks = m_party->GetMonster(m_monsterSelect)->GetAttacks();


	if (m_receponsTimer > 25)
	{

		// 左：敵ターゲット変更
		if (CheckHitKey(KEY_INPUT_LEFT))
		{
			m_targetEnemyIndex = GetValidTargetIndex(m_targetEnemyIndex - 1);

			m_receponsTimer = 0;

			printfDx(L"LEFT -> Cursor[%d]\n", m_targetEnemyIndex);
		}



		// 右：敵ターゲット変更
		if (CheckHitKey(KEY_INPUT_RIGHT))
		{
			m_targetEnemyIndex = GetValidTargetIndex(m_targetEnemyIndex + 1);

			m_receponsTimer = 0;

			printfDx(L"RIGHT -> Cursor[%d]\n", m_targetEnemyIndex);
		}



		// 下：技選択
		if (CheckHitKey(KEY_INPUT_DOWN))
		{
			m_attackSelect++;
			m_receponsTimer = 0;

			if (m_attackSelect >= attacks.size())
			{
				m_attackSelect = 0;
			}
		}



		// 上：技選択
		if (CheckHitKey(KEY_INPUT_UP))
		{
			m_attackSelect--;
			m_receponsTimer = 0;

			if (m_attackSelect < 0)
			{
				m_attackSelect = (int)attacks.size() - 1;
			}
		}



		// 決定
		if (CheckHitKey(KEY_INPUT_RETURN))
		{
			m_receponsTimer = 0;

			int targetIndex = GetValidTargetIndex(m_targetEnemyIndex);


			if (targetIndex < 0)
			{
				return;
			}


			// 現在カーソル位置を確定
			m_targetEnemyIndex = targetIndex;
			m_selectedTargetEnemyIndex = targetIndex;


			// 技を確定
			m_selectedAttack[m_monsterSelect] = m_attackSelect;


			printfDx(L"SELECT Enemy[%d] : %ls HP=%d\n", targetIndex, m_enemies[targetIndex]->GetName(), m_enemies[targetIndex]->GetHp());


			// 次のモンスターへ
			m_monsterSelect++;

			m_attackSelect = 0;
		}
	}
}


void Battle::RenderAttackSelect()
{
	if (m_party == nullptr)
	{
		return;
	}

	if (m_party->GetMonsterCount() == 0)
	{
		return;
	}
	// ウィンドウの中心
	int centerY = 600;


	// 上下に広がるためのY座標
	int topY = centerY - m_windowHeight / 2;


	Vector2 windowPosition;

	windowPosition.x = 20;
	windowPosition.y = topY;


	Vector2 windowSize;

	windowSize.x = 1240;
	windowSize.y = m_windowHeight;

	m_image->DrawCommandbox2(drawCommandBoxPosition2, drawCommandBoxSize2);
	//DrawBox(40, 530, 320 + m_windowWidth, 690, GetColor(0, 0, 0), TRUE);


	for (int m = 0;m < m_party->GetMonsterCount();m++)
	{
		Monster* monster = m_party->GetMonster(m);

		std::wstring name = monster->GetName();

		auto& attacks = monster->GetAttacks();

		int x = 60 + m * 300;


		// 仲間画面中は技を描画しない
		if (m_isJoinWindow)
		{
			return;
		}



		// 技一覧


		for (int i = 0;i < attacks.size();i++)
		{
			int color = GetColor(255, 255, 255);


			// 選択済み
			if (m_selectedAttack[m] == i)
			{
				color = GetColor(255, 255, 0);
			}


			// モンスター名
			DrawString(x, 500, name.c_str(), GetColor(0, 0, 255));


			// 技名
			DrawString(x, 550 + i * 40, attacks[i].name, color);


			// 現在のカーソル
			if (m == m_monsterSelect && i == m_attackSelect)
			{
				DrawString(x - 25, 550 + i * 40, L"▶", GetColor(255, 255, 0));
			}
		}
	}
}



// Attack Action


void Battle::UpdateAttackAction(Map& map, PlayerManager& player)
{

	// 選択済みターゲット取得
	Enemy* enemy = nullptr;

	if (m_selectedTargetEnemyIndex >= 0 && m_selectedTargetEnemyIndex < static_cast<int>(m_enemies.size()))
	{
		enemy = m_enemies[m_selectedTargetEnemyIndex];
	}


	if (enemy == nullptr)
	{
		EndTurn();
		return;
	}



	// コンボ処理


	if (m_comboPending)
	{
		m_displaytextTimer++;


		// コンボ発動
		if (m_displaytextTimer == 1)
		{
			UesElementalAttack(map, player);
		}


		// 1秒表示
		if (m_displaytextTimer < 60)
		{
			return;
		}



		// 敵全滅判定
		if (AreAllEnemiesDead())
		{
			m_deadEnemy = enemy;
			m_deadEnemyName = enemy->GetName();

			m_enemyDeadMotion = true;
			m_enemyDeadOffsetY = 0;

			m_displaytextTimer = 0;

			m_state =
				BattleState::EnemyDead;

			return;
		}



		// 次のターン
		m_comboPending = false;

		m_displayMessage.clear();
		m_displayIndex = 0;
		m_monsterSelect = 0;

		std::fill(m_selectedAttack.begin(), m_selectedAttack.end(), -1);

		m_state =
			BattleState::EnemyTurn;

		m_displaytextTimer = 0;

		return;
	}



	// コンボ条件確認


	bool hasFire = false;
	bool hasWater = false;
	bool hasGrass = false;
	bool hasSoil = false;


	for (int i = 0;i < m_party->GetMonsterCount();i++)
	{
		if (m_selectedAttack[i] < 0)
		{
			continue;
		}


		auto& attacks = m_party->GetMonster(i)->GetAttacks();

		auto type = attacks[m_selectedAttack[i]].ristics;


		if (type == Monster::CharacteRistics::Fire)
		{
			hasFire = true;
		}


		if (type == Monster::CharacteRistics::Water)
		{
			hasWater = true;
		}


		if (type == Monster::CharacteRistics::Grass)
		{
			hasGrass = true;
		}


		if (type == Monster::CharacteRistics::Soil)
		{
			hasSoil = true;
		}
	}


	bool steamCombo = hasFire && hasWater;
	bool floorCombo = hasWater && hasSoil;

	// HP0 または未選択の仲間を飛ばす


	while (m_displayIndex < m_party->GetMonsterCount() && (m_monsterhp[m_displayIndex] <= 0 || m_selectedAttack[m_displayIndex] < 0))
	{
		m_displayIndex++;
	}



	// 次のモンスターへ


	if (m_displaytextTimer >= 60)
	{
		m_displaytextTimer = 0;
		m_displayIndex++;


		if (m_displayIndex >= m_party->GetMonsterCount())
		{
			m_comboPending = true;
			m_displaytextTimer = 0;

			return;
		}
	}


	m_displaytextTimer++;



	// 攻撃開始
	if (m_displaytextTimer == 1)
	{
		Monster* monster = m_party->GetMonster(m_displayIndex);


		auto& attacks = monster->GetAttacks();

		int index = m_selectedAttack[m_displayIndex];


		// 念のため範囲チェック
		if (index < 0 || index >= (int)attacks.size())
		{
			m_displayIndex++;
			m_displaytextTimer = 0;

			return;
		}



		// 属性取得
		m_characteRistics = attacks[index].ristics;



		// 攻撃エフェクト開始
		m_effectElement = m_characteRistics;

		m_playAttackEffect = true;
		m_attackEffectTimer = 0;


		printfDx(L"Attack=%ls Type=%d", attacks[index].name, (int)m_characteRistics);



		// 発動順を保存
		UsedAttackInfo info;

		info.element = m_characteRistics;

		info.attackName = attacks[index].name;

		m_usedAttackOrder.push_back(info);



		// 攻撃倍率
		float magnification = 1.0f;


		switch (m_characteRistics)
		{
		case Monster::CharacteRistics::Normal:

			state |= USED_NORMAL;

			break;


		case Monster::CharacteRistics::Fire:

			state |= USED_FIRE;

			// 火属性は1.5倍
			magnification = 1.5f;

			break;


		case Monster::CharacteRistics::Water:

			state |= USED_WATER;

			break;


		case Monster::CharacteRistics::Grass:

			state |= USED_GRASS;

			break;


		case Monster::CharacteRistics::Soil:

			state |= USED_SOIL;

			break;


		case Monster::CharacteRistics::Thunder:

			state |= USED_THUNDER;

			break;


		case Monster::CharacteRistics::Wind:

			state |= USED_WIND;

			break;


		case Monster::CharacteRistics::Defense:

			// このターン防御
			m_requestDefense[m_displayIndex] = true;

			m_displayMessage = monster->GetName() + L"は身を守っている！";

			break;
		}



		// コンボメンバー判定
		bool isComboMember = IsComboMember(m_characteRistics, steamCombo, floorCombo);



		// 通常攻撃処理
		if (
			m_characteRistics != Monster::CharacteRistics::Defense && !isComboMember)
		{
			Enemy* target = GetSelectedTargetEnemy();


			if (target != nullptr)
			{
				target->Damage(static_cast<int>(attacks[index].power * magnification));
			}



			// 単体属性効果

			switch (m_characteRistics)
			{
			case Monster::CharacteRistics::Normal:
				map.NormalBreak(player);
				break;


			case Monster::CharacteRistics::Fire:
				map.FireBreak(player);
				break;


			case Monster::CharacteRistics::Water:
				map.WaterBreak(player);
				break;


			case Monster::CharacteRistics::Grass:
				map.GrassBreak(player);
				break;


			case Monster::CharacteRistics::Soil:
				map.SoilBreak(player);
				break;


			case Monster::CharacteRistics::Wind:
				map.WindBreak(player);
				break;


			case Monster::CharacteRistics::Thunder:
				map.ThunderBreak(player);
				break;


			default:
				break;
			}
		}
		else if (isComboMember)
		{

			// コンボ準備
			m_displayMessage = monster->GetName() + L"は連携の準備をしている！";
		}



		// 攻撃メッセージ


		if (!isComboMember)
		{
			m_displayMessage = monster->GetName() + L"の" + attacks[index].name + L"！";
		}
	}



	// 攻撃表示中


	if (m_displaytextTimer < 60)
	{
		return;
	}



	// 敵死亡判定


	if (m_selectedTargetEnemyIndex >= 0 && m_selectedTargetEnemyIndex < static_cast<int>(m_enemies.size()))
	{
		Enemy* target = m_enemies[m_selectedTargetEnemyIndex];


		if (target != nullptr &&
			target->GetHp() <= 0)
		{
			m_deadEnemy = target;
			m_deadEnemyName = target->GetName();

			m_enemyDeadMotion = true;
			m_enemyDeadOffsetY = 0;

			m_displaytextTimer = 0;

			m_state =
				BattleState::EnemyDead;

			return;
		}
	}
}


void Battle::RenderAttackAction()
{
	if (m_displayMessage.empty())
	{
		return;
	}


	DrawBox(40, 530, 1240, 690, GetColor(0, 0, 0), TRUE);


	DrawString(50, 550, m_displayMessage.c_str(), GetColor(255, 255, 255));
}



// Tool


void Battle::UpdateTool()
{
}


void Battle::RenderTool()
{
	if (m_windowWidthFront > 300)
	{
		m_windowWidthFront = 300;
	}

	if (m_windowWidth > 610)
	{
		m_windowWidth = 610;
	}


	DrawBox(340 - m_windowWidthFront, 530, 630 + m_windowWidth, 690, GetColor(0, 0, 0), TRUE);
}



// Scout


void Battle::UpdateScout()
{
}


void Battle::RenderScout()
{
	if (m_windowWidthFront > 610)
	{
		m_windowWidthFront = 610;
	}

	if (m_windowWidth > 310)
	{
		m_windowWidth = 310;
	}


	DrawBox(650 - m_windowWidthFront, 530, 930 + m_windowWidth, 690, GetColor(0, 0, 0), TRUE);


	DrawString(40, 580, L"スカウト", GetColor(255, 255, 255));
}



// Run


void Battle::UpdateRun()
{
	// フィールドへ戻す
	m_player->m_position = m_player->m_currentposition;


	m_isEnemyRequested = true;
	m_isFieldRequested = true;

	m_state =
		BattleState::Command;
}


void Battle::RenderRun()
{
	DrawBox(950 - m_windowWidthFront, 530, 1240, 690, GetColor(0, 0, 0), TRUE);
}



// Enemy Turn


void Battle::UpdateEnemyTurn(SceneManager* sceneManager)
{
	Enemy* enemy = GetTargetEnemy();


	if (enemy == nullptr)
	{
		EndTurn();
		return;
	}


	m_displaytextTimer++;



	// 敵攻撃メッセージ


	if (m_displaytextTimer <= 60)
	{
		m_displayMessage = std::wstring(enemy->GetName()) + L"の攻撃!!";
	}



	// 全体攻撃
	else if (m_displaytextTimer <= 120)
	{
		m_displayMessage = L"全体に";


		if (m_displaytextTimer == 120)
		{
			for (int i = 0;i < m_party->GetMonsterCount();i++)
			{
				Monster* monster = m_party->GetMonster(i);


				int damage = enemy->GetPower();


				// 防御中なら半減
				if (m_requestDefense[i])
				{
					damage /= 2;
				}


				monster->Damage(damage);


				m_monsterhp[i] = monster->GetCurrentHitPoint();


				if (m_monsterhp[i] < 0)
				{
					m_monsterhp[i] = 0;
				}
			}


			EndTurn();
		}
	}
}


void Battle::RenderEnemyTurn()
{
	DrawBox(40, 530, 1240, 690, GetColor(0, 0, 0), TRUE);


	DrawString(60, 550, m_displayMessage.c_str(), GetColor(255, 255, 255));


	DrawString(20, 500, L"EnemyTURN", GetColor(32, 132, 43), TRUE);
}



// Enemy Dead


void Battle::UpdateEnemyDead()
{
	m_displaytextTimer++;



	// 0～29フレーム：やられモーション


	if (m_displaytextTimer < 30)
	{
		m_displayMessage.clear();

		m_enemyDeadOffsetY += 3;

		return;
	}



	// 30フレーム目


	if (m_displaytextTimer == 30)
	{
		m_displayMessage = m_deadEnemyName + L"を倒した!";
	}



	// 90フレーム経過


	if (m_displaytextTimer >= 90)
	{
		m_enemyDeadMotion = false;
		m_enemyDeadOffsetY = 0;

		m_deadEnemy = nullptr;

		m_displaytextTimer = 0;



		// 全員倒した


		if (AreAllEnemiesDead())
		{
			m_isEnemyRequested = true;

			return;
		}



		// 生きている敵をターゲットにする


		m_targetEnemyIndex = -1;


		for (int i = 0;i < static_cast<int>(m_enemies.size());i++)
		{
			Enemy* enemy = m_enemies[i];


			if (enemy != nullptr && enemy->GetHp() > 0)
			{
				m_targetEnemyIndex = i;
				break;
			}
		}



		// 戦闘続行


		m_state =
			BattleState::Command;


		m_IsActive = true;
		m_Window = false;


		m_windowWidth = 0;
		m_windowWidthFront = 0;


		m_select = 0;
		m_monsterSelect = 0;
		m_attackSelect = 0;
		m_displayIndex = 0;


		m_displayMessage.clear();
	}
}


void Battle::RenderEnemyDead()
{
	DrawBox(40, 530, 1240, 690, GetColor(0, 0, 0), TRUE);


	DrawString(60, 550, m_displayMessage.c_str(), GetColor(255, 255, 255));
}



// Annihilation


void Battle::UpdateAnnihilation(GameOver& gameOver, InputManager& inputManager)
{
	gameOver.Update(inputManager);
}

void Battle::RenderAnnihilation(GameOver& gameOver)
{
	gameOver.Render();
}



// Turn End


void Battle::EndTurn()
{
	m_state = BattleState::Command;


	m_IsActive = true;
	m_Window = false;


	m_windowWidth = 0;
	m_windowWidthFront = 0;


	m_select = 0;



	// 防御状態リセット


	for (int i = 0; i < MAX_PARTY; i++)
	{
		m_requestDefense[i] = false;
	}



	// 選択技リセット


	std::fill(m_selectedAttack.begin(), m_selectedAttack.end(), -1);


	m_monsterSelect = 0;
	m_attackSelect = 0;
	m_displayIndex = 0;



	// 次ターンのターゲット


	m_selectedTargetEnemyIndex = m_targetEnemyIndex;
}



// Target


void Battle::SetTargetEnemyIndex(int index)
{
	m_targetEnemyIndex = GetValidTargetIndex(index);
}


int Battle::GetTargetEnemyIndex() const
{
	return m_targetEnemyIndex;
}


Enemy* Battle::GetTargetEnemy()
{
	if (m_enemies.empty())
	{
		return nullptr;
	}


	if (m_targetEnemyIndex < 0 || m_targetEnemyIndex >= static_cast<int>(m_enemies.size()))
	{
		return nullptr;
	}


	Enemy* enemy = m_enemies[m_targetEnemyIndex];


	if (enemy == nullptr)
	{
		return nullptr;
	}


	if (enemy->GetHp() <= 0)
	{
		return nullptr;
	}


	return enemy;
}


Enemy* Battle::GetSelectedTargetEnemy()
{
if (m_enemies.empty())
{
return nullptr;
}


if (m_selectedTargetEnemyIndex < 0 ||m_selectedTargetEnemyIndex >=static_cast<int>(m_enemies.size()))
{
return nullptr;
}


Enemy* enemy =m_enemies[m_selectedTargetEnemyIndex];


if (enemy == nullptr)
{
return nullptr;
}


if (enemy->GetHp() <= 0)
{
return nullptr;
}


return enemy;
}


int Battle::GetValidTargetIndex(int index) const
{
	if (m_enemies.empty())
	{
		return -1;
	}


	const int count = static_cast<int>(m_enemies.size());



	// 範囲調整


	if (index < 0)
	{
		index = 0;
	}


	if (index >= count)
	{
		index = count - 1;
	}



	// 指定位置が生存していればそのまま


	if (m_enemies[index] != nullptr && m_enemies[index]->GetHp() > 0)
	{
		return index;
	}



	// 死亡していた場合、右方向に検索


	for (int i = 1; i <= count; i++)
	{
		int searchIndex = (index + i) % count;


		if (m_enemies[searchIndex] != nullptr && m_enemies[searchIndex]->GetHp() > 0)
		{
			return searchIndex;
		}
	}


	return -1;
}



// Enemy Management


bool Battle::AreAllEnemiesDead() const
{
	if (m_enemies.empty())
	{
		return true;
	}


	for (Enemy* enemy : m_enemies)
	{
		if (enemy != nullptr && enemy->GetHp() > 0)
		{
			return false;
		}
	}


	return true;
}


void Battle::RemoveEnemy(Enemy* enemy)
{
	if (enemy == nullptr)
	{
		return;
	}


	int removeIndex = -1;



	// 削除対象検索


	for (int i = 0;i < static_cast<int>(m_enemies.size());i++)
	{
		if (m_enemies[i] == enemy)
		{
			removeIndex = i;
			break;
		}
	}


	if (removeIndex == -1)
	{
		return;
	}



	// 敵削除


	m_enemies.erase(m_enemies.begin() + removeIndex);



	// 敵がいなくなった


	if (m_enemies.empty())
	{
		m_targetEnemyIndex = -1;
		m_selectedTargetEnemyIndex = -1;

		return;
	}



	// 実ターゲット調整


	if (m_targetEnemyIndex > removeIndex)
	{
		m_targetEnemyIndex--;
	}
	else if (m_targetEnemyIndex == removeIndex)
	{
		m_targetEnemyIndex = -1;
	}



	// 選択中ターゲット調整


	if (m_selectedTargetEnemyIndex > removeIndex)
	{
		m_selectedTargetEnemyIndex--;
	}
	else if (m_selectedTargetEnemyIndex == removeIndex)
	{
		m_selectedTargetEnemyIndex = -1;
	}



	// 実ターゲット再設定


	if (m_targetEnemyIndex == -1)
	{
		SetTargetEnemyIndex(0);
	}



	// 選択ターゲット再設定


	if (m_selectedTargetEnemyIndex == -1)
	{
		m_selectedTargetEnemyIndex = m_targetEnemyIndex;
	}
}


void Battle::SetEnemies(const std::vector<Enemy*>& enemies)
{
	m_enemies = enemies;


	m_targetEnemyIndex = -1;
	m_selectedTargetEnemyIndex = -1;



	// 中央の敵を優先


	if (m_enemies.size() > 1 && m_enemies[1] != nullptr && m_enemies[1]->GetHp() > 0)
	{
		m_targetEnemyIndex = 1;
	}
	else
	{
		for (int i = 0;i < static_cast<int>(m_enemies.size());i++)
		{
			if (m_enemies[i] != nullptr && m_enemies[i]->GetHp() > 0)
			{
				m_targetEnemyIndex = i;
				break;
			}
		}
	}


	// 初期ターゲットを確定ターゲットにもする
	m_selectedTargetEnemyIndex = m_targetEnemyIndex;


	printfDx(L"SetEnemies : target=%d selected=%d\n", m_targetEnemyIndex, m_selectedTargetEnemyIndex);
}



// Setter
void Battle::SetImage(ImageManager* image)
{
m_image = image;
}


void Battle::SetPlayer(PlayerManager* player)
{
m_player = player;
}


void Battle::SetParty(Party* party)
{
m_party = party;
}


void Battle::SetJoinWindow(bool flag)
{
m_isJoinWindow = flag;
}



// Request / State Getter


bool Battle::IsFieldRequested()
{
return m_isFieldRequested;
}


bool Battle::IsEnemyRequested()
{
return m_isEnemyRequested;
}


bool Battle::GetAnnihilation()
{
return m_annihilation;
}

void Battle::DamageAllEnemies(int damage)
{
	for (Enemy* enemy : m_enemies)
	{
		if (enemy == nullptr)
		{
			continue;
		}

		if (enemy->GetHp() <= 0)
		{
			continue;
		}

		enemy->Damage(damage);
	}
}

// Elemental Attack / Combo
void Battle::UesElementalAttack(Map& map, PlayerManager& player)
{
	Enemy* enemy =
		GetSelectedTargetEnemy();


	if (enemy == nullptr)
	{
		EndTurn();
		return;
	}


	printfDx(L"ComboCheck F=%d Wa=%d state=%d", (state & USED_FIRE) != 0, (state & USED_WATER) != 0, state);


	printfDx(L"ComboCheck Wa=%d S=%d state=%d", (state & USED_WATER) != 0, (state & USED_SOIL) != 0, state);



	// 火 + 水
	// 蒸気爆発
	if ((state & USED_FIRE) && (state & USED_WATER))
	{
		map.SteamExplosionBreak(player);

		int comboDamage = 20;

		// 敵全体にダメージ
		DamageAllEnemies(comboDamage);
		m_displayMessage = L"蒸気爆発が発動した！";

		UsedAttackInfo info;

		info.element = Monster::CharacteRistics::Fire;
		info.attackName = L"蒸気爆発！";

		m_usedAttackOrder.push_back(info);
	}



	// 水 + 土
	// 地面崩壊
	if ((state & USED_WATER) && (state & USED_SOIL))
	{
		map.FloorBreak(player);

		int comboDamage = 20;

		// 敵全体にダメージ
		DamageAllEnemies(comboDamage);

		m_displayMessage = L"地面が崩壊した！";

		UsedAttackInfo info;

		info.element = Monster::CharacteRistics::Water;
		info.attackName = L"泥流生成！";

		m_usedAttackOrder.push_back(info);
	}


	// 水 + 風
	// 激流
	if ((state & USED_WATER) && (state & USED_WIND))
	{
		map.WaterFlowsBreak(player);

		int comboDamage = 20;

		// 敵全体にダメージ
		DamageAllEnemies(comboDamage);

		m_displayMessage = L"荒波が発生した！";

		UsedAttackInfo info;

		info.element = Monster::CharacteRistics::Water;
		info.attackName = L"激流！";


		m_usedAttackOrder.push_back(info);
	}



	// 草 + 水
	// 成長
	if ((state & USED_GRASS) && (state & USED_WATER))
	{
		map.GrowGrassBreak(player);


		UsedAttackInfo info;

		info.element = Monster::CharacteRistics::Grass;

		info.attackName = L"草！";


		m_usedAttackOrder.push_back(info);
	}



	// 土 + 火
	// 火山化


	if ((state & USED_SOIL) && (state & USED_FIRE))
	{
		map.VolcazationBreak(player);

		int comboDamage = 20;

		// 敵全体にダメージ
		DamageAllEnemies(comboDamage);

		m_displayMessage = L"溶岩が溢れ出す！";

		UsedAttackInfo info;

		info.element = Monster::CharacteRistics::Soil;

		info.attackName = L"火山化！";


		m_usedAttackOrder.push_back(info);
	}



	// 風


	if (state & USED_WIND)
	{
		map.WindBreak(player);
	}



	// 雷


	if (state & USED_THUNDER)
	{
		map.ThunderBreak(player);
	}



	// 次ターン用にリセット


	state = USED_NONE;
}



// Attack Order


const std::vector<Battle::UsedAttackInfo>&Battle::GetUsedAttackOrder() const
{
return m_usedAttackOrder;
}


void Battle::ClearUsedAttackOrder()
{
m_usedAttackOrder.clear();
}



// Combo Member


bool Battle::IsComboMember(Monster::CharacteRistics type,bool steamcombo,bool floorcombo)
{

// 火 + 水


if (steamcombo &&(type ==Monster::CharacteRistics::Fire ||type ==Monster::CharacteRistics::Water))
{
return true;
}



// 水 + 土


if (floorcombo &&(type ==Monster::CharacteRistics::Water ||type ==Monster::CharacteRistics::Soil))
{
return true;
}


return false;
}