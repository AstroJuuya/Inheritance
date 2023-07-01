#pragma once

#include <iostream>
#include <string>

#include "Dice.h"
#include "Attributes.h"
#include "Weapon.h"

class MemeFighter
{
public:
    virtual const std::string GetName() const { return m_Name; }
    int GetHealth() const { return m_Attributes.hp; }
    int GetInitiative() { return m_Attributes.speed + m_Dice.Roll(2); } // speed + 2d6
    bool IsAlive() const { return m_Attributes.hp > 0; }

    void Attack(MemeFighter& other);
    virtual void SpecialMove(MemeFighter&) {};
    virtual void Tick();
    virtual void Damage(int amount);
    void Heal(int amount) { m_Attributes.hp += amount; }
    virtual ~MemeFighter() { delete p_Weapon; }
    void GiveWeapon (MemeFighter& receiver);
    int GetWeaponRank() const { return p_Weapon->GetRank(); }
    std::string GetWeaponName() const { return p_Weapon->GetName(); }

protected:
    MemeFighter(std::string name, int hp, int speed, int power, Weapon* weapon);

protected:
    Dice m_Dice;
    std::string m_Name = "Empty";
    Attributes m_Attributes;
    Weapon* p_Weapon = nullptr;
};


class MemeFrog : public MemeFighter
{
public:
    MemeFrog(std::string name, Weapon* weapon);
    void SpecialMove(MemeFighter& other) override;
    void Tick() override;
    ~MemeFrog() = default;
};


class MemeStoner : public MemeFighter
{
public:
    MemeStoner(std::string name, Weapon* weapon);
    const std::string GetName() const override;
    void SpecialMove(MemeFighter&) override;
    ~MemeStoner() = default;

private:
    bool is_super = false;
};
