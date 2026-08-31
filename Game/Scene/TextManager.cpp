#include "pch.h"
#include "Game/Scene/TextManager.h"

#include"Game/InputManager/InputManager.h"
#include"Game/Scene/FieldScene.h"

TextManager::TextManager()
	:m_display{false}
	,m_displayTimer{}
	,m_displayText{}
	,m_receptionTimer{}
	,m_currentTextType{}
{
	m_TextCount[Start]			= 5;
	m_TextCount[Signboard_1]	= 5;
	m_TextCount[Signboard_2]	= 5;
	m_TextCount[Signboard_3]	= 5;
	m_TextCount[End]			= 5;
}

TextManager::~TextManager()
{
}

void TextManager::Update(InputManager& inputManager,FieldScene&fieldScene)
{
	if (!m_display)
	{
		return;
	}

	// Enterでテキストを閉じる
	if (fieldScene.m_count==m_TextCount[m_currentTextType])
	{
		m_display = false;
		fieldScene.m_count = 0;
		switch (m_currentTextType)
		{
		case Start:
			fieldScene.STtext.m_start = false;
			break;
		case Signboard_1:
			fieldScene.STtext.m_signboard_1 = false;
			break;
		case Signboard_2:
			fieldScene.STtext.m_signboard_2 = false;
			break;
		case Signboard_3:
			fieldScene.STtext.m_signboard_3 = false;
			break;
		case End:
			fieldScene.STtext.m_end = false;
			break;
		}
	}
}
void TextManager::Render()
{
	if (!m_display)
	{
		return;
	}

	DrawBox(100, 500, 1180, 680,GetColor(0, 0, 0), TRUE);

	DrawString(150,550,m_displayText.c_str(), GetColor(255, 255, 255), TRUE);
}
void TextManager::SetDisplayText()
{
	m_display = true;
}
bool TextManager::SelectDisplayText()const
{
	return m_display;
}

void TextManager::StartText(int count)
{
	m_display = true;
	m_currentTextType = TextType::Start;
	switch (count)
	{
	case 0:
		m_displayText = L"初めまして";
		break;
	}
}

void TextManager::SignBoard_1Text(int count)
{
	m_display = true;

	m_currentTextType = TextType::Signboard_1;
	switch (count)
	{
	case 0:
		m_displayText = L"ボード１";
		break;
	}
}

void TextManager::SignBoard_2Text(int count)
{
	m_display = true;

	m_currentTextType = TextType::Signboard_2;
	switch (count)
	{
	case 0:
		m_displayText = L"ボード２";
		break;
	}
}

void TextManager::SignBoard_3Text(int count)
{
	m_display = true;

	m_currentTextType = TextType::Signboard_3;
	switch (count)
	{
	case 0:
		m_displayText = L"ボード3";
		break;
	}
}

void TextManager::EndText(int count)
{
	m_display = true;

	m_currentTextType = TextType::End;
	switch (count)
	{
	case 0:
		m_displayText = L"終わり";
		break;
	}
}
