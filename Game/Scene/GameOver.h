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
		void Update(InputManager&inputManager);
		void Render();
		void Finalize();

		bool IsTitleRequest();//GameOver→TitleScene
};

