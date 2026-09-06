#include"pch.h"
#include"Game/Player/PlayerMove.h"

#include"Game/Scene/FieldScene.h"
#include"Game/Player/PlayerManager.h"
#include"Game/Enemy/BossManager.h"
#include"Game/Map/Map.h"
#include"Game/Battle/Battle.h"
PlayerMove::PlayerMove()
	:m_inputManager{}

	,m_speed		{}
	,m_movetimer	{}
	,m_chipsize		{}
	,m_mapX			{}
	,m_mapY			{}
	,m_nextmapX		{}
	,m_nextmapY		{}
	,m_type			{}
	,m_nexttile		{}
	,m_hitTreasure	{}
	,m_hitNextFloor	{}
	,m_hitFall		{}
{

}
PlayerMove::~PlayerMove()
{

}



void PlayerMove::Initialize(Map* map, PlayerManager& playermanager)
{
	m_inputManager.Initialize();

	//初期値＝１マス分移動
	m_speed = playermanager.GetSpeed();
	m_chipsize = map->GetChipSize();

	m_movetimer = 0;

	m_hitTreasure = false;

	m_isFalling = false;
	m_fallScale = 1.0f;
	m_fallTimer = 0;
	m_fallPosition = { 0,0 };
}

void PlayerMove::Update(FieldScene* field, Map* map, PlayerManager* playermanager,BossManager*bossManager, Accessory* accessory,Party*party)
{
	m_mapX = static_cast<int>(playermanager->m_position.x) / m_chipsize;
	m_mapY = static_cast<int>(playermanager->m_position.y) / m_chipsize;

	m_type = map->GetTileType(m_mapX, m_mapY);
	m_movetimer--;

	//落下判定
	m_hitFall = map->IsFallRect(
		static_cast<int>(playermanager->m_position.x),
		static_cast<int>(playermanager->m_position.y),
		playermanager->m_size.x,
		playermanager->m_size.y);

	if (m_hitFall || m_isFalling)
	{
		// 落下した瞬間だけダメージ
		if (!m_isFalling)
		{
			for (int i = 0; i < party->GetMonsterCount(); i++)
			{
				party->GetMonster(i)->Damage(5);
			}
		}

		ReductionPlayer(*playermanager, map);
		return;
	}
	m_inputManager.Update();

	//宝箱
	m_hitTreasure = map->IsTreasureRect(
		static_cast<int>(playermanager->m_position.x),
		static_cast<int>(playermanager->m_position.y),
		playermanager->m_size.x,
		playermanager->m_size.y);

	if (m_hitTreasure)
	{
		field->UpdateTreasureOpen(m_inputManager, *playermanager, *map, *accessory);
		return;
	}

	//次の階
	m_hitNextFloor = map->IsNextFloorRect(
		static_cast<int>(playermanager->m_position.x),
		static_cast<int>(playermanager->m_position.y),
		playermanager->m_size.x,
		playermanager->m_size.y);

	if (m_hitNextFloor)
	{
		switch (map->GetCurrentMap())
		{
		case 0:
			field->STtext.m_signboard_1 = true;
			break;
		case 4:
			field->STtext.m_signboard_2 = true;
			break;
		case 5:
			field->STtext.m_signboard_3 = true;
			break;
		case 8:
			map->EnterBossArea();

			playermanager->m_position.x = 2 * m_chipsize;
			playermanager->m_position.y = 2 * m_chipsize;
			break;
		case 9:
			map->ExitBossArea();

			playermanager->m_position.x = 37 * m_chipsize;
			playermanager->m_position.y = 20 * m_chipsize;
			break;
		}

		return;
	}

	m_hitSignboard = map->IsSignboardRect(
		static_cast<int>(playermanager->m_position.x),
		static_cast<int>(playermanager->m_position.y),
		playermanager->m_size.x,
		playermanager->m_size.y);

	if (m_hitSignboard&&(m_inputManager.IsTrigger(KEY_INPUT_RETURN)||m_inputManager.IsPadTrigger(PAD_INPUT_A)))
	{
		switch (map->GetCurrentMap())
		{
		case 0:
			field->STtext.m_signboard_1 = true;
			break;
		case 4:
			field->STtext.m_signboard_2 = true;
			break;
		case 6:
			field->STtext.m_signboard_3 = true;
			break;
		case 8:
			field->STtext.m_signboard_4 = true;
			break;
		}
	}
	//通常移動
	if (m_movetimer < 0)
	{
		playermanager->m_oldposition = playermanager->m_position;

		if (m_inputManager.IsPress(KEY_INPUT_RIGHT)||m_inputManager.IsPadPress(PAD_INPUT_RIGHT))
		{
			playermanager->m_position.x += m_speed;
			playermanager->m_direction = playermanager->Direction::Right;
		}
		else if (m_inputManager.IsPress(KEY_INPUT_LEFT) || m_inputManager.IsPadPress(PAD_INPUT_LEFT))
		{
			playermanager->m_position.x -= m_speed;
			playermanager->m_direction = playermanager->Direction::Left;
		}
		else if (m_inputManager.IsPress(KEY_INPUT_UP) || m_inputManager.IsPadPress(PAD_INPUT_UP))
		{
			playermanager->m_position.y -= m_speed;
			playermanager->m_direction = playermanager->Direction::Up;
		}
		else if (m_inputManager.IsPress(KEY_INPUT_DOWN) || m_inputManager.IsPadPress(PAD_INPUT_DOWN))
		{
			playermanager->m_position.y += m_speed;
			playermanager->m_direction = playermanager->Direction::Down;
		}

		bool hitWall = map->IsWallRect(
			static_cast<int>(playermanager->m_position.x),
			static_cast<int>(playermanager->m_position.y),
			playermanager->m_size.x,
			playermanager->m_size.y);

		if (hitWall)
		{
			playermanager->m_position = playermanager->m_oldposition;
		}
		m_movetimer = 5;
	}
	m_hitSlime = map->IsSlimeRect(
		static_cast<int>(playermanager->m_position.x),
		static_cast<int>(playermanager->m_position.y),
		playermanager->m_size.x,
		playermanager->m_size.y);
	if (m_hitSlime&& (m_inputManager.IsTrigger(KEY_INPUT_RETURN) || m_inputManager.IsPadTrigger(PAD_INPUT_A)))
	{
		if (bossManager->IsBossDefeated(3))
		{
			field->STtext.m_end = true;
			return;
		}
		field->STtext.m_start = true;
		field->m_count = 5;
		return;
	}
}

void PlayerMove::Render(FieldScene*field,Map*map,PlayerManager*playermanager,Accessory*accessory)
{
	if(m_hitTreasure)
	{
		field->RenderTreasureOpen(*accessory);
	}
}

void PlayerMove::Finalize()
{

}

void PlayerMove::ReductionPlayer(PlayerManager& playermanager, Map* map)
{
	//初めて落下した瞬間
	if (!m_isFalling)
	{
		m_isFalling = true;
		m_fallTimer = 0;
		m_fallScale = 1.0f;
		m_fallPosition = playermanager.m_position;
	}

	m_fallTimer++;

	//徐々に小さくする
	m_fallScale -= 0.03f;

	if (m_fallScale < 0.0f)
	{
		m_fallScale = 0.0f;
	}

	//描画倍率
	playermanager.m_drawScale = m_fallScale;

	//下に落ちる
	playermanager.m_position.y += 2;

	//30フレームで落下終了
	if (m_fallTimer >= 30)
	{
		m_isFalling = false;
		playermanager.m_drawScale = 1.0f;

		//周囲から安全な場所を探す
		playermanager.m_position = FindSafePosition(map, playermanager);
	}
}

Vector2 PlayerMove::FindSafePosition(Map* map, PlayerManager& playermanager)
{
	//落下開始地点を基準にする
	int centerX = static_cast<int>(m_fallPosition.x) / m_chipsize;
	int centerY = static_cast<int>(m_fallPosition.y) / m_chipsize;

	//周囲を近い順に探す
	for (int radius = 1;radius <= 5;++radius)
	{
		for (int y = -radius;y <= radius;++y)
		{
			for (int x = -radius;x <= radius;++x)
			{
				int tx = centerX + x;
				int ty = centerY + y;

				//候補地点
				Vector2 candidate;

				candidate.x = tx * m_chipsize;
				candidate.y = ty * m_chipsize;

				//プレイヤー2×2マス全体が
				//安全かどうかをチェック
				bool safe = true;

				for (int py = 0;py < 2;++py)
				{
					for (int px = 0;px < 2;++px)
					{
						int checkX = tx + px;
						int checkY = ty + py;

						TileType type = map->GetTileType(checkX, checkY);

						//穴ならアウト
						if (type == TileType::Fall)
						{
							safe = false;
						}
					}
				}

				if (!safe)
				{
					continue;
				}

				//2×2のプレイヤーが壁に入らないか確認
				if (map->IsWallRect(
					static_cast<int>(candidate.x),
					static_cast<int>(candidate.y),
					playermanager.m_size.x,
					playermanager.m_size.y))
				{
					continue;
				}
				//2×2全部が安全
				return candidate;
			}
		}
	}
	//安全な場所が見つからなかった場合
	return m_fallPosition;
}