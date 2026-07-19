#pragma once
#include"Game/Maths/Vector2.h"

class Map;
class PlayerManager;

class Enemy
{

public:
    enum class EnemyType
    {
        Slime,
        Wolf,
        Dragon
    };
    EnemyType type;


    const wchar_t* name;

    Vector2 position;
    Vector2 size;

    int hp;
    int moveTimer;

public:
    void Damage(int power);
    int GetHp() const;


public:
     Enemy() = default;
    virtual ~Enemy() = default;

    virtual void Initialize(Map& map,int x,int y) = 0;
    virtual void Update() = 0;
    virtual void Render() = 0;
    virtual void Finalize() = 0;

    virtual void OnHit(PlayerManager& player) = 0;

    virtual void RenderBattle() = 0;


};