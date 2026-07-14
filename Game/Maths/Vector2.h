#pragma once

class Vector2
{
public:
    int x;
    int y;

    Vector2();
    Vector2(int x, int y);

    // 演算子
    Vector2 operator+(const Vector2& v) const;
    Vector2& operator+=(const Vector2& v);
    Vector2 operator-(const Vector2& v) const;
    Vector2& operator-=(const Vector2& v);
    Vector2 operator*(float value) const;

    // ベクトル計算
    float Length() const;
    Vector2 Normalize() const;
};