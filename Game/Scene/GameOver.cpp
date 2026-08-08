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
}

void GameOver::Update(InputManager&inputManager)
{
    if (inputManager.IsTrigger(KEY_INPUT_RETURN))
    {
        printfDx(L"push enter");
        m_isTitleRequest = true;
    }
}

void GameOver::Render()
{
    DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE);

    SetFontSize(64);
    DrawString(420, 300, L"GAME OVER", GetColor(255, 0, 0), TRUE);

    SetFontSize(24);
    DrawString(430, 420, L"Enterでタイトルへ", GetColor(255, 255, 255), TRUE);
}

void GameOver::Finalize()
{
}

bool GameOver::IsTitleRequest()
{
    return m_isTitleRequest;
}