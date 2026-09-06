#pragma once
#include <DxLib.h>

#pragma once


class InputManager
{
private:
    char m_key[256];
    char m_oldKey[256];

    int m_pad;
    int m_oldPad;
public:
    InputManager() = default;
    ~InputManager() = default;

    void Initialize();
    void Update();

    // 押されている
    bool IsPress(int key) const;
    bool IsPadPress(int button)const;
    // 押した瞬間
    bool IsTrigger(int key) const;
    bool IsPadTrigger(int button)const;
    // 離した瞬間
    bool IsRelease(int key) const;
    bool IsPadRelease(int button)const;

};