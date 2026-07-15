#include "pch.h"
#include "Game/Scene/TitleScene.h"

#include"Game/InputManager/InputManager.h"
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
}

void TitleScene::Update(InputManager& inputmanager)
{
	if (CheckHitKey(KEY_INPUT_A))
	//if (inputmanager.IsRelease())
	{
		m_isStartRequested = true;
	}
}

void TitleScene::Render()
{

}

void TitleScene::Finalize()
{

}

bool TitleScene::IsStartRequested()const
{
	return m_isStartRequested;
}
