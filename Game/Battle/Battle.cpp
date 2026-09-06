#include "pch.h"

#include "Game/Battle/Battle.h"

#include"Game/SEManager/SEManager.h"
#include "Game/Scene/SceneManager.h"
#include "Game/Party/Party.h"
#include"Game/Enemy/BossManager.h"

// Constructor / Destructor
Battle::Battle(BossManager&bossManager)
:m_bossManager		{bossManager}
, m_receponsTimer	{}
, m_displaytextTimer{}
, m_Window			{}
, m_windowWidth		{}
, m_windowWidthFront{}
, m_IsActive		{}
, m_isJoinWindow	{}
, m_select			{}
, m_state			{}
, m_displayIndex	{}
, m_attackSelect	{}
, m_requestDefense	{}
,m_characteRistics	{}
, m_monsterSelect	{}
, m_monsterhp		{}
, m_isRun			{}
, m_isFieldRequested{}
, m_isEnemyRequested{}
, m_annihilation	{}
, m_replaceSelect	{}
, m_joinState		{}
, m_joinEnemy		{}
{
}

Battle::~Battle()
{
}

// Initialize / Finalize
void Battle::Initialize(SceneManager* sceneManager)
{
	// 攻撃履歴
	// 前回の戦闘で記録した攻撃履歴をクリア
	m_usedAttackOrder.clear();

	// 背景
	drawBgPosition.x = 10;
	drawBgPosition.y = 10;
	drawBgSize.x = 1260;
	drawBgSize.y = 500;
	// コマンドボックス
	// コマンド選択画面
	drawCommandBoxPosition1.x = 120;
	drawCommandBoxPosition1.y = 440;
	drawCommandBoxSize1.x = 430;
	drawCommandBoxSize1.y = 270;

	// 敵情報
	drawCommandBoxPosition1_1.x = 550;
	drawCommandBoxPosition1_1.y = 440;
	drawCommandBoxSize1_1.x = 610;
	drawCommandBoxSize1_1.y = 270;

	// 各キャラクターのコマンド一覧
	drawCommandBoxPosition2.x = 120;
	drawCommandBoxPosition2.y = 550;
	drawCommandBoxSize2.x = 430;
	drawCommandBoxSize2.y = 160;

	// キャラクター名・HP表示
	drawCommandBoxPosition2_1.x = 120;
	drawCommandBoxPosition2_1.y = 440;
	drawCommandBoxSize2_1.x = 430;
	drawCommandBoxSize2_1.y = 100;

	// 現在選択中のコマンド一覧
	drawCommandBoxPosition2_2.x = 550;
	drawCommandBoxPosition2_2.y = 440;
	drawCommandBoxSize2_2.x = 610;
	drawCommandBoxSize2_2.y = 270;

	// ターン中の表示
	drawCommandBoxPosition2_3.x = 120;
	drawCommandBoxPosition2_3.y = 440;
	drawCommandBoxSize2_3.x = 430;
	drawCommandBoxSize2_3.y = 270;

	// 攻撃エフェクト
	drawEffectPosition.x = 0;
	drawEffectPosition.y = 0;
	drawEffectSize.x = 1280;
	drawEffectSize.y = 1280;

	// 選択状態
	// コマンド選択位置
	m_select = 0;
	// 敵ターゲット
	m_targetEnemyIndex = -1;
	m_selectedTargetEnemyIndex = -1;
	// キャラクター・技の選択位置
	m_displayIndex = 0;
	m_monsterSelect = 0;
	m_attackSelect = 0;
	// 選択した技
	// パーティ人数分の選択技を確保
	m_selectedAttack.resize(m_party->GetMonsterCount());
	// 全員「未選択」にする
	std::fill(m_selectedAttack.begin(),m_selectedAttack.end(),-1);
	// 戦闘状態
	// 戦闘開始時はコマンド選択から開始
	m_state = BattleState::Command;
	// 属性状態をリセット
	m_characteRistics = Monster::CharacteRistics::None;
	// コマンド入力を有効にする
	m_IsActive = true;
	// 各ウィンドウを閉じる
	m_Window = false;
	m_isJoinWindow = false;
	// 逃走状態
	// 前回の逃走結果をリセット
	m_isRunSuccess = false;
	// 逃走処理中ではない
	m_isRun = false;
	// 敵の行動状態
	// 敵の攻撃タイプを初期化
	m_enemyAttackType = 0;
	// 敵の攻撃対象を初期化
	m_enemyTargetIndex = 0;
	// ウィンドウ・攻撃エフェクト
	m_windowWidth = 0;
	m_windowHeight = 0;
	m_windowWidthFront = 0;
	// 攻撃エフェクトを停止
	m_playAttackEffect = false;
	// エフェクト属性をリセット
	m_effectElement = Monster::CharacteRistics::None;
	// タイマー
	// コマンド入力受付用タイマー
	m_receponsTimer = 10;
	// メッセージ表示用タイマー
	m_displaytextTimer = 0;
	// 攻撃エフェクト用タイマー
	m_attackEffectTimer = 0;
	// シーン遷移リクエスト
	// フィールドへの遷移要求をリセット
	m_isFieldRequested = false;
	// 次の敵への遷移要求をリセット
	m_isEnemyRequested = false;
	// 敵死亡演出
	// 敵死亡演出を停止
	m_enemyDeadMotion = false;
	// 死亡演出の位置を初期化
	m_enemyDeadOffsetY = 0;
	// 前回の死亡敵情報をクリア
	m_deadEnemy = nullptr;
	m_deadEnemyName.clear();
	// 敵から受けたダメージ表示
	// パーティ人数分のダメージメッセージを確保
	m_displayMessageEnemyAttackDamage.resize(MAX_PARTY);
	// ダメージメッセージをすべてクリア
	for (int i = 0; i < MAX_PARTY; i++)
	{
		m_displayMessageEnemyAttackDamage[i].clear();
	}
	// 連携攻撃・コンボ状態
	// 使用した属性をリセット
	state = USED_NONE;

	// 連携攻撃待機状態を解除
	m_comboPending = false;

	// 味方HP
	// 戦闘開始時の味方HPを保存
	for (int i = 0; i < m_party->GetMonsterCount(); i++)
	{
		Monster* monster = m_party->GetMonster(i);

		m_monsterhp[i] = monster->GetCurrentHitPoint();
	}
	// 防御状態
	// 全員の防御状態を解除
	for (int i = 0; i < MAX_PARTY; i++)
	{
		m_requestDefense[i] = false;
	}
	// 全滅状態
	// 戦闘開始時は全滅していない
	m_annihilation = false;
	// 仲間加入
	// 仲間加入処理を初期状態に戻す
	m_joinState = JoinState::None;

	// 入れ替え対象の選択位置を初期化
	m_replaceSelect = 0;

	// 加入予定の敵をクリア
	m_joinEnemy = nullptr;
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
				// 上
				if (inputManager.IsTrigger(KEY_INPUT_UP)||inputManager.IsPadTrigger(PAD_INPUT_UP))
				{
					m_select--;
					if (m_select < 0)
					{
						m_select = COMMAND_NUM - 1;
					}
					m_receponsTimer = 0;
					m_sound->PlayTypeBackStart(SEManager::SoundList::Cursor);
				}

				// 下
				if (inputManager.IsTrigger(KEY_INPUT_DOWN) || inputManager.IsPadTrigger(PAD_INPUT_DOWN))
				{
					m_select++;
					if (m_select >= COMMAND_NUM)
					{
						m_select = 0;
					}
					m_receponsTimer = 0;
					m_sound->PlayTypeBackStart(SEManager::SoundList::Cursor);
				}

				// 決定
				if (inputManager.IsTrigger(KEY_INPUT_RETURN) || inputManager.IsPadTrigger(PAD_INPUT_A))
				{
					m_receponsTimer = 0;
					m_sound->PlayTypeBackStart(SEManager::SoundList::Decision);
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
						// 応援
						m_windowWidth = 0;
						m_windowWidthFront = 0;
						m_state = BattleState::Party;
						break;

					case 3:
						// 逃げる
						m_displaytextTimer = 0;
						m_displayMessage.clear();
						m_state = BattleState::Run;
						break;

					case 4:
						// 敵ターン
						m_state = BattleState::EnemyTurn;
						break;
					}
				}

				// 戻る処理
				if (!m_IsActive && m_state == BattleState::AttackSelect && (inputManager.IsTrigger(KEY_INPUT_BACK) || inputManager.IsPadTrigger(PAD_INPUT_B)) && m_monsterSelect == 0)
				{
					m_state = BattleState::Command;
					m_IsActive = true;
					m_Window = false;
					m_windowWidth = 0;
					m_windowWidthFront = 0;
					m_sound->PlayTypeBackStart(SEManager::SoundList::Cancel);
				}
			}
		}
	}
	if (m_joinState == JoinState::Replace)
	{
		m_receponsTimer++;
		return;
	}

	// State Update
	switch (m_state)
	{
	case BattleState::Command:
		break;
	case BattleState::AttackSelect:
		UpdateAttackSelect(inputManager);
		break;
	case BattleState::AttackAction:
		UpdateAttackAction(map, player);
		break;
	case BattleState::Tool:
		UpdateTool(inputManager);
		break;
	case BattleState::Party:
		UpdateParty(inputManager);
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
		case Monster::CharacteRistics::Normal:
			m_image->DrawNormal(drawEffectPosition, drawEffectSize);
			break;
		case Monster::CharacteRistics::Fire:
			m_image->DrawFire(drawEffectPosition, drawEffectSize);
			break;

		case Monster::CharacteRistics::Water:
			m_image->DrawWater(drawEffectPosition, drawEffectSize);
			break;

		case Monster::CharacteRistics::Grass:
			m_image->DrawGrass(drawEffectPosition, drawEffectSize);
			break;

		case Monster::CharacteRistics::Soil:
			m_image->DrawSoil(drawEffectPosition, drawEffectSize);
			break;

		case Monster::CharacteRistics::Wind:
			m_image->DrawWind(drawEffectPosition, drawEffectSize);
			break;

		case Monster::CharacteRistics::Darkness:
			m_image->DrawDarkness(drawEffectPosition, drawEffectSize);
			break;

		case Monster::CharacteRistics::SteamExplpsion:
			m_image->DrawSteamexplosion(drawEffectPosition, drawEffectSize);
			break;

		case Monster::CharacteRistics::WaterFlows:
			m_image->DrawWaterflows(drawEffectPosition, drawEffectSize);
			break;

		case Monster::CharacteRistics::FloorBreak:
			m_image->DrawFloorBreak(drawEffectPosition, drawEffectSize);
			break;

		case Monster::CharacteRistics::GrawGrass:
			m_image->DrawGrowgrass(drawEffectPosition, drawEffectSize);
			break;

		case Monster::CharacteRistics::Volcazation:
			m_image->DrawVolcazation(drawEffectPosition, drawEffectSize);
			break;

		default:
			break;
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
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

	case BattleState::Party:
		RenderParty();
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
}

// Command
void Battle::RenderCommand()
{
	if (!m_IsActive)
	{
		return;
	}
	m_image->DrawCommandbox1(drawCommandBoxPosition1, drawCommandBoxSize1);
	m_image->DrawCommandbox1(drawCommandBoxPosition1_1, drawCommandBoxSize1_1);
	SetFontSize(40);
	for (int i = 0; i < COMMAND_NUM; i++)
	{
		DrawString(250, 480 + 50 * i, m_command[i], GetColor(255, 255, 255));
	}
	// カーソル
	drawCommandCursorPosition.x = 200;
	drawCommandCursorPosition.y = 480+50*m_select;
	drawCommandCursorSize = { 40,40 };
	m_image->DrawCommandCursor(drawCommandCursorPosition, drawCommandCursorSize);
	SetFontSize(30);
	// 敵情報
	DrawString(620,490,L"敵データ",GetColor(255, 255, 255));
	// 敵は最大3体
	for (int i = 0; i < 3; i++)
	{
		// 敵が存在しない
		if (i >= static_cast<int>(m_enemies.size()))
		{
			continue;
		}

		Enemy* enemy = m_enemies[i];
		if (enemy == nullptr)
		{
			continue;
		}

		int x = 620 + i * 170;
		int y = 550;

		drawCommandCursorPosition.x = x-25;
		drawCommandCursorPosition.y = y;
		drawCommandCursorSize = { 20,20 };
		// 敵の名前
		int nameColor = GetColor(255, 255, 255);
		// 現在ターゲット中なら黄色
		if (i == m_targetEnemyIndex)
		{
			nameColor = GetColor(255, 255, 0);
			m_image->DrawCommandCursor(drawCommandCursorPosition, drawCommandCursorSize);
		}

		DrawString(x, y, enemy->GetName(), nameColor);
		// HP
		if (enemy->GetHp() > 0)
		{
			DrawFormatString(x, y + 45, GetColor(255, 100, 100), L"HP : %d", enemy->GetHp());
		}
		else
		{
			DrawString(x, y + 45, L"HP : 0", GetColor(100, 100, 100));
		}
	}
}

void Battle::RenderCurrentCommand()
{
	m_image->DrawCommandbox1(drawCommandBoxPosition2_2,drawCommandBoxSize2_2);

	DrawString(590, 480, L"行動指示", GetColor(255, 255, 255));

	for (int i = 0; i < m_party->GetMonsterCount(); i++)
	{
		Monster* monster = m_party->GetMonster(i);

		if (monster == nullptr)
		{
			continue;
		}

		std::wstring name = monster->GetName();
		int column = i % 2;
		int row = i / 2;

		int x = 620 + column * 250;
		int y = 530 + row * 80;
		drawCommandCursorPosition.x = x-25;
		drawCommandCursorPosition.y = y;
		drawCommandCursorSize = { 30,30 };

		// 死亡しているか
		bool isDead = (m_monsterhp[i] <= 0);
		// 現在選択中の仲間
		int nameColor = GetColor(255, 255, 255);

		if (isDead)
		{
			// 死亡中
			nameColor = GetColor(128, 128, 128);
		}
		else if (i == m_monsterSelect)
		{
			nameColor = GetColor(255, 255, 0);

			m_image->DrawCommandCursor(drawCommandCursorPosition, drawCommandCursorSize);
		}

		// 仲間の名前
		DrawString(x,y,name.c_str(),nameColor);
		// 死亡中なら「死亡中」と表示
		if (isDead)
		{
			DrawString(x,y + 35,L"戦闘不能",GetColor(128, 128, 128));

			continue;
		}

		// 選択した技
		int attackIndex = m_selectedAttack[i];
		if (attackIndex >= 0)
		{
			auto& attacks = monster->GetAttacks();
			if (attackIndex < static_cast<int>(attacks.size()))
			{
				DrawString(x,y + 35,attacks[attackIndex].name,GetColor(255, 255, 0));
			}
		}
		else
		{
			DrawString(x,y + 35,L"待機中",GetColor(150, 150, 150));
		}
	}
}

void Battle::RenderCurrentHp()
{
	m_image->DrawCommandbox1(drawCommandBoxPosition2_2, drawCommandBoxSize2_2);
	for (int i = 0; i < m_party->GetMonsterCount(); i++)
	{
		Monster* monster = m_party->GetMonster(i);

		if (monster == nullptr)
		{
			continue;
		}

		std::wstring name = monster->GetName();
		// 2列 × 2行
		int column = i % 2;
		int row = i / 2;
		int x = 620 + column * 250;
		int y = 530 + row * 80;

		// 仲間の名前
		DrawString(x, y, name.c_str(), GetColor(255, 255, 255));
		// 体力
		DrawFormatString(x, y + 35, GetColor(255, 255, 0), L"HP : %d / %d", m_monsterhp[i], monster->GetMaxHitPoint());
	}
}


// Attack Select
void Battle::UpdateAttackSelect(InputManager&inputManager)
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
	if (m_receponsTimer > 25 && (inputManager.IsTrigger(KEY_INPUT_BACK) || inputManager.IsPadTrigger(PAD_INPUT_B)))
	{
		m_receponsTimer = 0;
		m_sound->PlayTypeBackStart(SEManager::SoundList::Cancel);
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
		if (inputManager.IsTrigger(KEY_INPUT_LEFT) || inputManager.IsPadTrigger(PAD_INPUT_LEFT))
		{
			m_targetEnemyIndex = GetValidTargetIndex(m_targetEnemyIndex - 1);
			m_receponsTimer = 0;
			m_sound->PlayTypeBackStart(SEManager::SoundList::Cursor);
		}

		// 右：敵ターゲット変更
		if (inputManager.IsTrigger(KEY_INPUT_RIGHT) || inputManager.IsPadTrigger(PAD_INPUT_RIGHT))
		{
			m_targetEnemyIndex = GetValidTargetIndex(m_targetEnemyIndex + 1);
			m_receponsTimer = 0;
			m_sound->PlayTypeBackStart(SEManager::SoundList::Cursor);
		}

		// 下：技選択
		if (inputManager.IsTrigger(KEY_INPUT_DOWN) || inputManager.IsPadTrigger(PAD_INPUT_DOWN))
		{
			m_attackSelect++;
			m_receponsTimer = 0;
			m_sound->PlayTypeBackStart(SEManager::SoundList::Cursor);

			if (m_attackSelect >= attacks.size())
			{
				m_attackSelect = 0;
			}
		}

		// 上：技選択
		if (inputManager.IsTrigger(KEY_INPUT_UP) || inputManager.IsPadTrigger(PAD_INPUT_UP))
		{
			m_attackSelect--;
			m_receponsTimer = 0;
			m_sound->PlayTypeBackStart(SEManager::SoundList::Cursor);

			if (m_attackSelect < 0)
			{
				m_attackSelect = (int)attacks.size() - 1;
			}
		}

		// 決定
		if (inputManager.IsTrigger(KEY_INPUT_RETURN) || inputManager.IsPadTrigger(PAD_INPUT_A))
		{
			m_receponsTimer = 0;
			m_sound->PlayTypeBackStart(SEManager::SoundList::Decision);

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

	m_image->DrawCommandbox1(drawCommandBoxPosition2_3, drawCommandBoxSize2_3);
	RenderCurrentCommand();
	// 現在行動選択中の仲間
	if (m_monsterSelect >= m_party->GetMonsterCount())
	{
		return;
	}

	Monster* monster = m_party->GetMonster(m_monsterSelect);

	if (monster == nullptr)
	{
		return;
	}

	std::wstring name = monster->GetName();
	auto& attacks = monster->GetAttacks();

	// 仲間の名前
	DrawString(160,470,name.c_str(),GetColor(0, 0, 255));
	// HP
	DrawFormatString(300,520,GetColor(0, 255, 255),L"HP : %d / %d",	m_monsterhp[m_monsterSelect],monster->GetMaxHitPoint());

	// 技一覧
	for (int i = 0; i < attacks.size(); i++)
	{
		int color = GetColor(255, 255, 255);
		// 選択済み
		if (m_selectedAttack[m_monsterSelect] == i)
		{
			color = GetColor(255, 255, 0);
		}

		// 技名
		DrawString(250,580 + i * 40,attacks[i].name,color);

		// 現在のカーソル
		if (i == m_attackSelect)
		{
			drawCommandCursorPosition.x = 220;
			drawCommandCursorPosition.y= 580 + i * 40 ;
			drawCommandCursorSize = { 30,30 };
			m_image->DrawCommandCursor(drawCommandCursorPosition, drawCommandCursorSize);
		}
	}
}
// Attack Action
void Battle::UpdateAttackAction(Map& map, PlayerManager& player)
{
	// メッセージ表示中なら、攻撃処理を進めない
	if (m_isDisplayingMessage)
	{
		UpdateDisplayMessage();
		return;
	}

	// 次のメッセージがあるなら表示する
	if (!m_displayQueue.empty())
	{
		UpdateDisplayMessage();
		return;
	}
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
			m_displayMessageDamage.clear();
			m_displayMessageCombo.clear();
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

			m_state =BattleState::EnemyDead;

			return;
		}
		// 次のターン
		m_comboPending = false;
		m_displayMessage.clear();
		m_displayIndex = 0;
		m_monsterSelect = 0;
		std::fill(m_selectedAttack.begin(), m_selectedAttack.end(), -1);
		m_state =BattleState::EnemyTurn;
		m_displaytextTimer = 0;

		return;
	}

	// コンボ条件確認
	bool hasFire = false;
	bool hasWater = false;
	bool hasGrass = false;
	bool hasSoil = false;
	bool hasWind = false;

	for (int i = 0;i < m_party->GetMonsterCount();i++)
	{
		if (m_selectedAttack[i] < 0)
		{
			continue;
		}

		auto& attacks = m_party->GetMonster(i)->GetAttacks();
		auto type = attacks[m_selectedAttack[i]].element;

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
		if (type == Monster::CharacteRistics::Wind)
		{
			hasWind = true;
		}
	}

	bool steamCombo = hasFire && hasWater;
	bool floorCombo = hasWater && hasSoil;
	bool waterFallCombo = hasWater && hasWind;
	bool growgrassCombo = hasGrass && hasWater;
	bool volcazationCombo = hasSoil && hasFire;

	// HP0 または未選択の仲間を飛ばす
	while (m_displayIndex < m_party->GetMonsterCount() && (m_monsterhp[m_displayIndex] <= 0 || m_selectedAttack[m_displayIndex] < 0))
	{
		m_displayIndex++;
	}

	// 全員の行動が終了した
	if (m_displayIndex >= m_party->GetMonsterCount())
	{
		m_comboPending = true;
		m_displaytextTimer = 0;
		return;
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
		m_displayMessage.clear();
		m_displayMessageDamage.clear();

		Monster* monster = m_party->GetMonster(m_displayIndex);
		auto& attacks = monster->GetAttacks();
		int index = m_selectedAttack[m_displayIndex];

		// 範囲チェック
		if (index < 0 || index >= (int)attacks.size())
		{
			m_displayIndex++;
			m_displaytextTimer = 0;
			return;
		}
		// 属性取得
		m_characteRistics = attacks[index].element;

		// 攻撃エフェクト開始
		m_effectElement = m_characteRistics;

		m_playAttackEffect = true;
		m_attackEffectTimer = 0;

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

		case Monster::CharacteRistics::Darkness:
			state |= USED_DARKNESS;
			break;

		case Monster::CharacteRistics::Wind:
			state |= USED_WIND;
			break;

		case Monster::CharacteRistics::GrawGrass:
			// 回復
			m_requestDefense[m_displayIndex] = true;
			AddDisplayMessage(DisplayMessageType::Normal,monster->GetName() + L"の" + attacks[index].name +L"！\n全体が" + std::to_wstring(attacks[index].power) + L"回復!");
			for (int i = 0;i < m_party->GetMonsterCount();i++)
			{
				Monster* target = m_party->GetMonster(i);
				if (target == nullptr)
				{
					continue;
				}
				target->Heal(attacks[index].power);
			}
			break;

		case Monster::CharacteRistics::Defense:
			// このターン防御
			m_requestDefense[m_displayIndex] = true;
			AddDisplayMessage(DisplayMessageType::Normal, monster->GetName() + L"は身を守っている！");
			break;
		}

		// コンボメンバー判定
		bool isComboMember = IsComboMember(m_characteRistics, steamCombo, floorCombo,waterFallCombo,growgrassCombo,volcazationCombo);

		// 通常攻撃処理
		if (m_characteRistics != Monster::CharacteRistics::Defense && !isComboMember)
		{
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
			case Monster::CharacteRistics::Darkness:
				map.DarknessBreak(player);
				break;
			default:
				break;
			}
		}
		else if (isComboMember)
		{
			// コンボ準備
			AddDisplayMessage(DisplayMessageType::Normal,std::wstring(monster->GetName()) +L"\nは連携の構えをしている！");
		}

		// 攻撃メッセージ
		bool isDefense =
			m_characteRistics == Monster::CharacteRistics::Defense ||
			m_characteRistics == Monster::CharacteRistics::GrawGrass;

		if (!isDefense && !isComboMember)
		{
			AddDisplayMessage(DisplayMessageType::Normal,monster->GetName() + L"の" + attacks[index].name + L"！");
			Enemy* target = GetSelectedTargetEnemy();
			if (target != nullptr)
			{
				// 攻撃前のHP
				int beforeHp = target->GetHp();
				// ダメージ計算
				int damage = static_cast<int>(attacks[index].power * magnification);
				// 攻撃
				target->Damage(damage);
				int actualDamage = beforeHp - target->GetHp();
				AddDisplayMessage(DisplayMessageType::Damage, std::wstring(target->GetName()) + L"に" + std::to_wstring(actualDamage) + L"ダメージ！");
			}
		}
	}

	// メッセージが残っている間は次の攻撃へ進まない
	if (m_isDisplayingMessage || !m_displayQueue.empty())
	{
		return;
	}

	// 敵死亡判定
	if (m_selectedTargetEnemyIndex >= 0 && m_selectedTargetEnemyIndex < static_cast<int>(m_enemies.size()))
	{
		Enemy* target = m_enemies[m_selectedTargetEnemyIndex];
		if (target != nullptr &&target->GetHp() <= 0)
		{
			m_deadEnemy = target;
			m_deadEnemyName = target->GetName();
			m_enemyDeadMotion = true;
			m_enemyDeadOffsetY = 0;
			m_displaytextTimer = 0;
			m_state =BattleState::EnemyDead;
			return;
		}
	}
}
void Battle::RenderAttackAction()
{
	m_image->DrawCommandbox1(drawCommandBoxPosition2_3,drawCommandBoxSize2_3);
	if (m_isDisplayingMessage)
	{
		int color = GetColor(255, 255, 255);
		if (m_currentDisplayMessage.type ==	DisplayMessageType::Damage)
		{
			color = GetColor(255, 0, 0);
		}
		DrawString(150,480,m_currentDisplayMessage.text.c_str(),color);
	}
	RenderCurrentCommand();
}

// Tool
void Battle::UpdateTool(InputManager&inputManager)
{
	if (m_state != BattleState::Tool)
	{
		return;
	}
	//決定
	if (m_receponsTimer > 25 && (inputManager.IsTrigger(KEY_INPUT_RETURN) || inputManager.IsPadTrigger(PAD_INPUT_A)))
	{
		m_receponsTimer = 0;
		m_state = BattleState::EnemyTurn;
		m_sound->PlayTypeBackStart(SEManager::SoundList::Decision);
	}
	// 戻る
	if (m_receponsTimer > 25 && (inputManager.IsTrigger(KEY_INPUT_BACK) || inputManager.IsPadTrigger(PAD_INPUT_B)))
	{
		m_receponsTimer = 0;
		m_state = BattleState::Command;
		m_IsActive = true;
		m_Window = false;
		m_windowWidth = 0;
		m_windowWidthFront = 0;
		m_select = 1; // 「道具」にカーソルを残す
		m_sound->PlayTypeBackStart(SEManager::SoundList::Cancel);
		return;
	}
}
void Battle::RenderTool()
{
	m_image->DrawCommandbox1(drawCommandBoxPosition2_3, drawCommandBoxSize2_3);
	DrawString(200, 470, L"道具", GetColor(255, 255, 255));
	DrawFormatString(250, 580, GetColor(255, 255, 255), L"未入手");
}

//Party
void Battle::UpdateParty(InputManager&inputManager)
{
	if (m_state != BattleState::Party)
	{
		return;
	}

	//決定
	if (m_receponsTimer > 25 && (inputManager.IsTrigger(KEY_INPUT_RETURN) || inputManager.IsPadTrigger(PAD_INPUT_A)))
	{
		m_receponsTimer = 0;
		m_state = BattleState::EnemyTurn;
		m_sound->PlayTypeBackStart(SEManager::SoundList::Decision);
	}
	// 戻る
	if (m_receponsTimer > 25 && (inputManager.IsTrigger(KEY_INPUT_BACK) || inputManager.IsPadTrigger(PAD_INPUT_B)))
	{
		m_receponsTimer = 0;
		m_state = BattleState::Command;
		m_IsActive = true;
		m_Window = false;
		m_windowWidth = 0;
		m_windowWidthFront = 0;
		m_select = 2; // 「仲間を見る」にカーソルを残す
		m_sound->PlayTypeBackStart(SEManager::SoundList::Cancel);
		return;
	}
}
void Battle::RenderParty()
{
	m_image->DrawCommandbox1(drawCommandBoxPosition2_3, drawCommandBoxSize2_3);
	RenderCurrentHp();
	DrawString(200, 470, L"仲間の情報", GetColor(255, 255, 255));
}

// Run
void Battle::UpdateRun()
{
	m_displaytextTimer++;
	if (m_displaytextTimer == 1)
	{
		int rand = GetRand(1);

		if (rand == 0)
		{
			// 逃走成功
			m_isRunSuccess = true;
			m_displayMessage = L"にげだした!";
			m_sound->PlayTypeBackStart(SEManager::SoundList::Run);
		}
		else
		{
			// 逃走失敗
			m_isRunSuccess = false;
			m_displayMessage = L"にげられなかった...";
		}
	}

	if (m_displaytextTimer > 60)
	{
		if (m_isRunSuccess)
		{
			m_player->m_position = m_player->m_currentposition;
			m_isEnemyRequested = true;
			m_isFieldRequested = true;
			m_state = BattleState::Command;
		}
		else
		{
			m_state = BattleState::EnemyTurn;
		}
		m_displaytextTimer = 0;
	}
}
void Battle::RenderRun()
{
	m_image->DrawCommandbox1(drawCommandBoxPosition2_3, drawCommandBoxSize2_3);
	DrawString(150, 480, m_displayMessage.c_str(), GetColor(255, 255, 255));
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
	// 1回だけ攻撃方法を決める
	if (m_displaytextTimer == 1)
	{
		// 0 = 単体攻撃
		// 1 = 全体攻撃
		m_enemyAttackType = GetRand(1);

		// 単体攻撃の場合、攻撃対象を決める
		if (m_enemyAttackType == 0)
		{
			std::vector<int> aliveMembers;
			for (int i = 0; i < m_party->GetMonsterCount(); i++)
			{
				if (m_monsterhp[i] > 0)
				{
					aliveMembers.push_back(i);
				}
			}

			if (!aliveMembers.empty())
			{
				int randomIndex = GetRand(static_cast<int>(aliveMembers.size()) - 1);
				m_enemyTargetIndex = aliveMembers[randomIndex];
			}
			else
			{
				EndTurn();
				return;
			}
		}
	}

	// 敵攻撃メッセージ
	if (m_displaytextTimer <= 60)
	{
		m_displayMessage =std::wstring(enemy->GetName()) + L"の攻撃!!";
	}

	// 単体攻撃
	else if (m_enemyAttackType == 0 &&m_displaytextTimer <= 120)
	{
		Monster* target =m_party->GetMonster(m_enemyTargetIndex);

		if (target == nullptr)
		{
			EndTurn();
			return;
		}

		m_displayMessage =target->GetName() + L"に攻撃！";///////後に技名にする
		// 120フレーム目にダメージ
		if (m_displaytextTimer == 120)
		{
			int damage = enemy->GetPower();
			// 防御中なら半減
			if (m_requestDefense[m_enemyTargetIndex])
			{
				damage /= 2;
			}

			int beforeHp =target->GetCurrentHitPoint();
			target->Damage(damage);
			int actualDamage =beforeHp - target->GetCurrentHitPoint();
			m_monsterhp[m_enemyTargetIndex] =target->GetCurrentHitPoint();

			if (m_monsterhp[m_enemyTargetIndex] < 0)
			{
				m_monsterhp[m_enemyTargetIndex] = 0;
			}
			m_displayMessageEnemyAttackDamage[m_enemyTargetIndex] =target->GetName()+ L"に"+ std::to_wstring(actualDamage)+ L"ダメージ";
		}
	}

	// 全体攻撃
	else if (m_enemyAttackType == 1 &&m_displaytextTimer <= 120)
	{
		m_displayMessage = L"全体攻撃";///////後に技名にする

		// 120フレーム目にダメージ
		if (m_displaytextTimer == 120)
		{
			for (int i = 0;i < m_party->GetMonsterCount();i++)
			{
				Monster* monster =m_party->GetMonster(i);

				if (monster == nullptr)
				{
					continue;
				}

				// 死亡している仲間には攻撃しない
				if (m_monsterhp[i] <= 0)
				{
					continue;
				}

				int damage = enemy->GetPower();
				// 防御中なら半減
				if (m_requestDefense[i])
				{
					damage /= 2;
				}

				int beforeHp =monster->GetCurrentHitPoint();
				monster->Damage(damage*0.5);
				int actualDamage =beforeHp - monster->GetCurrentHitPoint();
				m_monsterhp[i] =monster->GetCurrentHitPoint();
				if (m_monsterhp[i] < 0)
				{
					m_monsterhp[i] = 0;
				}
				m_displayMessageEnemyAttackDamage[i] =monster->GetName()+ L"に"+ std::to_wstring(actualDamage)+ L"ダメージ";
			}
		}
	}
	// ダメージ表示
	if (m_displaytextTimer >= 180)
	{
		EndTurn();
	}
}
void Battle::RenderEnemyTurn()
{
	m_image->DrawCommandbox1(drawCommandBoxPosition2_3, drawCommandBoxSize2_3);
	RenderCurrentHp();

	DrawString(150, 480, m_displayMessage.c_str(), GetColor(255, 255, 255));
	for (int i=0;i < m_party->GetMonsterCount();i++)
	{
		DrawString(150, 530+i*40, m_displayMessageEnemyAttackDamage[i].c_str(), GetColor(255, 255, 255));
	}
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
		m_displaytextTimer = 0;

		// 全員倒した
		if (AreAllEnemiesDead())
		{
			m_isEnemyRequested = true;
			m_bossManager.DefeatBoss(m_deadEnemy->GetBossNo());
			return;
		}
		m_deadEnemy = nullptr;
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
		m_state =BattleState::Command;
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
	m_image->DrawCommandbox1(drawCommandBoxPosition2_3, drawCommandBoxSize2_3);
	RenderCurrentHp();

	DrawString(180, 480, m_displayMessage.c_str(), GetColor(255, 255, 255));
}

// Annihilation全滅
void Battle::UpdateAnnihilation(GameOver& gameOver, InputManager& inputManager)
{
	gameOver.GameOverUpdate(inputManager);
}
void Battle::RenderAnnihilation(GameOver& gameOver)
{
	gameOver.GameOverRender();
}

// Turn End
void Battle::EndTurn()
{
	m_state = BattleState::Command;

	m_displayMessage.clear();
	m_displayMessageDamage.clear();
	m_displayMessageCombo.clear();

	//テキスト表示状態を完全リセット
	ClearDisplayMessageQueue();

	//コンボ状態もリセット
	m_comboPending = false;

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

	// ダメージ表記リセット
	for (int i = 0; i < MAX_PARTY; i++)
	{
		m_displayMessageEnemyAttackDamage[i].clear();
	}
	// 次ターンのターゲット
	m_selectedTargetEnemyIndex = m_targetEnemyIndex;
}
void Battle::ResetRunSuccess()
{
	m_isRunSuccess = false;
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

	if (m_selectedTargetEnemyIndex < 0 || m_selectedTargetEnemyIndex >= static_cast<int>(m_enemies.size()))
	{
		return nullptr;
	}

	Enemy* enemy = m_enemies[m_selectedTargetEnemyIndex];
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

//DisplayMassage
void Battle::UpdateDisplayMessage()
{
	// 現在のメッセージを表示中
	if (m_isDisplayingMessage)
	{
		m_currentDisplayTimer++;
		// 60フレーム表示したら次へ
		if (m_currentDisplayTimer >= 60)
		{
			m_isDisplayingMessage = false;
			m_currentDisplayTimer = 0;
		}
		return;
	}

	// 次のメッセージがない
	if (m_displayQueue.empty())
	{
		return;
	}

	// キューの先頭を取り出す
	m_currentDisplayMessage = m_displayQueue.front();
	m_displayQueue.pop_front();
	m_isDisplayingMessage = true;
	m_currentDisplayTimer = 0;
}
void Battle::AddDisplayMessage(DisplayMessageType type,const std::wstring& text)
{
	if (text.empty())
	{
		return;
	}

	BattleDisplayMessage message;
	message.type = type;
	message.text = text;
	m_displayQueue.push_back(message);
}
void Battle::ClearDisplayMessageQueue()
{
	m_displayQueue.clear();
	m_currentDisplayMessage.text.clear();
	m_currentDisplayTimer = 0;
	m_isDisplayingMessage = false;
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

// Setter
void Battle::SetImage(ImageManager* image)
{
	m_image = image;
}
void Battle::SetSound(SEManager* sound)
{
	m_sound = sound;
}
void Battle::SetFieldScene(FieldScene* fieldScene)
{
	m_fieldScene = fieldScene;
}
void Battle::SetPlayer(PlayerManager* player)
{
	m_player = player;
}
void Battle::SetParty(Party* party)
{
	m_party = party;
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
bool Battle::IsRunSuccess() const
{
	return m_isRunSuccess;
}
bool Battle::GetAnnihilation()
{
	return m_annihilation;
}

//Damage
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

		int beforeHp = enemy->GetHp();
		enemy->Damage(damage);
		int actualDamage = beforeHp - enemy->GetHp();
		m_comboDamageTotal[enemy] += actualDamage;
	}
}

// Elemental Attack / Combo
void Battle::UesElementalAttack(Map& map, PlayerManager& player)
{
	// 連携ダメージ初期化
	m_comboDamageTotal.clear();
	m_displayMessageDamage.clear();

	bool steamCombo =(state & USED_FIRE) &&(state & USED_WATER);
	bool floorCombo =(state & USED_WATER) &&(state & USED_SOIL);
	bool waterFlowCombo =(state & USED_WATER) &&(state & USED_WIND);
	bool growGrassCombo = (state & USED_GRASS) && (state & USED_WATER);
	bool volcazationCombo = (state & USED_FIRE) && (state & USED_SOIL);

	// 火 + 水
	if (steamCombo)
	{
		map.SteamExplosionBreak(player);
		DamageAllEnemies(20+15*map.GetBreakLevel());
		if (!m_displayMessageCombo.empty())
		{
			m_displayMessageCombo += L"\n";
		}
		AddDisplayMessage(DisplayMessageType::Combo,L"蒸気爆発が発動した！");
		UsedAttackInfo info;
		info.element =Monster::CharacteRistics::SteamExplpsion;
		info.attackName = L"蒸気爆発！";
		m_usedAttackOrder.push_back(info);
		m_fieldScene->LearnCompositeSkill(FieldScene::CooperatList::SteamExplpsion);
	}

	// 水 + 土
	if (floorCombo)
	{
		map.FloorBreak(player);
		DamageAllEnemies(10+15 * map.GetBreakLevel());
		if (!m_displayMessageCombo.empty())
		{
			m_displayMessageCombo += L"\n";
		}
		AddDisplayMessage(DisplayMessageType::Combo,L"地面が崩壊した！");
		UsedAttackInfo info;
		info.element =Monster::CharacteRistics::FloorBreak;
		info.attackName = L"泥流生成！";
		m_usedAttackOrder.push_back(info);
		m_fieldScene->LearnCompositeSkill(FieldScene::CooperatList::FloorBreak);
	}

	// 水 + 風
	if (waterFlowCombo)
	{
		map.WaterFlowsBreak(player);
		DamageAllEnemies(25+ 15 * map.GetBreakLevel());
		if (!m_displayMessageCombo.empty())
		{
			m_displayMessageCombo += L"\n";
		}
		AddDisplayMessage(DisplayMessageType::Combo,L"荒波が発生した！");
		UsedAttackInfo info;
		info.element =Monster::CharacteRistics::WaterFlows;
		info.attackName = L"激流！";
		m_usedAttackOrder.push_back(info);
		m_fieldScene->LearnCompositeSkill(FieldScene::CooperatList::WaterFlows);
	}
	////水＋草
	if (growGrassCombo)
	{
		map.GrowGrassBreak(player);
		for (int i = 0;i < m_party->GetMonsterCount();i++)
		{
			Monster* target = m_party->GetMonster(i);
			if (target == nullptr)
			{
				continue;
			}
			target->Heal(20);
		}
		if (!m_displayMessageCombo.empty())
		{
			m_displayMessageCombo += L"\n";
		}
		AddDisplayMessage(DisplayMessageType::Combo, L"草木が生い茂る！");
		UsedAttackInfo info;
		info.element = Monster::CharacteRistics::GrawGrass;
		info.attackName = L"草！";
		m_usedAttackOrder.push_back(info);
		m_fieldScene->LearnCompositeSkill(FieldScene::CooperatList::GrawGrass);
	}
	////火＋土
	if (volcazationCombo)
	{
		map.VolcazationBreak(player);
		DamageAllEnemies(25 + 15 * map.GetBreakLevel());
		if (!m_displayMessageCombo.empty())
		{
			m_displayMessageCombo += L"\n";
		}
		AddDisplayMessage(DisplayMessageType::Combo, L"大地が荒れ狂う！");
		UsedAttackInfo info;
		info.element = Monster::CharacteRistics::Volcazation;
		info.attackName = L"火山！";
		m_usedAttackOrder.push_back(info);
		m_fieldScene->LearnCompositeSkill(FieldScene::CooperatList::Volcazation);
	}

	// 敵ごとの合計ダメージ表示を作成
	for (Enemy* enemy : m_enemies)
	{
		if (enemy == nullptr)
		{
			continue;
		}

		auto it = m_comboDamageTotal.find(enemy);

		if (it == m_comboDamageTotal.end())
		{
			continue;
		}

		if (it->second <= 0)
		{
			continue;
		}

		if (!m_displayMessageDamage.empty())
		{
			m_displayMessageDamage += L"\n";
		}
		AddDisplayMessage(DisplayMessageType::Damage,std::wstring(enemy->GetName()) +L"に" +std::to_wstring(it->second) +L"ダメージ！");
	}
	// 使用属性リセット
	state = USED_NONE;
}

// Attack Order
const std::vector<Battle::UsedAttackInfo>& Battle::GetUsedAttackOrder() const
{
	return m_usedAttackOrder;
}
void Battle::ClearUsedAttackOrder()
{
	m_usedAttackOrder.clear();
}

// Combo Member
bool Battle::IsComboMember(Monster::CharacteRistics type, bool steamcombo, bool floorcombo,bool waterfallcombo,bool growgrasscombo,bool volcazationcombo)
{
	// 火 + 水
	if (steamcombo && (type == Monster::CharacteRistics::Fire || type == Monster::CharacteRistics::Water))
	{
		return true;
	}
	// 水 + 土
	if (floorcombo && (type == Monster::CharacteRistics::Water || type == Monster::CharacteRistics::Soil))
	{
		return true;
	}
	//水＋風
	if (waterfallcombo && (type == Monster::CharacteRistics::Water || type == Monster::CharacteRistics::Wind))
	{
		return true;
	}
	//水＋草
	if (growgrasscombo && (type == Monster::CharacteRistics::Water || type == Monster::CharacteRistics::Grass))
	{
		return true;
	}
	//土＋火
	if (volcazationcombo && (type == Monster::CharacteRistics::Soil || type == Monster::CharacteRistics::Fire))
	{
		return true;
	}
	return false;
}