#pragma once
class InputManager;
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
	TextType m_currentTextType;
	int m_TextCount[TextType::textend];
	bool m_display;
	int m_displayTimer;
	int m_receptionTimer;
	std::wstring m_displayText;

public:
	TextManager();
	~TextManager();
	void Update(InputManager&inputManager,FieldScene&fieldScene);
	void Render();

	void SetDisplayText();
	bool SelectDisplayText()const;

	void StartText(int count);
	void SignBoard_1Text(int count);
	void SignBoard_2Text(int count);
	void SignBoard_3Text(int count);
	void SignBoard_4Text(int count);
	void EndText(int count);

};

