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
        Attack,
        Heal,
        Defend,
        Buff
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
    int m_hp;
    int m_level;
    std::vector<Attack> m_attacks;

public:
    std::wstring GetName() const;

public:
    Monster(Type type);

    const std::vector<Attack>& GetAttacks()const;

    Type GetType() const;
};