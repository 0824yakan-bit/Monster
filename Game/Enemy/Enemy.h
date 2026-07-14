#pragma once
#include"Game/Maths/Vector2.h"

class Map;

class Enemy
{
public:
    Vector2 position;
    Vector2 size;
    int hp;

    virtual ~Enemy() = default;

    virtual void Initialize(Map& map) = 0;
    virtual void Update() = 0;
    virtual void Render() = 0;
    virtual void Finalize() = 0;
};