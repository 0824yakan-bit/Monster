#pragma once
#include"Game/InputManager/InputManager.h"
#include"Game/ImageManager/ImageManager.h"
#include"Game/Maths/Vector2.h"
class GameOver
{
private:
	ImageManager* m_image;
	bool m_isTitleRequest;

	Vector2 bgPosition;
	Vector2 bgSize;
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

		void SetImage(ImageManager* image);
};

