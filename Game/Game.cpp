/**
 * @file   Game.cpp
 *
 * @brief  ゲーム全体に関するソースファイル
 *
 * @author 制作者名
 *
 * @date   日付
 */

// ヘッダファイルの読み込み ===================================================
#include "Game.h"

#include "Game/Screen.h"





// メンバ関数の定義 ===========================================================
/**
 * @brief デフォルトコンストラクタ
 *
 * @param なし
 */
Game::Game()
	: m_key		 { 0 }
	, m_oldKey	 { 0 }
	,m_WorldTimer{ 0 }
{
	// 乱数の初期値を設定
	SRand(static_cast<int>(time(nullptr)));
}



/**
 * @brief デストラクタ
 */
Game::~Game()
{

}



/**
 * @brief 初期化処理
 *
 * @param なし
 *
 * @return なし
 */
void Game::Initialize()
{

	m_WorldTimer = 0;

	// ゲームの初期化
	m_map.Initialize(L"Resources/map.csv");
	m_playerManager.Initialize(&m_map);
	m_enemyManager.Initialize(m_map);
}



/**
 * @brief 更新処理
 *
 * @param なし
 *
 * @return なし
 */
void Game::Update(float elapsedTime)
{
	m_WorldTimer++;

	// キー入力の取得
	m_oldKey = m_key;
	m_key    = GetJoypadInputState(DX_INPUT_KEY_PAD1);


	// ゲームの更新
	m_playerManager.Update(&m_map);
	m_enemyManager.Update();
	for (auto& enemy : m_enemyManager.m_enemies)
	{
		if (Collisionall::HitCharacter( m_playerManager,enemy.get()))
		{
			enemy->OnHit(m_playerManager);
		}
	}
}



/**
 * @brief 描画処理
 *
 * @param[in] なし
 *
 * @return なし
 */
void Game::Render()
{
	// ゲームの描画
	m_map.Render();
	m_playerManager.Render(&m_map);
	m_enemyManager.Render();

}



/**
 * @brief 終了処理
 *
 * @param なし
 *
 * @return なし
 */
void Game::Finalize()
{
	// ゲームの終了処理
	m_map.Finalize();
	m_playerManager.Finalize();
	m_enemyManager.Finalize();
}
