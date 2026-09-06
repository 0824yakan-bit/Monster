#include "pch.h"
#include "Game/InputManager/InputManager.h"

void InputManager::Initialize()
{
    for (int i = 0; i < 256; i++)
    {
        m_key[i] = 0;
        m_oldKey[i] = 0;
    }
    m_pad = 0;
    m_oldPad = 0;
}

void InputManager::Update()
{
    // 前フレームの状態を保存
    for (int i = 0; i < 256; i++)
    {
        m_oldKey[i] = m_key[i];
    }

    // 現在のキー状態を取得
    GetHitKeyStateAll(m_key);
    m_oldPad = m_pad;
    m_pad = GetJoypadInputState(DX_INPUT_PAD1);
}

bool InputManager::IsPress(int key) const
{
    return m_key[key] != 0;
}

bool InputManager::IsTrigger(int key) const
{
    return m_key[key] && !m_oldKey[key];
}

bool InputManager::IsRelease(int key) const
{
    return !m_key[key] && m_oldKey[key];
}
bool InputManager::IsPadPress(int button) const
{
    return (m_pad & button) != 0;
}

bool InputManager::IsPadTrigger(int button) const
{
    return (m_pad & button) && !(m_oldPad & button);
}

bool InputManager::IsPadRelease(int button) const
{
    return !(m_pad & button) && (m_oldPad & button);
}