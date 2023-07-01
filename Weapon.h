#pragma once

#include "Dice.h"
#include "Attributes.h"

class Weapon
{
public:
    Weapon(std::string name, int rank) : m_name(name), m_rank(rank) {}
    virtual ~Weapon() = default;

    virtual int CalculateDamage(const Attributes&, Dice&) const = 0;
    std::string GetName() { return m_name; }
    int GetRank() { return m_rank; }

private:
    std::string m_name;
    int m_rank;
};

class Fists : public Weapon
{
public:
    Fists() : Weapon("fists", 0) {}
    ~Fists() = default;
    virtual int CalculateDamage(const Attributes& attributes, Dice& dice) const override {
        return attributes.power + dice.Roll(2);
    }
};

class Bat : public Weapon
{
public:
    Bat() : Weapon("bat", 1) {}
    ~Bat() = default;

    virtual int CalculateDamage(const Attributes& attributes, Dice& dice) const override {
        return attributes.power * 2 + dice.Roll(1);
    }
};

class Knife : public Weapon
{
public:
    Knife() : Weapon("knife", 2) {}
    ~Knife() = default;

    virtual int CalculateDamage(const Attributes& attributes, Dice& dice) const override {
        return attributes.speed * 3 + dice.Roll(3);
    }
};
