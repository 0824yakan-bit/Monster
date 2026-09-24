#include"pch.h"
#include"Game/Scene/FieldScene.h"

#include"Game/Maths/Collisionall.h"
#include"Game/Scene/TextManager.h"
#include"Game/Scene/SceneManager.h"
#include"Game/ImageManager/ImageManager.h"
#include"Game/SEManager/SEManager.h"
#include"Game/Party/Monster.h"
#include"Game/Party/Party.h"
#include"Game/Battle/Battle.h"
#include"Game/Enemy/BossManager.h"
#include"Game/Screen.h"


static std::vector<Battle::UsedAttackInfo>MakeFieldEffect(Monster::CharacteRistics element, const wchar_t* name)
{
	Battle::UsedAttackInfo info{};

	info.element = element;
	info.attackName = name;

	return{ info };
}

FieldScene::FieldScene(BossManager& bossManager, Party& party)
	: m_hitEnemy				{ nullptr }
	, STtext					{false,false,false,false,false,false,false,false,false}
	, m_isBattleRequested		{false}
	, m_isMapActive				{ false }
	, m_isMenuActive			{ false }
	, m_menuList				{ MenuList::Empty }
	, m_menuListSelect			{ 0 }
	, m_isCooperatDetailActive	{ false }
	, m_CooperatDetailSelect	{ 0 }
	, m_cooperatList			{ CooperatList::Empty }
	, m_isTreasureOpen			{ false }
	, m_breakLevel				{}
	, m_bossManager				{ bossManager }
	, m_party					{ &party }
	,m_monsterhp				{}
	,m_count					{}
	,m_annihilation				{}
	, m_hasShownBoss1Text{}
	,m_hasShownBoss2Text{}
	, m_hasShownBoss3Text{}

{

}
FieldScene::~FieldScene()
{
}

void FieldScene::Initialize(SceneManager&sceneManager,TextManager&textManager,InputManager& inputmanager, PlayerManager& playerManager, Map& map)
{

	playerManager.SetImage(m_image);
	textManager.SetImage(m_image);

	m_count = 0;
	STtext.m_signboard_1 = false;
	STtext.m_signboard_2 = false;
	STtext.m_signboard_3 = false;
	STtext.m_signboard_4 = false;
	STtext.m_boss_1 = false;
	STtext.m_boss_2 = false;
	STtext.m_boss_3 = false;
	STtext.m_lastBossAlive = false;
	STtext.m_lastBossDefeated = false;
	STtext.m_end = false;

	if (!sceneManager.m_hasOnesActive)
	{
		m_hasShownBoss1Text = false;
		m_hasShownBoss2Text = false;
		m_hasShownBoss3Text = false;
		m_hasShownLastBossAliveText = false;
		m_hasShownLastBossDefeatedText = false;
		m_hasShownStairOpened = false;
	}
	//初回だけ登録
	if (m_unlockedSkills.empty())
	{
		//初期スライムの技
		m_unlockedSkills.push_back(CooperatList::None);
		m_unlockedSkills.push_back(CooperatList::Water);
	}

	m_isMapActive = false;
	m_isMenuActive = false;

	m_isCooperatDetailActive = false;

	if (map.GetCurrentMap() == 0 && map.GetStageStartMap() == 0)
	{
		map.ChangeStage(0);
	}
	m_annihilation = false;

	m_hitEnemy = nullptr;
	m_isBattleRequested = false;

	//画像配置
	Mposition.x = 10;
	Mposition.y = 600;

	Nposition.x = 50;
	Nposition.y = 650;

	size.x = 50;
	size.y = 50;

	drawSlimePosition.x = 5*map.GetChipSize();
	drawSlimePosition.y = 5*map.GetChipSize();
	drawSlimeSize.x = 1*map.GetChipSize();
	drawSlimeSize.y = 1*map.GetChipSize();


	drawMenuBoxPosition.x = 50;
	drawMenuBoxPosition.y = 50;
	drawMenuBoxSize.x = 200;
	drawMenuBoxSize.y = 350;

	drawMenuBoxPosition_1.x = 200;
	drawMenuBoxPosition_1.y = 50;
	drawMenuBoxSize_1.x = 1030;
	drawMenuBoxSize_1.y = 630;

	drawCooperatDetailActivePosition = { 600,100 };
	drawCooperatDetailActiveSize = { 500,300 };

	drawEffectPosition = { 0,0 };
	drawEffectSize = { 1280,720 };

	m_sound->PlayTypeLoopStart(SEManager::SoundList::FieldBGM);
}

void FieldScene::Finalize()
{
}
//PAD_INPUT_1 A
//PAD_INPUT_2 B
//PAD_INPUT_3 X
//PAD_INPUT_4 Y
void FieldScene::Update(TextManager&textManager,InputManager& inputManager,GameOver&gameOver, PlayerManager& playerManager, EnemyManager& enemyManager,Map& map, Battle& battle, Accessory& accessory, Party& party)
{
	inputManager.Update();
	//味方HPを保存
	for (int i = 0; i < m_party->GetMonsterCount(); i++)
	{
		Monster* monster = m_party->GetMonster(i);
		m_monsterhp[i] = monster->GetCurrentHitPoint();
	}
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

	if (m_annihilation||textManager.GameOver())
	{
		gameOver.GameOverUpdate(inputManager);
		return;
	}
	// 階段解放状態管理
	if (m_isStairOpened)
	{
		if (inputManager.IsTrigger(KEY_INPUT_RETURN) || inputManager.IsPadTrigger(PAD_INPUT_1))
		{
			m_stairOpenTimer = 0;
			m_isStairOpened = false;
		}
		else
		{
			m_stairOpenTimer--;

			if (m_stairOpenTimer <= 0)
			{
				m_stairOpenTimer = 0;
				m_isStairOpened = false;
			}
		}
	}
	if (m_isMonsterReplaceSelect)
	{
		UpdateMonsterReplaceSelect(inputManager);
		return;
	}
	map.Update(inputManager, playerManager);

	m_breakLevel = map.GetBreakLevel();
	// スキル習得表示
	if (m_isSkillLearned)
	{
		m_skillLearnTimer--;

		if (m_skillLearnTimer <= 0)
		{
			m_skillLearnTimer = 0;
			m_isSkillLearned = false;
		}
	}
	if (textManager.GameClear())
	{
		gameOver.GameClearUpdate(inputManager);
		return;
	}
	if (STtext.m_start)textManager.StartText(m_count);
	if (STtext.m_signboard_1)textManager.SignBoard_1Text(m_count);
	if (STtext.m_signboard_2)textManager.SignBoard_2Text(m_count);
	if (STtext.m_signboard_3)textManager.SignBoard_3Text(m_count);
	if (STtext.m_signboard_4)textManager.SignBoard_4Text(m_count);
	if (STtext.m_boss_1)textManager.Boss_1Text(m_count);
	if (STtext.m_boss_2)textManager.Boss_2Text(m_count);
	if (STtext.m_boss_3)textManager.Boss_3Text(m_count);
	if (STtext.m_lastBossAlive)textManager.LastBossAliveText(m_count);
	if (STtext.m_lastBossDefeated)textManager.LastBossDefeatedText	(m_count);
	if (STtext.m_end		)textManager.EndText		(m_count);

	if (textManager.SelectDisplayText())
	{
		bool enter =
			inputManager.IsTrigger(KEY_INPUT_RETURN) ||
			inputManager.IsPadTrigger(PAD_INPUT_1);

		if (enter)
		{
			if (textManager.GetTyping())
			{
				// 文字表示中
				// → 一気に全文表示
				textManager.SkipText();
			}
			else
			{
				// 全文表示済み
				// → 次の文章へ
				m_count++;

				textManager.SetTyping();
				textManager.SetDisplayTextLength();

				m_sound->PlayTypeBackStart(
					SEManager::SoundList::Decision
				);
			}
		}

		textManager.Update(inputManager,*this);
		return;
	}


	//マップ表示・メニュー表示状態管理
	//if ((inputManager.IsTrigger(KEY_INPUT_X)||inputManager.IsPadTrigger(PAD_INPUT_3)) && !m_isMapActive && !m_isMenuActive)
	//{
	//	m_isMapActive = true;
	//}
	//else if ((inputManager.IsTrigger(KEY_INPUT_X) || inputManager.IsPadTrigger(PAD_INPUT_3)|| (inputManager.IsTrigger(KEY_INPUT_BACK) || inputManager.IsPadTrigger(PAD_INPUT_B))) && m_isMapActive)
	//{
	//	m_isMapActive = false;
	//}

	if ((inputManager.IsTrigger(KEY_INPUT_Z) || inputManager.IsPadTrigger(PAD_INPUT_4)) && !m_isMenuActive && !m_isMapActive)
	{
		m_menuListSelect = 0;
		m_CooperatDetailSelect = 0;

		m_menuList = MenuList::Empty;
		m_cooperatList = CooperatList::Empty;

		m_isMenuActive = true;
		m_sound->PlayTypeBackStart(SEManager::SoundList::Decision);
	}
	else if ((inputManager.IsTrigger(KEY_INPUT_Z) || inputManager.IsPadTrigger(PAD_INPUT_4)|| (m_isCooperatDetailActive==false&&(inputManager.IsTrigger(KEY_INPUT_BACK)) || inputManager.IsPadTrigger(PAD_INPUT_B))) && m_isMenuActive)
	{
		m_isMenuActive = false;
		m_isCooperatDetailActive = false;
		m_sound->PlayTypeBackStart(SEManager::SoundList::Cancel);
	}

	if (m_isMenuActive)
	{
		if (inputManager.IsTrigger(KEY_INPUT_UP) || inputManager.IsPadTrigger(PAD_INPUT_UP))
		{
			if (m_isCooperatDetailActive)
			{
				m_CooperatDetailSelect--;
				if (m_CooperatDetailSelect < 0)
				{
					m_CooperatDetailSelect =static_cast<int>(m_unlockedSkills.size()) - 1;
				}
			}
			else
			{
				m_menuListSelect--;
				if (m_menuListSelect < 0)
				{
					m_menuListSelect = 3;
				}
			}
			m_sound->PlayTypeBackStart(SEManager::SoundList::Cursor);
		}

		if (inputManager.IsTrigger(KEY_INPUT_DOWN) || inputManager.IsPadTrigger(PAD_INPUT_DOWN))
		{
			if (m_isCooperatDetailActive)
			{
				m_CooperatDetailSelect++;

				if (m_CooperatDetailSelect >=
					static_cast<int>(m_unlockedSkills.size()))
				{
					m_CooperatDetailSelect = 0;
				}
			}
			else
			{
				m_menuListSelect++;
				if (m_menuListSelect > 3)
				{
					m_menuListSelect = 0;
				}
			}
			m_sound->PlayTypeBackStart(SEManager::SoundList::Cursor);
		}

		if (inputManager.IsTrigger(KEY_INPUT_RETURN) || inputManager.IsPadTrigger(PAD_INPUT_1))
		{
			if (m_isCooperatDetailActive)
			{
				if (m_CooperatDetailSelect >= 0 && m_CooperatDetailSelect < static_cast<int>(m_visibleSkills.size()))
				{
					m_cooperatList = m_visibleSkills[m_CooperatDetailSelect];
				}
			}
			else
			{
				switch (m_menuListSelect)
				{
				case 0:
					m_menuList = MenuList::CooperativeMove;
					m_isCooperatDetailActive = true;
					break;

				case 1:
					m_menuList = MenuList::PartyCheck;
					break;

				case 2:
					m_menuList = MenuList::ToolCheck;
					break;

				case 3:
					m_menuList = MenuList::OperationInstructions;
					break;
				}
				m_sound->PlayTypeBackStart(SEManager::SoundList::Decision);
			}
		}
		else if (inputManager.IsTrigger(KEY_INPUT_BACK) || inputManager.IsPadTrigger(PAD_INPUT_2))
		{
			m_menuList = MenuList::Empty;
			m_menuListSelect = 0;
			m_isCooperatDetailActive = false;
			m_sound->PlayTypeBackStart(SEManager::SoundList::Cancel);
		}

		switch (m_menuList)
		{
		case MenuList::CooperativeMove:
			UpdateCooperativeMove();
			break;

		case MenuList::PartyCheck:
			UpdatePartyCheck();
			break;

		case MenuList::ToolCheck:
			UpdateToolCheck();
			break;

		case MenuList::OperationInstructions:
			UpdateOperationInstructions();
			break;

		case MenuList::Empty:
			break;
		}

		//メニューから技使用可能
		switch (m_cooperatList)//連携技未追加
		{
		case CooperatList::None:
			map.NormalBreak(playerManager);

			SetBreakEffects(map, Monster::CharacteRistics::None);

			m_isMenuActive = false;
			m_isCooperatDetailActive = false;
			m_cooperatList = CooperatList::Empty;
			break;

		case CooperatList::Fire:
			map.FireBreak(playerManager);

			SetBreakEffects(map, Monster::CharacteRistics::Fire);

			m_isMenuActive = false;
			m_isCooperatDetailActive = false;
			m_cooperatList = CooperatList::Empty;
			break;

		case CooperatList::Water:
			map.WaterBreak(playerManager);

			SetBreakEffects(map, Monster::CharacteRistics::Water);

			m_isMenuActive = false;
			m_isCooperatDetailActive = false;
			m_cooperatList = CooperatList::Empty;
			break;

		case CooperatList::Grass:
			map.GrassBreak(playerManager);

			SetBreakEffects(map, Monster::CharacteRistics::Grass);

			m_isMenuActive = false;
			m_isCooperatDetailActive = false;
			m_cooperatList = CooperatList::Empty;
			break;

		case CooperatList::Soil:
			map.SoilBreak(playerManager);

			SetBreakEffects(map, Monster::CharacteRistics::Soil);

			m_isMenuActive = false;
			m_isCooperatDetailActive = false;
			m_cooperatList = CooperatList::Empty;
			break;

		case CooperatList::Wind:
			map.WindBreak(playerManager);

			SetBreakEffects(map, Monster::CharacteRistics::Wind);

			m_isMenuActive = false;
			m_isCooperatDetailActive = false;
			m_cooperatList = CooperatList::Empty;
			break;

		case CooperatList::Darkness:
			map.DarknessBreak(playerManager);

			SetBreakEffects(map, Monster::CharacteRistics::Darkness);

			m_isMenuActive = false;
			m_isCooperatDetailActive = false;
			m_cooperatList = CooperatList::Empty;
			break;

		case CooperatList::SteamExplpsion:
			map.SteamExplosionBreak(playerManager);

			SetBreakEffects(map, Monster::CharacteRistics::SteamExplosion);

			m_isMenuActive = false;
			m_isCooperatDetailActive = false;
			m_cooperatList = CooperatList::Empty;
			break;

		case CooperatList::WaterFlows:
			map.WaterFlowsBreak(playerManager);

			SetBreakEffects(map, Monster::CharacteRistics::WaterFlows);

			m_isMenuActive = false;
			m_isCooperatDetailActive = false;
			m_cooperatList = CooperatList::Empty;
			break;

		case CooperatList::FloorBreak:
			map.FloorBreak(playerManager);

			SetBreakEffects(map, Monster::CharacteRistics::FloorBreak);

			m_isMenuActive = false;
			m_isCooperatDetailActive = false;
			m_cooperatList = CooperatList::Empty;
			break;

		case CooperatList::GrawGrass:
			map.GrowGrassBreak(playerManager);

			SetBreakEffects(map, Monster::CharacteRistics::GrawGrass);

			m_isMenuActive = false;
			m_isCooperatDetailActive = false;
			m_cooperatList = CooperatList::Empty;
			break;

		case CooperatList::Volcazation:
			map.VolcazationBreak(playerManager);

			SetBreakEffects(map, Monster::CharacteRistics::Volcazation);

			m_isMenuActive = false;
			m_isCooperatDetailActive = false;
			m_cooperatList = CooperatList::Empty;
			break;
		case CooperatList::Empty:
			break;
		}
	}

	//プレイヤー管理
	if (!m_isMapActive && !m_isMenuActive)
	{
		playerManager.Update(this, &map,&m_bossManager ,&accessory,party);
	}

	if (playerManager.m_oldposition != playerManager.m_position)
	{
		playerManager.m_invicible = false;
	}

	//エネミー管理
	enemyManager.Update(map);
	Enemy* enemy =enemyManager.CheckHit(playerManager);

	if (!playerManager.m_invicible)
	{
		if (enemy)
		{
			playerManager.m_currentposition = playerManager.m_position;

			m_hitEnemy = enemy;
			m_isBattleRequested = true;
			battle.m_isBossBattle = enemy->IsBoss();
			playerManager.m_invicible = true;
		}
	}

	//エフェクト管理
	if (m_playEffect)
	{
		m_effectTimer++;

		if (m_effectTimer >= 30)
		{
			m_effectTimer = 0;
			m_effectIndex++;

			if (m_effectIndex >= static_cast<int>(m_attackEffects.size()))
			{
				m_playEffect = false;
			}
		}
	}
	for (auto& effect : m_breakEffects)
	{
		effect.timer--;
	}

	m_breakEffects.erase(std::remove_if(m_breakEffects.begin(),m_breakEffects.end(),[](const FieldBreakEffect& effect){return effect.timer <= 0;}),	m_breakEffects.end());
	//ブレイクレベル管理
	int m_level = map.GetBreakLevel();

	switch (m_level)
	{
	case 0:
		Level5(enemyManager, map);
		break;

	case 1:
		Level2();
		break;

	case 2:
		Level3();
		break;

	case 3:
		Level4();
		break;

	case 4:
		Level5(enemyManager, map);
		break;
	}

	//Boss状態管理
	if (m_bossManager.IsAllBossDefeated())
	{
		if (!m_hasShownStairOpened)
		{
			m_hasShownStairOpened = true;

			m_isStairOpened = true;
			m_stairOpenTimer = 120;
		}
	}

	if (m_bossManager.IsBossDefeated(0))
	{
		Boss1Defeat();
	}
	if (m_bossManager.IsBossDefeated(1))
	{
		Boss2Defeat();
	}
	if (m_bossManager.IsBossDefeated(2))
	{
		Boss3Defeat();
	}
	if (m_bossManager.IsBossDefeated(3))//ラスボス撃破後
	{
		LastBossDefeat(battle);
	}

}

void FieldScene::Render(TextManager& textManager,GameOver&gameOver, PlayerManager& playerManager, EnemyManager& enemyManager, Map& map, Accessory& accessory, Party& party)
{
	map.Render();
	if (map.GetCurrentMap() == 0)
	{
		m_image->DrawSlime(drawSlimePosition, drawSlimeSize);
	}
	// LastBoss撃破後：階段解放演出
	if (m_isStairOpened)
	{
		// フェードイン
		int elapsed = 120 - m_stairOpenTimer;
		int alpha = elapsed * 6;

		if (alpha > 150)
			alpha = 150;
		if (alpha < 0)
			alpha = 0;

		// 背景暗転
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// パネル
		// 外側の影
		DrawBox(480, 210, 1440, 430, GetColor(0, 0, 0), TRUE);

		// メインパネル
		DrawBox(500, 190, 1420, 410, GetColor(18, 22, 32), TRUE);

		// 外枠
		DrawBox(500, 190, 1420, 410, GetColor(100, 180, 255), FALSE);

		// 内側の枠
		DrawBox(515, 205, 1405, 395, GetColor(50, 70, 100), FALSE);

		// タイトル
		SetFontSize(42);
		const wchar_t* title = L"◆ 新たな道が開かれた ◆";
		int titleWidth = GetDrawStringWidth(title, -1);
		DrawString(960 - titleWidth / 2, 235, title, GetColor(120, 210, 255), TRUE);

		// 区切り線
		DrawLine(600, 310, 1320, 310, GetColor(100, 130, 160));

		// メインメッセージ
		SetFontSize(34);
		const wchar_t* message = L"どこかの階段が開いたようだ";
		int messageWidth = GetDrawStringWidth(message, -1);
		DrawString(960 - messageWidth / 2, 335, message, GetColor(255, 255, 255), TRUE);

		// 決定ボタン表示
		SetFontSize(22);
		const wchar_t* button = L"   A   / ENTER で閉じる";
		int buttonWidth = GetDrawStringWidth(button, -1);

		DrawString(960 - buttonWidth / 2, 450, button, GetColor(120, 210, 255), TRUE);
		SetFontSize(30);
	}
	// スキル習得演出
	if (m_isSkillLearned)
	{
		int elapsed = 120 - m_skillLearnTimer;

		int alpha = 0;

		// フェードイン：30フレーム
		if (elapsed < 30)
		{
			alpha = elapsed * 180 / 30;
		}
		// 表示維持：30～90フレーム
		else if (elapsed < 90)
		{
			alpha = 180;
		}
		// フェードアウト：90～120フレーム
		else
		{
			alpha = (120 - elapsed) * 180 / 30;
		}

		if (alpha < 0)
			alpha = 0;

		if (alpha > 180)
			alpha = 180;

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawBox(0, 0, Screen::WIDTH, Screen::HEIGHT, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// パネル
		DrawBox(500, 300, 1420, 520, GetColor(25, 25, 35), TRUE);

		// 金色の枠
		DrawBox(500, 300, 1420, 520, GetColor(255, 215, 0), FALSE);

		// タイトル
		SetFontSize(50);

		DrawString(760, 325, L"★ スキル習得！ ★", GetColor(255, 220, 50), TRUE);

		// スキル名
		SetFontSize(45);

		const wchar_t* skillName = GetSkillName(m_learnedSkill);
		int width = GetDrawStringWidth(skillName, -1);

		DrawString(960 - width / 2, 420, skillName, GetColor(255, 255, 255), TRUE);

		SetFontSize(30);
	}
	if (STtext.m_lastBossAlive == true)
	{
		Vector2 drawLastBossPosition = { 23*map.m_chipSize,10 * map.m_chipSize };
		Vector2 drawLastBossSize = { map.m_chipSize,map.m_chipSize };
		m_image ->DrawSlime(drawLastBossPosition, drawLastBossSize);
	}
	if (!m_isTreasureOpen&&!map.m_isTransition)
	{
		enemyManager.Render();
		playerManager.Render(this, &map, &accessory);
	}

	if (m_annihilation||textManager.GameOver())
	{
		gameOver.GameOverRender();
		return;
	}
	if (textManager.GameClear())
	{
		gameOver.GameClearRender();
		return;
	}

	//モンスター交換画面
	if (m_isMonsterReplaceSelect)
	{
		RenderMonsterReplaceSelect();
		return;
	}
	if (map.m_moveDir == Map::MoveDir::Right)
	{
		map.DrawFog(-map.m_transition, 0);
		map.DrawFog(Screen::WIDTH - map.m_transition, 0);
	}
	else if (map.m_moveDir == Map::MoveDir::Left)
	{
		map.DrawFog(map.m_transition, 0);
		map.DrawFog(-Screen::WIDTH + map.m_transition, 0);
	}
	else if (map.m_moveDir == Map::MoveDir::Up)
	{
		map.DrawFog(0, map.m_transition);
		map.DrawFog(0, -Screen::HEIGHT + map.m_transition);
	}
	else if (map.m_moveDir == Map::MoveDir::Down)
	{
		map.DrawFog(0, -map.m_transition);
		map.DrawFog(0, Screen::HEIGHT - map.m_transition);
	}
	else
	{
		map.DrawFog(0, 0);
	}
	textManager.Render();

	// フィールド技エフェクト
	if (!m_breakEffects.empty())
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);

		for (const auto& effect : m_breakEffects)
		{
			Vector2 position = effect.position;
			Vector2 size ={map.GetChipSize(),map.GetChipSize()};

			switch (effect.element)
			{
			case Monster::CharacteRistics::None:
				m_image->DrawNormal(position, size);
				break;

			case Monster::CharacteRistics::Fire:
				m_image->DrawFire(position, size);
				break;

			case Monster::CharacteRistics::Water:
				m_image->DrawWater(position, size);
				break;

			case Monster::CharacteRistics::Grass:
				m_image->DrawGrass(position, size);
				break;

			case Monster::CharacteRistics::Wind:
				m_image->DrawWind(position, size);
				break;

			case Monster::CharacteRistics::Darkness:
				m_image->DrawDarkness(position, size);
				break;

			case Monster::CharacteRistics::SteamExplosion:
				m_image->DrawSteamexplosion(position, size);
				break;

			case Monster::CharacteRistics::WaterFlows:
				m_image->DrawWaterflows(position, size);
				break;

			case Monster::CharacteRistics::FloorBreak:
				m_image->DrawFloorBreak(position, size);
				break;

			case Monster::CharacteRistics::GrawGrass:
				m_image->DrawGrowgrass(position, size);
				break;

			case Monster::CharacteRistics::Volcazation:
				m_image->DrawVolcazation(position, size);
				break;

			default:
				break;
			}
		}

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	if (m_isMapActive)
	{
		DrawBox(200, 200, 900, 900, GetColor(0, 0, 0), TRUE);
		//DrawString(500, 500, L"マップオープン", GetColor(255, 255, 255), TRUE);
	}

	if (m_isMenuActive)
	{
		m_image->DrawCommandbox1(drawMenuBoxPosition, drawMenuBoxSize);
		//DrawString(500, 500, L"メニューオープン", GetColor(0, 0, 0), TRUE);


		int positionx	= 70;
		int positiony	= 82;
		int sizex		= 100;
		int sizey		= 50;

		int cursorY = positiony + 50 * m_menuListSelect;
		drawSelectCursorPosition.x = positionx;
		drawSelectCursorPosition.y = cursorY;
		drawSelectCursorSize = { 30,30 };
		m_image->DrawCommandCursor(drawSelectCursorPosition, drawSelectCursorSize);

		SetFontSize(25);

		for (int i = 0;i < static_cast<int>(MenuList::Empty);i++)
		{
			DrawString(100, 85 + i * 50, m_menuText[i], GetColor(255, 255, 255), TRUE);
		}

		SetFontSize(10);


		switch (m_menuList)
		{
		case MenuList::CooperativeMove:
			RenderCooperativeMove(textManager);
			break;

		case MenuList::PartyCheck:
			RenderPartyCheck(party);
			break;

		case MenuList::ToolCheck:
			RenderToolCheck();
			break;

		case MenuList::OperationInstructions:
			RenderOperationInstructions();
			break;

		case MenuList::Empty:
			break;
		}
	}


	if (m_image == nullptr)
	{
		printfDx(L"m_imageisnullptr!");
		return;
	}


}


void FieldScene::UpdateCooperativeMove()
{
}
void FieldScene::UpdatePartyCheck()
{
}
void FieldScene::UpdateToolCheck()
{
}
void FieldScene::UpdateOperationInstructions()
{
}

void FieldScene::RenderCooperativeMove(TextManager&textManager)
{
	// 覚えた技を表示用にコピー
	m_visibleSkills = m_unlockedSkills;

	// 技がない場合
	if (m_visibleSkills.empty())
	{
		return;
	}

	// 1ページに表示する技数
	const int skillsPerPage = 6;

	// 現在のページ
	int currentPage =m_CooperatDetailSelect / skillsPerPage;

	// ページ内での選択位置
	int selectInPage =m_CooperatDetailSelect % skillsPerPage;

	// 総ページ数
	int totalPage =(static_cast<int>(m_visibleSkills.size()) + skillsPerPage - 1)/ skillsPerPage;

	// 背景
	m_image->DrawCommandbox1(drawMenuBoxPosition_1,drawMenuBoxSize_1);

	int positionx = 280;
	int positiony = 200;

	// カーソル
	int cursorY =positiony + 50 * selectInPage;

	Vector2 drawSelectCursorPosition;
	Vector2 drawSelectCursorSize = { 40, 40 };

	drawSelectCursorPosition.x = positionx;
	drawSelectCursorPosition.y = cursorY;

	m_image->DrawCommandCursor(drawSelectCursorPosition,drawSelectCursorSize);

	SetFontSize(50);
	DrawString(300,100,L"技一覧",GetColor(255, 255, 255),TRUE);

	// 現在のページに表示する範囲
	int startIndex =currentPage * skillsPerPage;

	int endIndex =std::min(startIndex + skillsPerPage,static_cast<int>(m_visibleSkills.size()));

	// 技名表示
	int y = static_cast<int>(positiony);

	for (int i = startIndex; i < endIndex; i++)
	{
		const wchar_t* name = L"";

		switch (m_visibleSkills[i])
		{
		case CooperatList::None:
			name = L"無属性";
			break;

		case CooperatList::Fire:
			name = L"火属性";
			break;

		case CooperatList::Water:
			name = L"水属性";
			break;

		case CooperatList::Grass:
			name = L"草属性";
			break;

		case CooperatList::Soil:
			name = L"土属性";
			break;

		case CooperatList::Wind:
			name = L"風属性";
			break;

		case CooperatList::Darkness:
			name = L"闇属性";
			break;

		case CooperatList::SteamExplpsion:
			name = L"蒸界爆砕";
			break;

		case CooperatList::FloorBreak:
			name = L"地殻崩壊";
			break;

		case CooperatList::WaterFlows:
			name = L"蒼波";
			break;

		case CooperatList::GrawGrass:
			name = L"大地の恵み";
			break;

		case CooperatList::Volcazation:
			name = L"灼界";
			break;
		}
		DrawString(330,y,name,GetColor(255, 255, 255),TRUE);
		y += 50;
	}

	// ページ番号
	SetFontSize(25);

	DrawFormatString(1030,630,GetColor(255, 255, 255),L"%d / %d",currentPage + 1,totalPage);
	SetFontSize(10);

	// 技の詳細
	if (m_isCooperatDetailActive)
	{
		Vector2 detailBoxPosition = { 600, 100 };
		Vector2 detailBoxSize = { 560, 300 };

		m_image->DrawCommandbox1(detailBoxPosition,detailBoxSize);
		textManager.CooperatText(m_unlockedSkills[m_CooperatDetailSelect]);
		textManager.DrawCooperatText();
	}
}
void FieldScene::RenderPartyCheck(Party& party)
{
	m_image->DrawCommandbox1(drawMenuBoxPosition_1, drawMenuBoxSize_1);
	SetFontSize(40);
	DrawString(300, 100, L"仲間", GetColor(255,255,255), TRUE);

	int y = 200;
	for (int i = 0;i < party.GetMonsterCount();i++)
	{
		Monster* monster = party.GetMonster(i);
		if (monster == nullptr)
		{
			continue;
		}

		//名前
		DrawString(350, y, monster->GetName().c_str(), GetColor(255,255,255), TRUE);
		//HP
		DrawFormatString(650, y, GetColor(255,255,255), L"HP%d/%d", monster->GetCurrentHitPoint(), monster->GetMaxHitPoint());
		y += 80;
	}

	SetFontSize(10);
}
void FieldScene::RenderToolCheck()
{
	m_image->DrawCommandbox1(drawMenuBoxPosition_1, drawMenuBoxSize_1);
	SetFontSize(50);
	DrawFormatString(600, 300, GetColor(255, 255, 255), L"未入手");
	SetFontSize(30);
}
void FieldScene::RenderOperationInstructions()
{
	m_image->DrawCommandbox1(drawMenuBoxPosition_1, drawMenuBoxSize_1);
	Vector2 drawKeyPosition = { 450,120 };
	Vector2 drawKeySize = { 700,250 };
	Vector2 drawControllerPosition = { 550,400 };
	Vector2 drawControllerSize = { 300,200 };
	m_image->DrawKey(drawKeyPosition, drawKeySize);
	m_image->DrawController(drawControllerPosition, drawControllerSize);
	SetFontSize(20);
	DrawFormatString(300, 100, GetColor(255, 255, 255), L"←→↑↓ ：移動・選択");
	DrawFormatString(300, 150, GetColor(255, 255, 255), L"　ENTER  ：決定・話す・読む");
	DrawFormatString(300, 200, GetColor(255, 255, 255), L"BACKSPACE：戻る");
	DrawFormatString(300, 250, GetColor(255, 255, 255), L"　　Z　　：メニュー");
	
	DrawLine(300, 370, 1100, 370, GetColor(255, 255, 255), 5);

	DrawFormatString(300, 400, GetColor(255, 255, 255), L"スティック：移動・選択");
	DrawFormatString(900, 400, GetColor(255, 255, 255), L"A：決定・話す・読む");
	DrawFormatString(900, 450, GetColor(255, 255, 255), L"B：戻る");
	//DrawFormatString(900, 550, GetColor(255, 255, 255), L"X：マップ");
	DrawFormatString(900, 500, GetColor(255, 255, 255), L"Y：メニュー");



	SetFontSize(30);
}


void FieldScene::Level1()
{
}
void FieldScene::Level2()
{
}
void FieldScene::Level3()
{
}
void FieldScene::Level4()
{
}
void FieldScene::Level5(EnemyManager&enemyManager,Map&map)
{
enemyManager.Update(map);
}

void FieldScene::Boss1Defeat()
{
	if (!m_hasShownBoss1Text)
	{
		STtext.m_boss_1 = true;
		m_hasShownBoss1Text = true;
	}
}
void FieldScene::Boss2Defeat()
{
	if (!m_hasShownBoss2Text)
	{
		STtext.m_boss_2 = true;
		m_hasShownBoss2Text = true;
	}
}
void FieldScene::Boss3Defeat()
{
	if (!m_hasShownBoss3Text)
	{
		STtext.m_boss_3 = true;
		m_hasShownBoss3Text = true;
	}
}
void FieldScene::LastBossDefeat(Battle&battle)
{
	if (!m_hasShownLastBossDefeatedText)
	{
		if (battle.IsChackBossSelect())
		{
			if (!m_hasShownLastBossAliveText)
			{
				STtext.m_lastBossAlive = true;
				m_hasShownLastBossAliveText = true;
				return;
			}
		}
		else
		{
			STtext.m_lastBossDefeated = true;
			m_hasShownLastBossDefeatedText = true;
		}
	}
}

//技属性を1つ取得
void FieldScene::LearnCompositeSkill(CooperatList skill)
{
	if (HasSkill(skill))//重複を防止
	{
		return;
	}
	m_unlockedSkills.push_back(skill);
}
const wchar_t* FieldScene::GetSkillName(CooperatList skill)
{
	switch (skill)
	{
	case CooperatList::Fire:
		return L"火属性";

	case CooperatList::Water:
		return L"水属性";

	case CooperatList::Grass:
		return L"草属性";

	case CooperatList::Soil:
		return L"土属性";

	case CooperatList::Darkness:
		return L"闇属性";

	case CooperatList::Wind:
		return L"風属性";

	default:
		return L"連携スキル";
	}
}
bool FieldScene::TryLearnSkill(CooperatList skill)
{
	
	// 新しく習得した場合だけ表示
	if (HasSkill(skill))
	{
		return false;
	}
	m_unlockedSkills.push_back(skill);

	m_isSkillLearned = true;
	m_skillLearnTimer = 120; // 約2秒
	m_learnedSkill = skill;
	return true;
}
//Monsterが持っている技属性をすべて取得
void FieldScene::LearnMonsterSkills(const Monster& monster)
{
	const std::vector<Monster::Attack>& attacks = monster.GetAttacks();

	for (const Monster::Attack& attack : attacks)
	{
		switch (attack.element)
		{
		case Monster::CharacteRistics::None:
			TryLearnSkill(CooperatList::None);
			break;

		case Monster::CharacteRistics::Normal:
			TryLearnSkill(CooperatList::None);
			break;

		case Monster::CharacteRistics::Fire:
			TryLearnSkill(CooperatList::Fire);
			break;

		case Monster::CharacteRistics::Water:
			TryLearnSkill(CooperatList::Water);
			break;

		case Monster::CharacteRistics::Grass:
			TryLearnSkill(CooperatList::Grass);
			break;

		case Monster::CharacteRistics::Soil:
			TryLearnSkill(CooperatList::Soil);
			break;

		case Monster::CharacteRistics::Darkness:
			TryLearnSkill(CooperatList::Darkness);
			break;

		case Monster::CharacteRistics::Wind:
			TryLearnSkill(CooperatList::Wind);
			break;

		case Monster::CharacteRistics::Defense:
			// 防御は連携技属性として取得しない
			break;
		}
	}
}

bool FieldScene::HasSkill(CooperatList skill) const
{
	for (const auto& unlockedSkill : m_unlockedSkills)
	{
		if (unlockedSkill == skill)
		{
			return true;
		}
	}

	return false;
}
void FieldScene::RefreshPartySkills()
{
	// 更新前に使えた属性を保存
	std::vector<CooperatList> oldSkills = m_unlockedSkills;

	// 現在のパーティで使える属性技を再構築
	std::vector<CooperatList> newSkills;

	// 無属性は常に使用可能
	newSkills.push_back(CooperatList::None);

	for (int i = 0; i < m_party->GetMonsterCount(); i++)
	{
		Monster* monster = m_party->GetMonster(i);

		if (monster == nullptr)
		{
			continue;
		}

		const std::vector<Monster::Attack>& attacks = monster->GetAttacks();

		for (const Monster::Attack& attack : attacks)
		{
			CooperatList skill = CooperatList::Empty;

			switch (attack.element)
			{
			case Monster::CharacteRistics::None:
			case Monster::CharacteRistics::Normal:
				skill = CooperatList::None;
				break;

			case Monster::CharacteRistics::Fire:
				skill = CooperatList::Fire;
				break;

			case Monster::CharacteRistics::Water:
				skill = CooperatList::Water;
				break;

			case Monster::CharacteRistics::Grass:
				skill = CooperatList::Grass;
				break;

			case Monster::CharacteRistics::Soil:
				skill = CooperatList::Soil;
				break;

			case Monster::CharacteRistics::Wind:
				skill = CooperatList::Wind;
				break;

			case Monster::CharacteRistics::Darkness:
				skill = CooperatList::Darkness;
				break;

			case Monster::CharacteRistics::Defense:
			default:
				continue;
			}

			// 重複防止
			if (skill != CooperatList::Empty &&
				std::find(newSkills.begin(), newSkills.end(), skill)
				== newSkills.end())
			{
				newSkills.push_back(skill);
			}
		}
	}

	//「新しく使えるようになった属性」を探す
	for (const auto& skill : newSkills)
	{
		// 以前は持っていなかった技なら習得演出
		if (std::find(oldSkills.begin(), oldSkills.end(), skill)
			== oldSkills.end())
		{
			m_isSkillLearned = true;
			m_skillLearnTimer = 120;
			m_learnedSkill = skill;

			// 今回は1つだけ演出する
			break;
		}
	}

	// 現在のパーティで使える属性に更新
	m_unlockedSkills = newSkills;

	// 選択位置を補正
	if (m_unlockedSkills.empty())
	{
		m_CooperatDetailSelect = 0;
	}
	else if (m_CooperatDetailSelect >=
		static_cast<int>(m_unlockedSkills.size()))
	{
		m_CooperatDetailSelect =
			static_cast<int>(m_unlockedSkills.size()) - 1;
	}
}
//宝箱
void FieldScene::UpdateTreasureOpen(InputManager& inputManager, PlayerManager& playerManager, Map& map, Accessory& accessory)
{
	if (!m_isTreasureOpen)
	{
		m_isTreasureOpen = true;

		result.clear();

		int a = 3;

		std::random_device rd;
		std::mt19937 gen(rd());

		std::shuffle(accessory.elementTypes.begin(), accessory.elementTypes.end(), gen);


		result.assign(accessory.elementTypes.begin(), accessory.elementTypes.begin() + std::min<std::size_t>(a, accessory.elementTypes.size()));
	}


	if (m_isTreasureOpen)
	{
		accessory.GetAccessory(Accessory::NOMAL);


		if (inputManager.IsTrigger(KEY_INPUT_BACK) || inputManager.IsPadTrigger(PAD_INPUT_B))
		{
			accessory.Upgrade(Accessory::NOMAL);
			accessory.Upgrade(Accessory::FIRE);
			accessory.Upgrade(Accessory::WATER);
			accessory.Upgrade(Accessory::GRASS);
			accessory.Upgrade(Accessory::SOIL);
			accessory.Upgrade(Accessory::THUNDER);
			accessory.Upgrade(Accessory::WIND);

			m_isTreasureOpen = false;
			map.UsedTreasure(playerManager);
			playerManager.m_position = playerManager.m_oldposition;
		}
	}
}
void FieldScene::RenderTreasureOpen(Accessory& accessory)
{
	if (m_isTreasureOpen)
	{
		DrawBox(30, 30, 1250, 690, GetColor(255, 255, 255), TRUE);


		DrawString(200, 200, L"TreasureOpen!", GetColor(255, 255, 0));

		for (int i = 0;i < static_cast<int>(result.size());i++)
		{
			DrawFormatString(150 + i * 400, 200, GetColor(0, 0, 0), L"%d%ls", i + 1, accessory.GetElementName(static_cast<Accessory::ElementType>(result[i])));
			DrawFormatString(150 + i * 400, 250, GetColor(0, 0, 0), L"%d", accessory.GetAccessory(static_cast<Accessory::ElementType>(result[i])).level);
		}
	}
}

//各種
void FieldScene::SetImage(ImageManager* image)
{
	m_image = image;
}
void FieldScene::SetSound(SEManager* sound)
{
	m_sound = sound;
}
void FieldScene::SetAttackEffects(const std::vector<Battle::UsedAttackInfo>& effects)
{
	m_attackEffects = effects;

	m_effectIndex = 0;
	m_effectTimer = 0;

	m_playEffect = !effects.empty();
}
void FieldScene::ResetBattleRequest()
{
	m_isBattleRequested = false;
	m_hitEnemy = nullptr;
}
bool FieldScene::IsBattleRequested()const
{
	return m_isBattleRequested;
}
void FieldScene::SetBreakEffects(Map& map,Monster::CharacteRistics element)
{
	m_breakEffects.clear();
	const auto& positions = map.GetBreakEffectPositions();
	for (const auto& position : positions)
	{
		FieldBreakEffect effect{};
		effect.element = element;
		effect.position = Vector2{position.x,position.y};
		effect.timer = 30;
		m_breakEffects.push_back(effect);
	}
}

Enemy* FieldScene::GetHitEnemy()const
{
	return m_hitEnemy;
}
void FieldScene::ReceiveJoinedMonster(std::unique_ptr<Monster>monster)
{
	if (monster == nullptr)
	{
		return;
	}

	//パーティに空きがある
	if (m_party->GetMonsterCount() < 4)
	{
		m_party->AddMonster(std::move(monster));
		RefreshPartySkills();

		return;
	}

	//パーティが4体なら交換画面へ
	m_pendingJoinedMonster = std::move(monster);

	m_replaceSelect = 0;
	m_isMonsterReplaceSelect = true;
}
void FieldScene::UpdateMonsterReplaceSelect(InputManager& inputManager)
{
	if (m_pendingJoinedMonster == nullptr)
	{
		m_isMonsterReplaceSelect = false;
		return;
	}

	int count = m_party->GetMonsterCount();

	if (count <= 0)
	{
		m_isMonsterReplaceSelect = false;

		m_party->AddMonster(std::move(m_pendingJoinedMonster));

		return;
	}

	//上
	if (inputManager.IsTrigger(KEY_INPUT_UP) || inputManager.IsPadTrigger(PAD_INPUT_UP))
	{
		m_replaceSelect--;
		if (m_replaceSelect < 0)
		{
			m_replaceSelect = count - 1;
		}
		m_sound->PlayTypeBackStart(SEManager::SoundList::Cursor);
	}

	//下
	if (inputManager.IsTrigger(KEY_INPUT_DOWN) || inputManager.IsPadTrigger(PAD_INPUT_DOWN))
	{
		m_replaceSelect++;
		if (m_replaceSelect >= count)
		{
			m_replaceSelect = 0;
		}
		m_sound->PlayTypeBackStart(SEManager::SoundList::Cursor);
	}

	//決定
	if (inputManager.IsTrigger(KEY_INPUT_RETURN) || inputManager.IsPadTrigger(PAD_INPUT_A))
	{
		m_sound->PlayTypeBackStart(SEManager::SoundList::Decision);
		Monster* oldMonster = m_party->GetMonster(m_replaceSelect);
		if (oldMonster == nullptr)
		{
			return;
		}
		//古いモンスターを削除
		m_party->RemoveMonster(m_replaceSelect);

		//新しいモンスターを追加
		m_party->AddMonster(std::move(m_pendingJoinedMonster));

		//現在のパーティ構成から使える技を再構築
		RefreshPartySkills();
		m_isMonsterReplaceSelect = false;
		m_replaceSelect = 0;
	}

	//キャンセル
	if (inputManager.IsTrigger(KEY_INPUT_BACK) || inputManager.IsPadTrigger(PAD_INPUT_B))
	{
		m_sound->PlayTypeBackStart(SEManager::SoundList::Cancel);
		//仲間にするのをキャンセル
		m_pendingJoinedMonster.reset();
		m_isMonsterReplaceSelect = false;
		m_replaceSelect = 0;
	}
}
void FieldScene::RenderMonsterReplaceSelect()
{
	Vector2 drawJoinPosition = {100,80};
	Vector2 drawJoinSize = {1080,640};
	m_image->DrawCommandbox1(drawJoinPosition, drawJoinSize);
	SetFontSize(40);
	DrawString(220, 120, L"パーティがいっぱいです", GetColor(255,255,255), TRUE);
	DrawString(220, 170, L"入れ替える仲間を選んでください", GetColor(255, 255, 255), TRUE);
	//現在の4体
	for (int i = 0;i < m_party->GetMonsterCount();i++)
	{
		Monster* monster = m_party->GetMonster(i);
		if (monster == nullptr)
		{
			continue;
		}

		int y = 250 + i * 70;
		if (i == m_replaceSelect)
		{
			Vector2 drawJoinCommandPosition = { 150,y };
			Vector2 drawJoinCommandSize = {30,30};
			m_image->DrawCommandCursor(drawJoinCommandPosition, drawJoinCommandSize);
		}
		DrawString(200, y, monster->GetName().c_str(), GetColor(255, 255, 255), TRUE);
	}

	//新しく加入するモンスター
	if (m_pendingJoinedMonster != nullptr)
	{
		DrawString(700, 250, L"加入するモンスター", GetColor(255, 255, 255), TRUE);
		DrawString(700, 320, m_pendingJoinedMonster->GetName().c_str(), GetColor(255, 255, 255), TRUE);
	}

	DrawString(180, 580, L"↑↓：選択　Enter：交換　Back：やめる", GetColor(255, 255, 255), TRUE);
	DrawString(180, 630, L"※入れ替えると技が使えなくなる可能性があるぞ", GetColor(255, 255, 255), TRUE);

	SetFontSize(30);
}
