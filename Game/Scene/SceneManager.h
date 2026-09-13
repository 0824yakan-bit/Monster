#pragma once

#include"TitleScene.h"
#include"FieldScene.h"
#include"BattleScene.h"
#include"GameOver.h"

#include"TransitionManager.h"
class Map;
class Battle;
class SceneManager
{
public:
	std::vector <int> m_renderOrders;

private:
	ImageManager* m_image = nullptr;
	SEManager* m_sound = nullptr;
	static constexpr int MAX_PARTY = 4;
	enum class SceneID
	{
		None,
		Title,
		Field,
		Battle,
	};
	SceneID m_previousSceneID;
	SceneID m_currentSceneID;
	SceneID m_nextSceneID;
	enum class TransitionStateSceneManager
	{
		None,
		FadeOut,
		ChangeScene,
		FadeIn,
	};
	TransitionStateSceneManager m_transitionState;

	TitleScene m_titleScene;
	FieldScene m_fieldScene;
	BattleScene m_battleScene;
	GameOver m_gameOver;

	TransitionManager m_transitionManager;

	std::unique_ptr<SceneID>m_scene;
public:
	int m_monsterCurrentDamge[MAX_PARTY];
	bool m_hasOnesActive;
public:
	SceneManager(BossManager&bossManager,Party&party);
	~SceneManager();

	void Initialize(TextManager& textManager,SEManager&sound, InputManager& inputmanager, SceneManager& sceneManager, PlayerManager& playerManager, Map&map,Party&party,ImageManager&image);
	void Update(TextManager& textManager,InputManager& inputmanager,SceneManager&sceneManager, PlayerManager& playerManager, EnemyManager& enemyManager,Map&map,Party&party, Accessory& accessory);
	void Render(TextManager& textManager, PlayerManager& playerManager, EnemyManager& enemyManager,Map&map,Party&party, Accessory& accessory);
	void Finalize();

	void NextSceneID(SceneID nextSceneID);
	void ChangeScene(TextManager& textManager, InputManager& inputmanager, SceneManager& sceneManager, PlayerManager& playerManager, Map&map,Party&party);

	void InitializeCurrentScene(TextManager& textManager, InputManager& inputmanager, SceneManager& sceneManager,PlayerManager&playerManager, Map&map,Party&party);
	void UpdateCurrentScene(TextManager& textManager, InputManager& inputmanager,SceneManager&sceneManagerz,PlayerManager& playerManager, EnemyManager& enemyManager,Map&map,Party&party,Battle&battle, Accessory& accessory);
	void RenderCurrentScene(TextManager& textManager, PlayerManager& playerManager, EnemyManager& enemyManager,Map&map,Party&party, Accessory& accessory);
	void FinalizeCurrentScene();

	void SetFadeOutRequest(Map&map);
	void SetFadeInRequest(Map&map);

	bool IsTitleRequested() const;
	void ResetTitleRequest();
};

