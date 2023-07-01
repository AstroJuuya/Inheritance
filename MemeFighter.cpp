#include "MemeFighter.h"

MemeFighter::MemeFighter(std::string name, int hp, int speed, int power, Weapon* weapon)
    :
        m_Name(name),
        m_Attributes(hp, speed, power),
        p_Weapon(weapon)
{}

void MemeFighter::Attack(MemeFighter& other) {
    if (other.IsAlive() && IsAlive()) {
        int damage = p_Weapon->CalculateDamage(m_Attributes, m_Dice);
        std::cout << GetName() << " attacks " << other.GetName()
            << " with their " << p_Weapon->GetName() << '.' << std::endl;
        other.Damage(damage);
    }
}

void MemeFighter::Tick() {
    // recover 1d6 hp every turn
    if (IsAlive()) {
        int amount = m_Dice.Roll(1);
        std::cout << GetName() << " recovers " << amount << " HP." << std::endl;
        Heal(amount);
    }
}

void MemeFighter::Damage(int amount) {
    m_Attributes.hp -= amount;
    std::cout << GetName() << " loses " << amount << " HP." << std::endl;
    if (!IsAlive()) {
        std::cout << GetName() << " stumbles and keels over dead." << std::endl;
    }
}

void MemeFighter::GiveWeapon (MemeFighter& receiver) {
    delete receiver.p_Weapon;
    receiver.p_Weapon = p_Weapon;
    p_Weapon = nullptr;
}


MemeFrog::MemeFrog(std::string name, Weapon* weapon)
    :
        MemeFighter(name, 69, 7, 14, weapon)
{}

void MemeFrog::SpecialMove(MemeFighter& other) {
    // 1/3 chance to deal 3d6 + 20 damage
    if(other.IsAlive()) {
        if(m_Dice.Roll(1) <= 2) {
            int damage = m_Dice.Roll(3) + 20;
            std::cout << GetName() << " roundhouse kicks " << other.GetName() << std::endl;
            other.Damage(damage);
        }
        else {
            std::cout << GetName() << " falls off the unicycle." << std::endl;
        }
    }
}

void MemeFrog::Tick() {
    // damage by 1d6 every turn
    std::cout << GetName() << " was hurt by the bad AIDS." << std::endl;
    Damage(m_Dice.Roll(1));
}


MemeStoner::MemeStoner(std::string name, Weapon* weapon)
    :
        MemeFighter(name, 80, 4, 10, weapon)
{};

const std::string MemeStoner::GetName() const {
    if (is_super) {
        return "Super " + MemeFighter::GetName();
    }
    return MemeFighter::GetName();
}

void MemeStoner::SpecialMove(MemeFighter&) {
    // 1/2 chance to power up and increase health, speed, power
    // and add "Super" to their name
    if (IsAlive()) {
        if (is_super) {
            std::cout << MemeFighter::GetName() << " smokes another fat one " << std::endl;
            return;
        }
        if (m_Dice.Roll(1) <= 3) {
            m_Attributes.hp += 10;
            m_Attributes.speed += 3;
            m_Attributes.power += (int)(69 / 42);
            is_super = true;
            std::cout << MemeFighter::GetName()
                << " smokes a fat one and becomes " << GetName() << "!" << std::endl;
        }
        else {
            std::cout << GetName() << "'s lighter isn't working." << std::endl;
        }
    }
}
