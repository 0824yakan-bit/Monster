/**
 * @file   Game.h
 *
 * @brief  ゲーム全体に関するヘッダファイル
 *
 * @author 制作者名
 *
 * @date   日付
 */

// 多重インクルードの防止 =====================================================
#pragma once




// ヘッダファイルの読み込み ===================================================
#include"Game/Map/Map.h"
#include"Game/Player/PlayerManager.h"
#include"Game/Enemy/EnemyManager.h"
#include"Game/Inputmanager/InputManager.h"
#include"Game/Scene/SceneManager.h"
#include"Game/Party/Party.h"

// クラスの宣言 ===============================================================



// クラスの定義 ===============================================================
/**
 * @brief ゲーム
 */
class Game
{
// クラス定数の宣言 -------------------------------------------------
public:
	// システム関連
	static constexpr const wchar_t* TITLE = L"Sample Game";   ///< ゲームタイトル
	//後にほかのマネージャに移す
	PlayerManager m_playerManager;
	EnemyManager m_enemyManager;
	Map m_map;
	InputManager m_inputManager;
	SceneManager m_sceneManager;
	Party m_party;

	//

// データメンバの宣言 -----------------------------------------------
private:
	// 入力関連
	int m_key;       ///< 現在のキー情報
	int m_oldKey;    ///< 前回のキー情報
public:
	int m_WorldTimer;//ゲーム内時間（１ｆ）


// メンバ関数の宣言 -------------------------------------------------
// コンストラクタ/デストラクタ
public:
	// コンストラクタ
	Game();

	// デストラクタ
	~Game();


// 操作
public:
	// 初期化処理
	void Initialize();

	// 更新処理
	void Update(float elapsedTime);

	// 描画処理
	void Render();

	// 終了処理
	void Finalize();


// 取得/設定
public:


// 内部実装
private:


};
