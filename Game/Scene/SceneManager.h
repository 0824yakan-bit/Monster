#pragma once

#include"TitleScene.h"
#include"FieldScene.h"
#include"BattleScene.h"

class Map;
class Battle;
class SceneManager
{

private:
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

	std::unique_ptr<SceneID>m_scene;
public:
	int m_monsterCurrentDamge[MAX_PARTY];

public:
	SceneManager();
	~SceneManager();

	void Initialize(InputManager& inputmanager, SceneManager& sceneManager, Map&map,Party&party);
	void Update(InputManager& inputmanager,SceneManager&sceneManager, PlayerManager& playerManager, EnemyManager& enemyManager,Map&map,Party&party,Battle&battle);
	void Render(PlayerManager& playerManager, EnemyManager& enemyManager,Map&map,Party&party);
	void Finalize();

	void NextSceneID(SceneID nextSceneID);
	void ChangeScene(InputManager& inputmanager, SceneManager& sceneManager, Map&map,Party&party);

	void InitializeCurrentScene(InputManager& inputmanager, SceneManager& sceneManager, Map&map,Party&party);
	void UpdateCurrentScene(InputManager& inputmanager,SceneManager&sceneManager,PlayerManager& playerManager, EnemyManager& enemyManager,Map&map,Party&party,Battle&battle);
	void RenderCurrentScene(PlayerManager& playerManager, EnemyManager& enemyManager,Map&map,Party&party);
	void FinalizeCurrentScene();


	bool IsTitleRequested() const;
	void ResetTitleRequest();
};

