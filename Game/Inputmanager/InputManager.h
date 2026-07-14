#pragma once
#include <DxLib.h>

#pragma once


class InputManager
{
private:
    char m_key[256];
    char m_oldKey[256];

public:
    InputManager() = default;
    ~InputManager() = default;

    void Initialize();
    void Update();

    // 押されている
    bool IsPress(int key) const;

    // 押した瞬間
    bool IsTrigger(int key) const;

    // 離した瞬間
    bool IsRelease(int key) const;
};