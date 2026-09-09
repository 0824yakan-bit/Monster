#pragma once
#include"Game/Maths/Vector2.h"
#include"Game/Scene/CooperatList.h"
class InputManager;
class ImageManager;
class FieldScene;

class TextManager
{
public:
	enum TextType
	{
		Start,//ゲーム開始時
		Signboard_1,//看板１
		Signboard_2,//看板２
		Signboard_3,//看板３
		Signboard_4,//看板４
		End,//ゲーム終了時
		textend,
	};
private:
	ImageManager *m_image;
	Vector2 drawTextBoxPosition;
	Vector2 drawTextBoxSize;

	TextType m_currentTextType;
	int m_TextCount[TextType::textend];
	bool m_display;
	bool m_blinking;
	int m_displayTimer;
	int m_receptionTimer;
	std::wstring m_displayText;

	bool m_gameClear;
public:
	TextManager();
	~TextManager();
	void Initialize();
	void Update(InputManager&inputManager,FieldScene&fieldScene);
	void Render();

	void SetImage(ImageManager* image);
	void SetDisplayText();
	bool SelectDisplayText()const;

	void StartText(int count);
	void SignBoard_1Text(int count);
	void SignBoard_2Text(int count);
	void SignBoard_3Text(int count);
	void SignBoard_4Text(int count);
	void EndText(int count);

	void DrawCooperatText();
	void CooperatText(CooperatList skill);

	bool GameClear();
};

