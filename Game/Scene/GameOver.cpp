#include "pch.h"
#include "Game/Scene/GameOver.h"

GameOver::GameOver()
{
}

GameOver::~GameOver()
{
}

void GameOver::Initialize()
{
	m_isTitleRequest = false;
	bgPosition = { 0,0 };
	bgSize = { 1280,720 };
}

void GameOver::GameOverUpdate(InputManager& inputManager)
{
	if (inputManager.IsTrigger(KEY_INPUT_RETURN) ||
		inputManager.IsPadTrigger(PAD_INPUT_A))
	{
		m_isTitleRequest = true;
	}
}

void GameOver::GameOverRender()
{
	DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE);
	// 背景
	m_image->GameOver(bgPosition, bgSize);

	// 半透明パネル
	// パネルの範囲
	int panelLeft = 330;
	int panelTop = 190;
	int panelRight = 950;
	int panelBottom = 530;

	// 半透明の黒
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);

	DrawBox(panelLeft,panelTop,panelRight,panelBottom,GetColor(20, 35, 30),TRUE);

	// ブレンドモードを戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// パネルの枠線
	DrawBox(panelLeft,panelTop,panelRight,panelBottom,GetColor(180, 210, 170),FALSE);

	// GAME OVER
	SetFontSize(64);

	const wchar_t* title = L"GAME OVER";

	int titleWidth =GetDrawStringWidth(title, (int)wcslen(title));
	int titleX =(1280 - titleWidth) / 2;
	DrawString(titleX,250,title,GetColor(220, 220, 220),GetColor(30, 45, 40));

	// サブタイトル
	SetFontSize(28);

	const wchar_t* subtitle =L"森は、静かに消えていった";
	int subtitleWidth =GetDrawStringWidth(subtitle, (int)wcslen(subtitle));
	int subtitleX =(1280 - subtitleWidth) / 2;
	DrawString(subtitleX,340,subtitle,GetColor(190, 190, 190),GetColor(30, 45, 40));

	// 区切り線
	DrawLine(420,390,860,390,GetColor(100, 120, 110));

	// 操作説明
	SetFontSize(24);
	
	const wchar_t* guide =L"A / Enter でタイトルへ";

	int guideWidth =GetDrawStringWidth(guide, (int)wcslen(guide));
	int guideX =(1280 - guideWidth) / 2;
	DrawString(guideX,450,guide,GetColor(230, 230, 220),GetColor(30, 45, 40));
}

void GameOver::GameClearUpdate(InputManager& inputManager)
{
	if (inputManager.IsTrigger(KEY_INPUT_RETURN) ||
		inputManager.IsPadTrigger(PAD_INPUT_A))
	{
		m_isTitleRequest = true;
	}
}

void GameOver::GameClearRender()
{
	// 背景
	m_image->GameClear(bgPosition, bgSize);

	// 半透明パネル
	int panelLeft = 330;
	int panelTop = 190;
	int panelRight = 950;
	int panelBottom = 530;

	// 半透明の深い緑
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 140);

	DrawBox(panelLeft,panelTop,panelRight,panelBottom,GetColor(20, 70, 55),TRUE);

	// ブレンド解除
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// パネルの枠
	DrawBox(panelLeft,panelTop,panelRight,panelBottom,GetColor(210, 235, 180),FALSE);

	// GAME CLEAR
	SetFontSize(64);

	const wchar_t* title = L"GAME CLEAR";

	int titleWidth =GetDrawStringWidth(title, (int)wcslen(title));
	int titleX =(1280 - titleWidth) / 2;
	DrawString(titleX,250,title,GetColor(255, 244, 214),GetColor(22, 74, 58));

	// サブタイトル
	SetFontSize(28);

	const wchar_t* subtitle =L"森に、再び命が戻った";

	int subtitleWidth =GetDrawStringWidth(subtitle, (int)wcslen(subtitle));
	int subtitleX =(1280 - subtitleWidth) / 2;

	DrawString(subtitleX,340,subtitle,GetColor(200, 225, 190),GetColor(22, 74, 58));

	// 区切り線
	DrawLine(420,390,860,390,GetColor(190, 220, 150));

	// 操作説明
	SetFontSize(24);

	const wchar_t* guide =L"A / Enter でタイトルへ";

	int guideWidth =GetDrawStringWidth(guide, (int)wcslen(guide));
	int guideX =(1280 - guideWidth) / 2;
	DrawString(guideX,450,guide,GetColor(255, 255, 240),GetColor(23, 59, 92));
}

void GameOver::Finalize()
{
}

bool GameOver::IsTitleRequest() const
{
	return m_isTitleRequest;
}

void GameOver::SetImage(ImageManager* image)
{
	m_image = image;
}