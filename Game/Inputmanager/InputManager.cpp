#include "pch.h"
#include "Game/InputManager/InputManager.h"

void InputManager::Initialize()
{
    for (int i = 0; i < 256; i++)
    {
        m_key[i] = 0;
        m_oldKey[i] = 0;
    }
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