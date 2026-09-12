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
    // 描画倍率
    int m_drawScale = 100;

    // 落下状態
    bool m_isFalling = false;
    int m_fallTimer = 0;
    int m_fallScale = 100;
private:
    bool m_isBattleEnemy = false;
    bool m_isBoss = false;
    int m_bossNo = -1;
    int m_enemyId = -1;
    std::wstring m_singleAttackName;
    std::wstring m_allAttackName;
public:
    void Damage(int power);//パーティのpower
    int GetPower()const;
    int GetHp() const;
    const wchar_t* GetName() const;
    void SetImage(ImageManager* image);
    Vector2 GetPosition();
    Vector2 GetSize();

    void SetBattleEnemy(bool battleEnemy);

    bool IsBattleEnemy() const;

    void SetBoss(int bossNo);
    bool IsBoss() const;
    bool IsFalling() const;
    int GetBossNo()const;
    // 敵を仲間の種類に変換
    Monster::Type GetMonsterType() const;
    void SetEnemyStats(Map& map,Party&party, int power, int hp);
    void SetEnemyId(int id);
    int GetEnemyId() const;
    void SetAttackNames(const std::wstring& singleAttackName,const std::wstring& allAttackName);
    const std::wstring& GetSingleAttackName() const;
    const std::wstring& GetAllAttackName() const;
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