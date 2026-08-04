#pragma once
#include"Game/Maths/Vector2.h"
#include"Game/ImageManager/ImageManager.h"

class Map;
class PlayerManager;
class ImageManager;

class Enemy
{

public:
    ImageManager* m_image = nullptr;

    enum class EnemyType
    {
        Slime,
        Wolf,
        Dragon,
        Golem,
        Fairy
    };
    EnemyType type;


    const wchar_t* name;

    Vector2 position;
    Vector2 size;

    Vector2 renderPosition;
    Vector2 renderSize;

    int hp;

    int power;

    int moveTimer;

public:
    void Damage(int power);//パーティのpower
    int GetPower()const;
    int GetHp() const;
    const wchar_t* GetName() const;
    void SetImage(ImageManager* image);

public:
    Enemy();
    virtual ~Enemy() = default;

    virtual void Initialize(Map& map,int x,int y) = 0;
    virtual void Update() = 0;
    virtual void Render() = 0;//マップシーンでの描画
    virtual void Finalize() = 0;

    virtual void OnHit(PlayerManager& player) = 0;

    virtual void RenderBattle() = 0;//バトルシーンでの描画


};