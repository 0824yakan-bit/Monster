#pragma once

#include"TitleScene.h"
#include"FieldScene.h"
#include"BattleScene.h"

#include"GameOver.h"
class Map;
class Battle;
class SceneManager
{
public:
	std::vector <int> m_renderOrders;

private:
	ImageManager* m_image = nullptr;

	static constexpr int MAX_PARTY = 4;
	enum class SceneID
	{
		None,
		Title,
		Field,
		Battle,
	};
	SceneID m_currentSceneID;
	SceneID m_nextSceneID;

	TitleScene m_titleScene;
	FieldScene m_fieldScene;
	BattleScene m_battleScene;

	GameOver m_gameOver;

	std::unique_ptr<SceneID>m_scene;
public:
	int m_monsterCurrentDamge[MAX_PARTY];

public:
	SceneManager(BossManager&bossManager,Party&party);
	~SceneManager();

	void Initialize(InputManager& inputmanager, SceneManager& sceneManager, PlayerManager& playerManager, Map&map,Party&party,ImageManager&image);
	void Update(InputManager& inputmanager,SceneManager&sceneManager, PlayerManager& playerManager, EnemyManager& enemyManager,Map&map,Party&party,Battle&battle, Accessory& accessory);
	void Render(PlayerManager& playerManager, EnemyManager& enemyManager,Map&map,Party&party, Accessory& accessory);
	void Finalize();

	void NextSceneID(SceneID nextSceneID);
	void ChangeScene(InputManager& inputmanager, SceneManager& sceneManager, PlayerManager& playerManager, Map&map,Party&party);

	void InitializeCurrentScene(InputManager& inputmanager, SceneManager& sceneManager,PlayerManager&playerManager, Map&map,Party&party);
	void UpdateCurrentScene(InputManager& inputmanager,SceneManager&sceneManagerz,PlayerManager& playerManager, EnemyManager& enemyManager,Map&map,Party&party,Battle&battle, Accessory& accessory);
	void RenderCurrentScene(PlayerManager& playerManager, EnemyManager& enemyManager,Map&map,Party&party, Accessory& accessory);
	void FinalizeCurrentScene();


	bool IsTitleRequested() const;
	void ResetTitleRequest();
};

