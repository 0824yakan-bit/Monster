#include "pch.h"
#include "Vector2.h"
#include <cmath>

Vector2::Vector2()
    : x(0)
    , y(0)
{
}

Vector2::Vector2(int x, int y)
    : x(x)
    , y(y)
{
}

Vector2 Vector2::operator+(const Vector2& v) const //{x,y}にそれぞれ足し算
{
    return Vector2(x + v.x, y + v.y);
}

Vector2& Vector2::operator+=(const Vector2& v)//{x,y}への足し算
{
    x += v.x;
    y += v.y;
    return *this;
}

Vector2 Vector2::operator-(const Vector2& v) const//{x,y}にそれぞれ引き算
{
    return Vector2(x - v.x, y - v.y);
}

Vector2& Vector2::operator-=(const Vector2& v)//{x,y}への引き算
{
    x -= v.x;
    y -= v.y;
    return *this;
}

Vector2 Vector2::operator*(float value) const//{x,y}とそれぞれ掛け算
{
    return Vector2(x * value,y * value);
}

bool Vector2::operator==(const Vector2& other) const
{
    return x == other.x && y == other.y;
}

bool Vector2::operator!=(const Vector2& other) const
{
    return !(*this == other);
}


float Vector2::Length() const//距離
{
    return std::sqrt(x * x + y * y);
}

Vector2 Vector2::Normalize() const//正規化
{
    float len = Length();

    if (len <= 0.0f)
    {
        return Vector2(0.0f, 0.0f);
    }

    return Vector2(x / len, y / len);
}
