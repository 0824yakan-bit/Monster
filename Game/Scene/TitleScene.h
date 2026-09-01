#pragma once
#include"Game/Maths/Vector2.h"

class InputManager;
class ImageManager;
class Vector2;
class TitleScene
{
private:
	bool m_isStartRequested;
	int m_Timer;
	bool m_render;
	ImageManager* m_image;
	Vector2 drawTitlePosition;
	Vector2 drawTitleSize;
	Vector2 drawBGTitlePosition;
	Vector2 drawBGTitleSize;
	Vector2 drawCommandPosition;
	Vector2 drawCommandSize;
	Vector2 drawCurrentCommandPosition;
	Vector2 drawCurrentCommandSize;
public:
	TitleScene();
	~TitleScene();

	void Initialize(InputManager&inputmanager);
	void Update(InputManager&inputmanager);
	void Render();
	void Finalize();

	bool IsStartRequested()const;
	void SetImage(ImageManager* image);
};

