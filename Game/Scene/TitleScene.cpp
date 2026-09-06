#include "pch.h"
#include "Game/Scene/TitleScene.h"

#include"Game/InputManager/InputManager.h"
#include"Game/ImageManager/ImageManager.h"
#include"Game/SEManager/SEManager.h"
TitleScene::TitleScene()
	:m_isStartRequested{}
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize(InputManager&inputmanager)
{
	m_isStartRequested = false;
	m_Timer = 0;
	m_render = false;

	drawBGTitlePosition = { 0,0 };
	drawBGTitleSize = { 1280,720 };
	drawTitlePosition = { 300,100 };
	drawTitleSize = { 700,400 };
	drawCommandPosition = { 350,500 };
	drawCommandSize = { 550,200 };
	drawCurrentCommandPosition = { 400,550 };
	drawCurrentCommandSize = { 50,50 };
	m_sound->PlayTypeLoopStart(SEManager::SoundList::TitleBGM);
}

void TitleScene::Update(InputManager& inputmanager)
{
	inputmanager.Update();
	m_Timer++;
	if (m_Timer % 30==0)
	{
		m_render = !m_render;
	}
	if (inputmanager.IsTrigger(KEY_INPUT_RETURN)||inputmanager.IsPadTrigger(PAD_INPUT_A))
	{
		m_isStartRequested = true;
		m_sound->PlayTypeBackStart(SEManager::SoundList::Decision);
	}
}

void TitleScene::Render()
{
	m_image->DrawTitle(drawBGTitlePosition, drawBGTitleSize);
	m_image->DrawTitleText(drawTitlePosition, drawTitleSize);
	m_image->DrawCommandbox1(drawCommandPosition, drawCommandSize);
	if (m_render)
	{
		m_image->DrawCommandCursor(drawCurrentCommandPosition, drawCurrentCommandSize);
		SetFontSize(30);
		DrawFormatString(550, 620, GetColor(255, 255, 255), L"AorENTER");

	}
	SetFontSize(50);
	DrawFormatString(450, 550, GetColor(255, 255, 255), L"冒険をはじめる");
	SetFontSize(20);
}

void TitleScene::Finalize()
{

}

bool TitleScene::IsStartRequested()const
{
	return m_isStartRequested;
}

void TitleScene::SetImage(ImageManager* image)
{
	m_image = image;
}

void TitleScene::SetSound(SEManager* sound)
{
	m_sound = sound;
}
