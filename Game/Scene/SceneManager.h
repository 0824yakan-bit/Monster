#pragma once

#include"TitleScene.h"
#include"FieldScene.h"
#include"BattleScene.h"
#include"Game/Player/PlayerManager.h"
#include"Game/Enemy/EnemyManager.h"
class Map;
class SceneManager
{
private:
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

	PlayerManager m_playerManager;
	EnemyManager m_enemyManager;


	std::unique_ptr<SceneID>m_scene;
public:
	SceneManager();
	~SceneManager();

	void Initialize(InputManager& inputmanager,Map&map,Party&party);
	void Update(InputManager& inputmanager, PlayerManager& playerManager, EnemyManager& enemyManager,Map&map,Party&party);
	void Render(PlayerManager& playerManager, EnemyManager& enemyManager,Map&map,Party&party);
	void Finalize();

	void NextSceneID(SceneID nextSceneID);
	void ChangeScene(InputManager& inputmanager,Map&map,Party&party);

	void InitializeCurrentScene(InputManager& inputmanager,Map&map,Party&party);
	void UpdateCurrentScene(InputManager& inputmanager,PlayerManager& playerManager, EnemyManager& enemyManager,Map&map,Party&party);
	void RenderCurrentScene(PlayerManager& playerManager, EnemyManager& enemyManager,Map&map,Party&party);
	void FinalizeCurrentScene();
};

