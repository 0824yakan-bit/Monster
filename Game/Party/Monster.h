#pragma once

#include <vector>



class Monster
{
public:
    enum class Type
    {
        Slime,
        Wolf,
        Dragon,
        Golem,
        Fairy
    };

    enum class CharacteRistics
    {
        None,
        Normal,//無属性
        Fire,  //火属性
        Water, //水属性
        Grass, //草属性
        Soil,  //土属性
        Thunder,//雷属性
        Wind,  //風属性

        Defense//防御
    };
    CharacteRistics characteRistics;

    struct Attack
    {
        const wchar_t* name;
        int power;
        CharacteRistics ristics;
    };

private:
    Type m_type;
    std::wstring m_name;
    int m_maxHitPoint;
    int m_currentHitPoint;
    int m_level;
    std::vector<Attack> m_attacks;

public:
    std::wstring GetName() const;
    int GetCurrentHitPoint() const;
    void Damage(int value);


public:
    Monster(Type type);

    const std::vector<Attack>& GetAttacks()const;

    Type GetType() const;
};