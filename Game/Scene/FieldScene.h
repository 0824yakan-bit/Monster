#pragma once
#include<vector>
#include"Game/Battle/Battle.h"
#include"Game/Maths/Vector2.h"
class InputManager;
class PlayerManager;
class Enemy;
class EnemyManager;
class Map;
class Battle;
class FieldScene
{
public:
	std::vector<Battle::UsedAttackInfo> m_attackEffects;
	int m_effectIndex = 0;
	int m_effectTimer = 0;
	bool m_playEffect = false;

	void SetAttackEffects(const std::vector<Battle::UsedAttackInfo>& effects);

private:
	bool m_isBattleRequested;
	ImageManager* m_image=nullptr;
	Enemy* m_hitEnemy;

	Vector2 Mposition;
	Vector2 Nposition;
	Vector2 size ;

////メニュー一覧
	bool m_isMapActive;//その階層のマップ一覧ｏｒ全階層マップ
	bool m_isMenuActive;//メニュー表示
	int m_menuListSelect;//選択中のメニュー
	enum class MenuList
	{
		Empty,//何もなし
		CooperativeMove,//連携技使用
		PartyCheck,//現在のパーティ確認
		ToolCheck,//所持中のもちもの
		OperationInstructions,//操作説明

	};
	MenuList m_menuList;
////技一覧
	bool m_isCooperatDetailActive;//詳細表示
	int m_CooperatDetailSelect;//選択中
	enum class CooperatList//技一覧
	{
		Empty,
		None,
		Fire,
		Water,
		Grass,
		Soil,
		Wind,
		Thunder,
	};
	CooperatList m_cooperatList;

public:
	FieldScene();
	~FieldScene();

	void Initialize(InputManager& inputmanager, Map& map);
	void Update(InputManager& inputManager, PlayerManager& playerManager, EnemyManager& enemyManager,Map&map,Battle&battle);
	void Render(PlayerManager& playerManager, EnemyManager& enemyManager,Map&map);
	void Finalize();


	void UpdateCooperativeMove();
	void UpdatePartyCheck();
	void UpdateToolCheck();
	void UpdateOperationInstructions();

	void RenderCooperativeMove();
	void RenderPartyCheck();
	void RenderToolCheck();
	void RenderOperationInstructions();

	void SetImage(ImageManager* image);

	bool IsBattleRequested() const;

	Enemy* GetHitEnemy() const;

	void ResetBattleRequest();
};

