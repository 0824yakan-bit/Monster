#pragma once

#include <vector>



class Monster
{
public:
    enum class Type
    {
        Slime,
        Wolf,
        Dragon
    };

    enum class CharacteRistics
    {
        Normal,//無属性
        Fire,  //火属性
        Water, //水属性
        Grass, //草属性
        Soil,  //土属性
        Wind,  //風属性
        Thunder//雷属性
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
    int m_hitpoint;
    int m_level;
    std::vector<Attack> m_attacks;

public:
    std::wstring GetName() const;
    int GetHitPoint() const;

public:
    Monster(Type type);

    const std::vector<Attack>& GetAttacks()const;

    Type GetType() const;
};