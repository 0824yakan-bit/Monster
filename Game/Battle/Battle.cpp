#include "pch.h"
#include "Game/Battle/Battle.h"

#include"Game/Scene/SceneManager.h"
#include"Game/Party/Party.h"


Battle::Battle()
	:m_receponsTimer{}
	,m_displaytextTimer{}
	,m_Window{}
	,m_windowWidth{}
	,m_windowWidthFront{}
	,m_IsActive{}
	,m_isJoinWindow{}
	,m_select{}
	,m_state{}
	,m_displayIndex{}
	,m_attackSelect{}
	,m_monsterSelect{}
	,m_monsterhp{}
	,m_isRun{}
	,m_isFieldRequested{}
	,m_isEnemyRequested{}
	,m_annihilation{}
{

}

Battle::~Battle()
{

}

void Battle::Initialize(SceneManager*sceneManager)
{
	m_select = 0;
	m_displayIndex = 0;
	m_state = BattleState::Command;
	m_receponsTimer = 10;
	m_displaytextTimer = 0;
	m_enemyDeadMotion = false;
	m_enemyDeadOffsetY = 0;
	m_IsActive = true;
	m_Window = false;
	m_isJoinWindow = false;
	m_isFieldRequested = false;
	m_isEnemyRequested = false;
	m_windowWidthFront = 0;
	m_windowWidth = 0;
	m_monsterSelect = 0;     // 今選択中の仲間
	m_attackSelect = 0;      // 今選択中の技

	m_selectedAttack.resize(m_party->GetMonsterCount(),-1);

	state = 0;//戦闘開始時属性攻撃の状態リセット(１で使用）

	for (int i = 0;i < m_party->GetMonsterCount();i++)//現在のパーティのHP
	{
		Monster* monsterhitpoint = m_party->GetMonster(i);
		m_monsterhp[i] = monsterhitpoint->GetHitPoint() - sceneManager->m_monsterCurrentDamge[i];
	}

	m_annihilation = true;
}

void Battle::Update(SceneManager*sceneManager)
{
	m_annihilation = true;
	for (int i = 0;i < m_party->GetMonsterCount();i++)
	{
		if (m_monsterhp[i] != 0)
		{
			m_annihilation = false;
			break;
		}
	}
	m_receponsTimer++;
	if (m_receponsTimer > 15)
	{
		if (CheckHitKey(KEY_INPUT_RIGHT))
		{
			m_select++;
			m_receponsTimer = 0;
			if (m_select >= COMMAND_NUM)
			{
				m_select = 0;
			}
		}

		if (CheckHitKey(KEY_INPUT_LEFT))
		{
			m_select--;
			m_receponsTimer = 0;
			if (m_select < 0)
			{
				m_select = COMMAND_NUM - 1;
			}
		}
		if (!m_Window && CheckHitKey(KEY_INPUT_RETURN))
		{
			m_receponsTimer = 0;
			m_IsActive = false;
			m_Window = true;


			switch (m_select)//指揮・もちもの・さそう・にげだすの選択　キーカーソルの位置
			{
			case 0:
				m_windowWidth = 0;

				m_state = BattleState::AttackSelect;
				break;

			case 1:
				m_windowWidth = 0;

				m_state = BattleState::Tool;
				break;

			case 2:
				m_state = BattleState::Scout;
				break;

			case 3:
				m_state = BattleState::Run;
				break;

			case 4:
				m_state = BattleState::EnemyTurn;
				break;
			}
		}
		
		if (!m_IsActive && CheckHitKey(KEY_INPUT_BACK) && m_monsterSelect == 0)//詳細から戻る
		{
			m_state = BattleState::Command;
			m_IsActive = true;

			m_Window = false;
			m_windowWidth = 0;
			m_windowWidthFront = 0;
		}

		if (m_Window)
		{
			m_windowWidthFront += 50;
			if (m_windowWidthFront > 910)
				m_windowWidthFront = 910;
			m_windowWidth += 50;

			if (m_windowWidth > 920)
				m_windowWidth = 920;
		}
	}
	switch (m_state)//現在の状態
	{
	case BattleState::Command:
		break;

	case BattleState::AttackSelect:
		UpdateAttackSelect();
		break;

	case BattleState::AttackAction:
		UpdateAttackAction();
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
		UpdateAnnihilation();
		break;
	}
}

void Battle::Render()
{
	
	//戦闘画面
	DrawBox(10, 10, 1270, 500, GetColor(255, 255, 255), TRUE);
	//敵配置
	DrawBox(50, 50, 1230, 460, GetColor(255, 0, 0), FALSE);
	//コマンド選択位置
	DrawBox(20, 520, 1260, 700, GetColor(255, 255, 255), TRUE);
	for (int i = 0;i < MAX_PARTY;i++)
	{
		DrawFormatString(30+(i*200), 30,  GetColor(0, 0, 0), L"%d", m_monsterhp[i]);//パーティの現在のHP表示

	}
	//printfDx(L"%s", m_annihilation ? L"true" : L"false");

		//
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
		RenderAnnihilation();
		break;
	}


	//敵描画
	if (m_enemy)
	{
		if (m_enemyDeadMotion)
		{
			// やられ中は位置を下へずらす
			SetDrawArea(0, m_enemyDeadOffsetY, 1280, 720 + m_enemyDeadOffsetY);

			// 点滅させる（偶数フレームだけ描画）
			if ((m_displaytextTimer / 5) % 2 == 0)
			{
				m_enemy->RenderBattle();
			}

			SetDrawArea(0, 0, 1280, 720);
		}
		else
		{
			m_enemy->RenderBattle();
		}
	}

	//仮ｈｐ
	DrawFormatString(500,120,GetColor(0, 255, 255),L"HP : %d",m_enemy->GetHp());

	DrawFormatString(
		20, 300,GetColor(0, 0, 0),	L"N:%d F:%d Wa:%d G:%d S:%d T:%d Wi:%d",

		(state & USED_NORMAL) ? 1 : 0,
		(state & USED_FIRE)   ? 1 : 0,
		(state & USED_WATER)  ? 1 : 0,
		(state & USED_GRASS)  ? 1 : 0,
		(state & USED_SOIL)   ? 1 : 0,
		(state & USED_THUNDER)? 1 : 0,
		(state & USED_WIND)   ? 1 : 0
	);
}

void Battle::Finalize()
{

}

void Battle::RenderCommand()
{
	if (m_IsActive)//詳細を開くまで表示
	{
		DrawBox(40 , 530, 320, 690, GetColor(0, 0, 0), TRUE);
		DrawBox(340, 530, 630, 690, GetColor(0, 0, 0), TRUE);
		DrawBox(650, 530, 930, 690, GetColor(0, 0, 0), TRUE);
		DrawBox(950, 530, 1240, 690, GetColor(0, 0, 0), TRUE);
		SetFontSize(50);
		for (int i = 0; i < COMMAND_NUM; i++)
		{
			DrawString(100 + i * 300, 580, m_command[i], GetColor(255, 255, 255));
		}

		DrawString(70 + m_select * 300, 580, L"▶", GetColor(255, 255, 0));
	}
}

void Battle::UpdateAttackSelect()
{
	if (m_party == nullptr) return;
	if (m_party->GetMonsterCount() == 0) return;
	

	if (m_monsterSelect >= m_party->GetMonsterCount())
	{
		m_displayIndex = 0;
		m_displaytextTimer = 0;

		m_state = BattleState::AttackAction;
		return;
	}
	else
	{ 
	auto& attacks = m_party->GetMonster(m_monsterSelect)->GetAttacks();
		if (m_receponsTimer > 25)
		{
			if (CheckHitKey(KEY_INPUT_DOWN))
			{
				m_attackSelect++;
				m_receponsTimer = 0;

				if (m_attackSelect >= attacks.size())
				{
					m_attackSelect = 0;
				}
			}

			if (CheckHitKey(KEY_INPUT_UP))
			{
				m_attackSelect--;
				m_receponsTimer = 0;

				if (m_attackSelect < 0)
				{
					m_attackSelect = (int)attacks.size() - 1;
				}
			}

			// Enterキーが押されたら
			if (CheckHitKey(KEY_INPUT_RETURN))
			{
				// メッセージ表示用タイマーをリセット
				m_receponsTimer = 0;

				// 現在選択中の技を使用したことを表示
				//printfDx(L"%ls を使った！", attacks[m_attackSelect].name);

				// 選択した技の番号を記録
				m_selectedAttack[m_monsterSelect] = m_attackSelect;

				// 次のモンスターの技選択へ移る
				m_monsterSelect++;

				// 技選択カーソルを先頭に戻す
				m_attackSelect = 0;
			}
			if (CheckHitKey(KEY_INPUT_BACK) && m_monsterSelect >= 1)
			{
				m_receponsTimer = 0;
				//printfDx(L"%ls の取り消し", attacks[m_attackSelect].name);

				//前のモンスターの技に戻る
				m_monsterSelect--;

				m_selectedAttack[m_monsterSelect] = -1;
				// 技選択カーソルを先頭に戻す
				m_attackSelect = 0;
			}

		}

	}
}

void Battle::RenderAttackSelect()
{
	if (m_party == nullptr) return;
	if (m_party->GetMonsterCount() == 0) return;

	DrawBox(40, 530, 320 + m_windowWidth, 690,GetColor(0, 0, 0), TRUE);

	for (int m = 0; m < m_party->GetMonsterCount(); m++)
	{
		Monster* monster = m_party->GetMonster(m);

		std::wstring name = monster->GetName();

		auto& attacks = monster->GetAttacks();

		int x = 60 + m * 300;

		
		if (m_isJoinWindow)
		{
			return;    // 仲間画面中は技を描画しない
		}
			//各敵の技表示
			for (int i = 0; i < attacks.size(); i++)
			{
				int color = GetColor(255, 255, 255);
				if (m_selectedAttack[m] == i)
				{
					color = GetColor(255, 255, 0);
				}
				DrawString(x, 500, name.c_str(), GetColor(0, 0, 255));
				DrawString(x, 550 + i * 40, attacks[i].name, color);
				//現カーソル
				if (m == m_monsterSelect && i == m_attackSelect)
				{
					DrawString(x - 25, 550 + i * 40, L"▶", GetColor(255, 255, 0));
				}
			}
		

	}

}

void Battle::UpdateAttackAction()
{
	m_displaytextTimer++;

	if (m_displaytextTimer == 1)
	{
		Monster* monster = m_party->GetMonster(m_displayIndex);
		auto& attacks = monster->GetAttacks();

		int index = m_selectedAttack[m_displayIndex];

		Monster::CharacteRistics m_characteRistics = attacks[index].ristics;

		float magnification = 1.0f;
		switch (m_characteRistics)
		{
		case Monster::CharacteRistics::Normal:
		{
			//通常攻撃ON
			state |= USED_NORMAL;
		}
		break;
		case Monster::CharacteRistics::Fire:
		{
			//火属性攻撃ON
			state |= USED_FIRE;
			//威力 １.５倍
			magnification = 1.5f;

		}
		break;
		case Monster::CharacteRistics::Water:
		{
			//水属性攻撃ON
			state |= USED_WATER;

		}
		break;
		case Monster::CharacteRistics::Grass:
		{
			//草属性攻撃ON
			state |= USED_GRASS;

		}
		break;
		case Monster::CharacteRistics::Soil:
		{
			//土属性攻撃ON
			state |= USED_SOIL;

		}
		break;
		case Monster::CharacteRistics::Thunder:
		{
			//雷属性攻撃ON
			state |= USED_THUNDER;

		}
		break;
		case Monster::CharacteRistics::Wind:
		{
			//風属性攻撃ON
			state |= USED_WIND;

		}
		break;
		}

		m_enemy->Damage(attacks[index].power * magnification);



		m_displayMessage = monster->GetName() + L"の" + attacks[index].name + L"！";
	}
	if (m_displaytextTimer < 60)
	{
		return;
	}

	if (m_enemy->GetHp() <= 0)
	{
		// 倒した敵の名前だけ保存
		m_deadEnemyName = m_enemy->GetName();

		m_enemyDeadMotion = true;
		m_enemyDeadOffsetY = 0;

		// EnemyDeadへ移動
		m_displaytextTimer = 0;
		m_state = BattleState::EnemyDead;

		return;
	}


	if (m_displaytextTimer >= 60)
	{
		m_displaytextTimer = 0;
		m_displayIndex++;

		if (m_displayIndex >= m_party->GetMonsterCount())
		{
			m_displayMessage.clear();

			m_displayIndex = 0;
			m_monsterSelect = 0;

			std::fill(m_selectedAttack.begin(),	m_selectedAttack.end(),	-1);

			m_state = BattleState::EnemyTurn;

			m_displaytextTimer = 0;

		}
	}
}

void Battle::RenderAttackAction()
{
	if (!m_displayMessage.empty())
	{
		DrawBox(40, 530, 1240, 690,GetColor(0, 0, 0), TRUE);
		DrawString(	50,	550,m_displayMessage.c_str(),GetColor(255,255,255));
	}
}

void Battle::UpdateTool()
{

}

void Battle::RenderTool()
{
	if (m_windowWidthFront > 300)m_windowWidthFront = 300;
	if (m_windowWidth > 610)m_windowWidth = 610;
	DrawBox(340 - m_windowWidthFront, 530, 630 + m_windowWidth, 690, GetColor(0, 0, 0), TRUE);
}

void Battle::UpdateScout()
{

}

void Battle::RenderScout()
{
	if (m_windowWidthFront > 610)m_windowWidthFront = 610;
	if (m_windowWidth > 310)m_windowWidth = 310;
	DrawBox(650 - m_windowWidthFront, 530, 930 + m_windowWidth, 690, GetColor(0, 0, 0), TRUE);

	DrawString(40, 580, L"スカウト", GetColor(255, 255, 255));

}

void Battle::UpdateRun()
{
	printfDx(L"戻す (%d,%d)\n",
		m_player->m_currentposition.x,
		m_player->m_currentposition.y);


	m_player->m_position = m_player->m_currentposition;

	m_isEnemyRequested = true;
	m_isFieldRequested = true;

	m_state = BattleState::Command;
}

void Battle::RenderRun()
{
	DrawBox(950 - m_windowWidthFront, 530, 1240, 690, GetColor(0, 0, 0), TRUE);

}


void Battle::UpdateEnemyTurn(SceneManager*sceneManager)
{
	m_displaytextTimer++;
	if (m_displaytextTimer <= 60)
	{
		m_displayMessage = std::wstring(m_enemy->GetName()) + L"の攻撃!!";
		}
	else if (m_displaytextTimer <= 120)
	{
		m_displayMessage = L"全体に";

		if (m_displaytextTimer == 120)
		{
			for (int i = 0;i < m_party->GetMonsterCount();i++)//全員に同じダメージを与える
			{
				sceneManager->m_monsterCurrentDamge[i] += m_enemy->GetPower();
				m_monsterhp[i] -= m_enemy->GetPower();
				if (m_monsterhp[i] - m_enemy->GetPower() < 0)
					m_monsterhp[i] = 0;
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


void Battle::UpdateEnemyDead()
{
	m_displaytextTimer++;

	// 0〜29フレーム：やられモーション
	if (m_displaytextTimer < 30)
	{
		m_displayMessage = L"";
		m_enemyDeadOffsetY += 3; // 少しずつ下へ落とす
		return;
	}

	// 30フレーム目で倒したメッセージに切り替え
	if (m_displaytextTimer == 30)
	{
		m_displayMessage = m_deadEnemyName + L"を倒した!";
	}

	// さらに60フレーム表示したら終了
	if (m_displaytextTimer >= 90)
	{
		m_enemyDeadMotion = false;
		m_enemyDeadOffsetY = 0;

		m_displaytextTimer = 0;

		m_isEnemyRequested = true;

		m_state = BattleState::Command;
	}
}
void Battle::RenderEnemyDead()
{
	DrawBox(40, 530, 1240, 690, GetColor(0, 0, 0), TRUE);

	DrawString(	60,	550,m_displayMessage.c_str(),GetColor(255, 255, 255));
}

void Battle::UpdateAnnihilation()
{


}

void Battle::RenderAnnihilation()
{
	DrawBox(0, 0, 500, 500, GetColor(255, 255, 255), TRUE);
}

void Battle::EndTurn()
{

	m_state = BattleState::Command;

	m_IsActive = true;
	m_Window = false;

	m_windowWidth = 0;
	m_windowWidthFront = 0;

	m_select = 0;
}










void Battle::SetPlayer(PlayerManager* player)
{
	m_player = player;
}

void Battle::SetParty(Party* party)
{
	//printfDx(L"SetParty");
	m_party = party;
}
void Battle::SetEnemy(Enemy* enemy)
{
	m_enemy = enemy;
}


void Battle::SetJoinWindow(bool flag)
{
	m_isJoinWindow = flag;
}

bool Battle::IsFieldRequested()
{
	return m_isFieldRequested;
}

bool Battle::GetAnnihilation()
{
	return m_annihilation;
}

bool Battle::IsEnemyRequested()
{
	return m_isEnemyRequested;
}