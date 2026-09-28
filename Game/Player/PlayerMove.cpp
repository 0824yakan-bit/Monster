#include "pch.h"
#include "Game/Player/PlayerMove.h"

#include "Game/Screen.h"
#include "Game/Scene/FieldScene.h"
#include "Game/Player/PlayerManager.h"
#include "Game/Enemy/BossManager.h"
#include "Game/Map/Map.h"
#include "Game/Battle/Battle.h"

PlayerMove::PlayerMove()
    : m_inputManager{}
    , m_speed{}
    , m_movetimer{}
    , m_chipsize{}
    , m_mapX{}
    , m_mapY{}
    , m_nextmapX{}
    , m_nextmapY{}
    , m_type{}
    , m_nexttile{}
    , m_hitTreasure{}
    , m_hitNextFloor{}
    , m_hitFall{}
    , m_alpha{ 0 }
{
}

PlayerMove::~PlayerMove()
{
}

void PlayerMove::Initialize(
    Map* map,
    PlayerManager& playermanager)
{
    m_inputManager.Initialize();

    // 1マス分の移動速度
    m_speed = playermanager.GetSpeed();

    m_chipsize = map->GetChipSize();

    m_isMoving = false;

    // 現在のマス座標を画面座標へ変換
    m_moveTarget.x =
        playermanager.m_position.x * m_chipsize;

    m_moveTarget.y =
        playermanager.m_position.y * m_chipsize;

    // 移動速度
    m_moveSpeed = 8.0f;

    m_movetimer = 0;

    m_hitTreasure = false;

    m_isFalling = false;
    m_fallScale = 1.0f;
    m_fallTimer = 0;

    // 落下開始位置も「マス座標」
    m_fallPosition = playermanager.m_position;

    m_alpha = 0;
}
void PlayerMove::Finalize()
{

}
void PlayerMove::Update(
    FieldScene* field,
    Map* map,
    PlayerManager* playermanager,
    BossManager* bossManager,
    Accessory* accessory,
    Party* party)
{
    // m_positionは「マップ上のマス座標」
    m_mapX = playermanager->m_position.x;
    m_mapY = playermanager->m_position.y;

    m_type = map->GetTileType(
        static_cast<int>(m_mapX),
        static_cast<int>(m_mapY)
    );

    m_movetimer--;

    //==================================================
    // 落下判定
    //==================================================

    m_hitFall = map->IsFallRect(
        static_cast<int>(playermanager->m_position.x),
        static_cast<int>(playermanager->m_position.y),
        playermanager->m_size.x,
        playermanager->m_size.y
    );

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

    //==================================================
    // 宝箱
    //==================================================

    m_hitTreasure = map->IsTreasureRect(
        static_cast<int>(playermanager->m_position.x),
        static_cast<int>(playermanager->m_position.y),
        playermanager->m_size.x,
        playermanager->m_size.y
    );

    if (m_hitTreasure)
    {
        field->UpdateTreasureOpen(
            m_inputManager,
            *playermanager,
            *map,
            *accessory
        );

        return;
    }

    //==================================================
    // 次の階
    //==================================================

    m_hitNextFloor = map->IsNextFloorRect(
        static_cast<int>(playermanager->m_position.x),
        static_cast<int>(playermanager->m_position.y),
        playermanager->m_size.x,
        playermanager->m_size.y
    );

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

            // マス座標
            playermanager->m_position.x = 2;
            playermanager->m_position.y = 2;

            // 画面座標へ変換
            playermanager->m_drawPosition.x =
                playermanager->m_position.x * m_chipsize;

            playermanager->m_drawPosition.y =
                playermanager->m_position.y * m_chipsize;

            break;

        case 9:
            map->ExitBossArea();

            // マス座標
            playermanager->m_position.x = 37;
            playermanager->m_position.y = 20;

            // 画面座標へ変換
            playermanager->m_drawPosition.x =
                playermanager->m_position.x * m_chipsize;

            playermanager->m_drawPosition.y =
                playermanager->m_position.y * m_chipsize;

            break;
        }

        return;
    }

    //==================================================
    // 看板
    //==================================================

    m_hitSignboard = map->IsSignboardRect(
        static_cast<int>(playermanager->m_position.x),
        static_cast<int>(playermanager->m_position.y),
        playermanager->m_size.x,
        playermanager->m_size.y
    );

    if (
        m_hitSignboard &&
        (
            m_inputManager.IsTrigger(KEY_INPUT_RETURN) ||
            m_inputManager.IsPadTrigger(PAD_INPUT_A)
            )
        )
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

    //==================================================
    // 移動中
    //==================================================

    if (m_isMoving)
    {
        Vector2 direction =
            m_moveTarget - playermanager->m_drawPosition;

        float length = direction.Length();

        if (length <= m_moveSpeed)
        {
            // 目的地に到着
            playermanager->m_drawPosition = m_moveTarget;

            m_isMoving = false;
        }
        else
        {
            direction /= length;

            playermanager->m_drawPosition +=
                direction * m_moveSpeed;
        }

        return;
    }

    //==================================================
    // 通常移動
    //==================================================

    if (!map->m_isTransition)
    {
        Vector2 nextPosition =
            playermanager->m_position;

        bool input = false;

        if (
            m_inputManager.IsPress(KEY_INPUT_RIGHT) ||
            m_inputManager.IsPadPress(PAD_INPUT_RIGHT)
            )
        {
            nextPosition.x += 1;

            playermanager->m_direction =
                PlayerManager::Direction::Right;

            input = true;
        }
        else if (
            m_inputManager.IsPress(KEY_INPUT_LEFT) ||
            m_inputManager.IsPadPress(PAD_INPUT_LEFT)
            )
        {
            nextPosition.x -= 1;

            playermanager->m_direction =
                PlayerManager::Direction::Left;

            input = true;
        }
        else if (
            m_inputManager.IsPress(KEY_INPUT_UP) ||
            m_inputManager.IsPadPress(PAD_INPUT_UP)
            )
        {
            nextPosition.y -= 1;

            playermanager->m_direction =
                PlayerManager::Direction::Up;

            input = true;
        }
        else if (
            m_inputManager.IsPress(KEY_INPUT_DOWN) ||
            m_inputManager.IsPadPress(PAD_INPUT_DOWN)
            )
        {
            nextPosition.y += 1;

            playermanager->m_direction =
                PlayerManager::Direction::Down;

            input = true;
        }

        if (input)
        {
            // 次のマスが壁か確認
            bool hitWall = map->IsWallRect(
                static_cast<int>(nextPosition.x),
                static_cast<int>(nextPosition.y),
                playermanager->m_size.x,
                playermanager->m_size.y
            );

            if (!hitWall)
            {
                // 現在位置を保存
                playermanager->m_oldposition =
                    playermanager->m_position;

                //==========================================
                // m_positionは「マス座標」
                //==========================================

                playermanager->m_position =
                    nextPosition;

                //==========================================
                // nextPositionを「画面座標」に変換
                //==========================================

                m_moveTarget.x =
                    nextPosition.x * m_chipsize;

                m_moveTarget.y =
                    nextPosition.y * m_chipsize;

                // 滑らかに移動開始
                m_isMoving = true;
            }
        }
    }

    //==================================================
    // スライム
    //==================================================

    m_hitSlime = map->IsSlimeRect(
        static_cast<int>(playermanager->m_position.x),
        static_cast<int>(playermanager->m_position.y),
        playermanager->m_size.x,
        playermanager->m_size.y
    );

    if (
        m_hitSlime &&
        (
            m_inputManager.IsTrigger(KEY_INPUT_RETURN) ||
            m_inputManager.IsPadTrigger(PAD_INPUT_A)
            )
        )
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
void PlayerMove::ReductionPlayer(
    PlayerManager& playermanager,
    Map* map)
{
    // 初めて落下した瞬間
    if (!m_isFalling)
    {
        m_isFalling = true;

        m_fallTimer = 0;
        m_fallScale = 1.0f;

        // 落下開始地点
        // ここもマス座標として保存
        m_fallPosition = playermanager.m_position;

        m_alpha = 150;
    }

    m_fallTimer++;

    // 徐々に小さくする
    m_fallScale -= 0.03f;

    if (m_fallScale < 0.0f)
    {
        m_fallScale = 0.0f;
    }

    // プレイヤーの描画倍率
    playermanager.m_drawScale =
        m_fallScale;

    //==================================================
    // 落下アニメーション
    // m_positionは変更しない
    //==================================================

    playermanager.m_drawPosition.y += 2.0f;

    // 30フレームで落下終了
    if (m_fallTimer >= 30)
    {
        m_isFalling = false;

        playermanager.m_drawScale = 1.0f;

        // 安全な場所を探す
        Vector2 safePosition =
            FindSafePosition(map, playermanager);

        //==============================================
        // 安全な場所は「マス座標」
        //==============================================

        playermanager.m_position =
            safePosition;

        //==============================================
        // マス座標 → 画面座標
        //==============================================

        playermanager.m_drawPosition.x =
            safePosition.x * m_chipsize;

        playermanager.m_drawPosition.y =
            safePosition.y * m_chipsize;

        // 移動状態をリセット
        m_moveTarget =
            playermanager.m_drawPosition;

        m_isMoving = false;
    }
}
void PlayerMove::Render(FieldScene* field, Map* map, PlayerManager* playermanager, Accessory* accessory)
{
    if (m_alpha > 0) { m_alpha--; } if (m_hitTreasure) { field->RenderTreasureOpen(*accessory); } // 落下中だけ赤い画面演出 
    if (m_isFalling) { SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alpha);
    DrawBox(0,0,Screen::WIDTH,Screen::HEIGHT,GetColor(255, 0, 0),TRUE); 
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); } 
}

Vector2 PlayerMove::FindSafePosition(
    Map* map,
    PlayerManager& playermanager)
{
    // 落下開始地点を基準にする
    // m_fallPositionはマス座標
    int centerX =
        static_cast<int>(m_fallPosition.x);

    int centerY =
        static_cast<int>(m_fallPosition.y);

    // 周囲を近い順に探す
    for (int radius = 1; radius <= 10; ++radius)
    {
        for (int y = -radius; y <= radius; ++y)
        {
            for (int x = -radius; x <= radius; ++x)
            {
                int tx = centerX + x;
                int ty = centerY + y;

                //==========================================
                // 候補地点
                //
                // candidateは「マス座標」
                //==========================================

                Vector2 candidate;

                candidate.x = tx;
                candidate.y = ty;

                //==========================================
                // プレイヤー2×2マス全体が安全か確認
                //==========================================

                bool safe = true;

                for (int py = 0; py < 2; ++py)
                {
                    for (int px = 0; px < 2; ++px)
                    {
                        int checkX = tx + px;
                        int checkY = ty + py;

                        TileType type =
                            map->GetTileType(
                                checkX,
                                checkY
                            );

                        // 穴ならアウト
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

                //==========================================
                // 2×2のプレイヤーが壁に入らないか確認
                //
                // IsWallRectが「マス座標」を受け取る設計なら
                // candidateをそのまま渡す
                //==========================================

                if (map->IsWallRect(
                    tx,
                    ty,
                    playermanager.m_size.x,
                    playermanager.m_size.y))
                {
                    continue;
                }

                // 2×2全部が安全
                return candidate;
            }
        }
    }

    // 安全な場所が見つからなかった場合
    return m_fallPosition;
}
void PlayerMove::SetPosition(PlayerManager& playerManager)
{
    m_moveTarget = playerManager.m_drawPosition;
    m_isMoving = false;
}