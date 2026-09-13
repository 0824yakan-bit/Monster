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
    bgSize={ 1280,720 };
}

void GameOver::GameOverUpdate(InputManager&inputManager)
{
    if (inputManager.IsTrigger(KEY_INPUT_RETURN) || inputManager.IsPadTrigger(PAD_INPUT_A))
    {
        m_isTitleRequest = true;
    }
}

void GameOver::GameOverRender()
{
    m_image->GameOver(bgPosition, bgSize);
    // GAME OVER
    SetFontSize(64);
    DrawString(
        405, 250,
        L"GAME OVER",
        GetColor(180, 180, 180),
        TRUE
    );

    // サブタイトル
    SetFontSize(28);
    DrawString(
        425, 340,
        L"森は、静かに消えていった",
        GetColor(150, 150, 150),
        TRUE
    );

    // 区切り線
    DrawLine(
        420, 390,
        860, 390,
        GetColor(80, 80, 80)
    );

    // 操作説明
    SetFontSize(24);
    DrawString(
        430, 450,
        L"A / Enter でタイトルへ",
        GetColor(180, 180, 180),
        TRUE
    );
}

void GameOver::GameClearUpdate(InputManager& inputManager)
{
    if (inputManager.IsTrigger(KEY_INPUT_RETURN) || inputManager.IsPadTrigger(PAD_INPUT_A))
    {
        m_isTitleRequest = true;
    }
}

void GameOver::GameClearRender()
{
    m_image->GameClear(bgPosition, bgSize);
    // タイトル
    SetFontSize(64);
    DrawString(
        390, 250,
        L"GAME CLEAR",
        GetColor(230, 245, 220),
        TRUE
    );

    // サブタイトル
    SetFontSize(28);
    DrawString(
        445, 340,
        L"森に、再び命が戻った",
        GetColor(200, 225, 190),
        TRUE
    );

    // 区切り線
    DrawLine(
        420, 390,
        860, 390,
        GetColor(120, 160, 120)
    );

    // 操作説明
    SetFontSize(24);
    DrawString(
        430, 450,
        L"A / Enter でタイトルへ",
        GetColor(220, 220, 220),
        TRUE
    );
}

void GameOver::Finalize()
{
}

bool GameOver::IsTitleRequest()const
{
    return m_isTitleRequest;
}

void GameOver::SetImage(ImageManager* image)
{
    m_image = image;
}