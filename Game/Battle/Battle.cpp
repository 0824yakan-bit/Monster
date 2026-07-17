#include "pch.h"
#include "Game/Battle/Battle.h"
#include"Game/Party/Party.h"


Battle::Battle()
	:m_receponsTimer{}
	,m_Window{}
	,m_windowWidth{}
	,m_IsActive{}
	,m_select{}
	,m_attackSelect{}
{

}

Battle::~Battle()
{

}

void Battle::Initialize()
{

	m_select = 0;
	m_state = BattleState::Command;
	m_receponsTimer = 10;
	m_IsActive = true;
	m_Window = false;
	m_isJoinWindow = false;
	m_windowWidthFront = 0;
	m_windowWidth = 0;
	m_monsterSelect = 0;     // 今選択中の仲間
	m_attackSelect = 0;      // 今選択中の技

	m_selectedAttack.resize(m_party->GetMonsterCount(),-1);
}

void Battle::Update()
{

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
		if (!m_Window&&CheckHitKey(KEY_INPUT_RETURN))
		{
			m_receponsTimer = 0;
			m_IsActive = false;
			m_Window = true;

			switch (m_select)//指揮・もちもの・さそう・にげだすの選択　キーカーソルの位置
			{
			case 0:
				m_windowWidth = 0;

				m_state = BattleState::Attack;
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
			}
		}
		switch (m_state)//現在の状態
		{
		case BattleState::Command:
			break;

		case BattleState::Attack:
			UpdateAttack();
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
		}
		if (!m_IsActive && CheckHitKey(KEY_INPUT_BACK))//詳細から戻る
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

}

void Battle::Render()
{
	//戦闘画面
	DrawBox(10, 10, 1270, 500, GetColor(255, 255, 255), TRUE);
	//敵配置
	DrawBox(50, 50, 1230, 460, GetColor(255, 0, 0), FALSE);
	//コマンド選択位置
	DrawBox(20, 520, 1260, 700, GetColor(255, 255, 255), TRUE);
		//
	switch (m_state)
	{
	case BattleState::Command:
		RenderCommand();
		break;

	case BattleState::Attack:
		RenderAttack();
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
	}


	//敵描画
	if (m_enemy)
	{
		m_enemy->RenderBattle();
	}
	//仮ｈｐ
	DrawFormatString(
		500,
		120,
		GetColor(0, 255, 255),
		L"HP : %d",
		m_enemy->GetHp());
		
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

void Battle::UpdateAttack()
{
	if (m_party == nullptr) return;
	if (m_party->GetMonsterCount() == 0) return;

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

		if (CheckHitKey(KEY_INPUT_RETURN))
		{
			m_receponsTimer = 0;

			// 選択した技を使用
			printfDx(L"%ls を使った！",attacks[m_attackSelect].name);
			m_selectedAttack[m_monsterSelect] = m_attackSelect;
	
			m_monsterSelect++;
			m_attackSelect = 0;
		}
		if (m_monsterSelect >= m_party->GetMonsterCount())
		{
			for (int i = 0; i < m_party->GetMonsterCount(); i++)
			{
				Monster* monster = m_party->GetMonster(i);

				auto& attacks = monster->GetAttacks();

				int index = m_selectedAttack[i];

				m_enemy->Damage(attacks[index].power);
			}
		
			m_monsterSelect = 0;

			std::fill(m_selectedAttack.begin(),m_selectedAttack.end(),-1);
		}
	}
}

void Battle::RenderAttack()
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

}

void Battle::RenderRun()
{
	DrawBox(950 - m_windowWidthFront, 530, 1240, 690, GetColor(0, 0, 0), TRUE);

}


void Battle::SetParty(Party* party)
{
	printfDx(L"SetParty");
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