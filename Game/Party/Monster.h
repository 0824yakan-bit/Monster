#pragma once

#include <vector>



class Monster
{
public:
    enum class Type
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

    enum class CharacteRistics
    {
        None,
        Normal,//無属性
        Fire,  //火属性
        Water, //水属性
        Grass, //草属性
        Soil,  //土属性
        Darkness,//闇属性
        Wind,  //風属性
        SteamExplosion,
        FloorBreak,
        WaterFlows,
        GrawGrass,
        Volcazation,
        Defense//防御
    };

    struct Attack
    {
        const wchar_t* name;
        int power;
        CharacteRistics element;
    };

private:

    std::wstring m_name;

    Type m_type;

    CharacteRistics characteRistics;

    int m_maxHitPoint;

    int m_currentHitPoint;

    int m_level;

    std::vector<Attack> m_attacks;
public:

    Monster(Type type);


    const std::vector<Attack>& GetAttacks() const;

    Type GetType() const;

    std::wstring GetName() const;

    int GetMaxHitPoint() const;

    int GetCurrentHitPoint() const;

    void Damage(int value);

    void Heal(int value);
};