#pragma once
#include"Game/Maths/Vector2.h"
#include"Game/ImageManager/ImageManager.h"
#include "Game/Party/Monster.h"

class Map;
class Party;
class PlayerManager;
class ImageManager;

class Enemy
{

public:
    ImageManager* m_image = nullptr;

    enum class EnemyType//エネミー追加ならここ//順番を変えたらEnemyManager::Initialize・CreateRandomEnemyも変更MonsterのTypeとBattleSceneの仲間加入にも追加
    {
        Slime,  //0
        Wolf,   //1
        Fairy,  //2
        Turtle, //3
        Mole,   //4
        Fox,    //5
        Golem,  //6
        Phoenix,//7
        Dragon, //8
        Daemon, //9
    };
    EnemyType type;

    enum Direction
    {
        Up,
        Down,
        Left,
        Right,
    };
    Direction direction;

    const wchar_t* name;

    Vector2 position;
    Vector2 size;

    Vector2 renderPosition;
    Vector2 renderSize;

    int hp;

    int power;

    int level;

    int moveTimer;
    int moveCounter;
private:
    bool m_isBattleEnemy = false;
    bool m_isBoss = false;
    int m_bossNo = -1;
public:
    void Damage(int power);//パーティのpower
    int GetPower()const;
    int GetHp() const;
    const wchar_t* GetName() const;
    void SetImage(ImageManager* image);
    Vector2 GetPosition();

    void SetBattleEnemy(bool battleEnemy);

    bool IsBattleEnemy() const;

    void SetBoss(int bossNo);
    bool IsBoss() const;
    int GetBossNo()const;
    // 敵を仲間の種類に変換
    Monster::Type GetMonsterType() const;
    void SetEnemyStats(Map& map,Party&party, int power, int hp);

public:
    Enemy();
    virtual ~Enemy() = default;

    virtual void Initialize(Map& map,Party&party,int x,int y,bool isBoss) = 0;
    virtual void Update(Map&map) = 0;
    virtual void Render() = 0;//マップシーンでの描画
    virtual void Finalize() = 0;

    virtual void OnHit(PlayerManager& player) = 0;

    virtual void RenderBattle() = 0;//バトルシーンでの描画


};