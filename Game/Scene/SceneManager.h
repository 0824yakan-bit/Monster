#pragma once

#include"TitleScene.h"
#include"FieldScene.h"
#include"BattleScene.h"

class Map;
class Battle;
class SceneManager
{
private:
	enum RenderAttack
	{
		NORMAL  = 1 << 0,//通常　攻撃
		FIRE    = 1 << 1,//　火　属性
		WATER   = 1 << 2,//　水　属性
		GRASS   = 1 << 3,//　草　属性
		SOIL    = 1 << 4,//　土　属性
		THUNDER = 1 << 5,//　雷　属性
		WIND    = 1 << 6,//　風　属性
	};
	unsigned int state = 0;
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

	std::unique_ptr<SceneID>m_scene;
public:
	int m_monsterCurrentDamge[MAX_PARTY];

public:
	SceneManager();
	~SceneManager();

	void Initialize(InputManager& inputmanager, SceneManager& sceneManager, Map&map,Party&party,ImageManager&image);
	void Update(InputManager& inputmanager,SceneManager&sceneManager, PlayerManager& playerManager, EnemyManager& enemyManager,Map&map,Party&party,Battle&battle);
	void Render(PlayerManager& playerManager, EnemyManager& enemyManager,Map&map,Party&party);
	void Finalize();

	void NextSceneID(SceneID nextSceneID);
	void ChangeScene(InputManager& inputmanager, SceneManager& sceneManager, Map&map,Party&party);

	void InitializeCurrentScene(InputManager& inputmanager, SceneManager& sceneManager, Map&map,Party&party);
	void UpdateCurrentScene(InputManager& inputmanager,SceneManager&sceneManagerz,PlayerManager& playerManager, EnemyManager& enemyManager,Map&map,Party&party,Battle&battle);
	void RenderCurrentScene(PlayerManager& playerManager, EnemyManager& enemyManager,Map&map,Party&party);
	void FinalizeCurrentScene();


	bool IsTitleRequested() const;
	void ResetTitleRequest();
};

