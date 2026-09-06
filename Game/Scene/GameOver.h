#pragma once
#include"Game/InputManager/InputManager.h"
class GameOver
{
private:
	bool m_isTitleRequest;
public:
	GameOver();
		~GameOver();

		void Initialize();
		void GameOverUpdate(InputManager&inputManager);
		void GameOverRender();
		void GameClearUpdate(InputManager& inputManager);
		void GameClearRender();
		void Finalize();

		bool IsTitleRequest()const;//GameOver→TitleScene
};

