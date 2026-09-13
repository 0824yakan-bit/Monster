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
		Boss_1,//ボス１撃破時
		Boss_2,//ボス２撃破時
		Boss_3,//ボス３撃破時
		LastBossAlive,//悪魔生存時
		LastBossDefeated,//悪魔撃破時
		End,//ゲーム終了時
		textend,
	};
private:
	ImageManager *m_image;
	Vector2 drawTextBoxPosition;
	Vector2 drawTextBoxSize;

	TextType m_currentTextType;
	std::wstring m_currentChara;
	int m_TextCount[TextType::textend];
	bool m_display;
	int m_fpsCounter;
	int m_displaytextLength;
	int m_displaySpeed;
	bool m_isTyping;
	bool m_blinking;
	int m_displayTimer;
	int m_receptionTimer;
	std::wstring m_displayText;
	std::wstring m_currentdisplayText;
	bool m_gameClear;
	bool m_gameOver;
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
	void Boss_1Text(int count);
	void Boss_2Text(int count);
	void Boss_3Text(int count);
	void LastBossAliveText(int count);
	void LastBossDefeatedText(int count);
	void EndText(int count);

	void DrawCooperatText();
	void CooperatText(CooperatList skill);

	bool GameClear();
	bool GameOver();

	void TypeWriter(InputManager&inputManager);
	void SkipText();

	void SetTyping();
	bool GetTyping()const;
	void SetDisplayTextLength();
};

