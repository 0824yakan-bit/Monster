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

Vector2 Vector2::operator+(const Vector2& v) const
{
    return Vector2(x + v.x, y + v.y);
}

Vector2& Vector2::operator+=(const Vector2& v)
{
    x += v.x;
    y += v.y;
    return *this;
}
Vector2 Vector2::operator-(const Vector2& v) const
{
    return Vector2(x - v.x, y - v.y);
}
Vector2& Vector2::operator-=(const Vector2& v)
{
    x -= v.x;
    y -= v.y;
    return *this;
}
Vector2 Vector2::operator*(float value) const
{
    return Vector2(x * value,y * value);
}

float Vector2::Length() const
{
    return std::sqrt(x * x + y * y);
}

Vector2 Vector2::Normalize() const
{
    float len = Length();

    if (len <= 0.0f)
    {
        return Vector2(0.0f, 0.0f);
    }

    return Vector2(x / len, y / len);
}
