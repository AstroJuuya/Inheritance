#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <random>
#include <unistd.h>
#include <termios.h>

class Dice
{
public:
	int Roll( int nDice )
	{
		int total = 0;
		for( int n = 0; n < nDice; n++ )
		{
			total += d6( rng );
		}
		return total;
	}
private:
	std::uniform_int_distribution<int> d6 = std::uniform_int_distribution<int>( 1,6 );
	std::mt19937 rng = std::mt19937( std::random_device{}() );
};

struct Attributes
{
    Attributes(int hp, int speed, int power)
        :
            hp(hp),
            speed(speed),
            power(power)
    {}

    int hp = 0;
    int speed = 0;
    int power = 0;
};

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

class MemeFighter
{
public:
    virtual const std::string GetName() const { return m_Name; }
    int GetHealth() const { return m_Attributes.hp; }
    int GetInitiative() { return m_Attributes.speed + m_Dice.Roll(2); } // speed + 2d6
    bool IsAlive() const { return m_Attributes.hp > 0; }

    void Attack(MemeFighter& other) {
        if (other.IsAlive() && IsAlive()) {
            int damage = p_Weapon->CalculateDamage(m_Attributes, m_Dice);
            std::cout << GetName() << " attacks " << other.GetName()
                << " with their " << p_Weapon->GetName() << '.' << std::endl;
            other.Damage(damage);
        }
    }

    virtual void SpecialMove(MemeFighter&) {};

    virtual void Tick() {
        // recover 1d6 hp every turn
        if (IsAlive()) {
        int amount = m_Dice.Roll(1);
        std::cout << GetName() << " recovers " << amount << " HP." << std::endl;
        Heal(amount);
        }
    }

    virtual void Damage(int amount) {
        m_Attributes.hp -= amount;
        std::cout << GetName() << " loses " << amount << " HP." << std::endl;
        if (!IsAlive()) {
            std::cout << GetName() << " stumbles and keels over dead." << std::endl;
        }
    }
    void Heal(int amount) { m_Attributes.hp += amount; }
    virtual ~MemeFighter() {
        delete p_Weapon;
    }

    void GiveWeapon (MemeFighter& receiver) {
        delete receiver.p_Weapon;
        receiver.p_Weapon = p_Weapon;
        p_Weapon = nullptr;
    }

    int GetWeaponRank() const { return p_Weapon->GetRank(); }
    std::string GetWeaponName() const { return p_Weapon->GetName(); }

protected:
    MemeFighter(std::string name, int hp, int speed, int power, Weapon* weapon)
        :
            m_Name(name),
            m_Attributes(hp, speed, power),
            p_Weapon(weapon)
    {}

protected:
    Dice m_Dice;
    std::string m_Name = "Empty";
    Attributes m_Attributes;
    Weapon* p_Weapon = nullptr;
};

class MemeFrog : public MemeFighter
{
public:
    MemeFrog(std::string name, Weapon* weapon)
        :
            MemeFighter(name, 69, 7, 14, weapon)
    {}

    void SpecialMove(MemeFighter& other) override {
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

    void Tick() override {
        // damage by 1d6 every turn
        std::cout << GetName() << " was hurt by the bad AIDS." << std::endl;
        Damage(m_Dice.Roll(1));
    }

    ~MemeFrog() = default;
};

class MemeStoner : public MemeFighter
{
public:
    MemeStoner(std::string name, Weapon* weapon)
    :
        MemeFighter(name, 80, 4, 10, weapon)
    {};

    const std::string GetName() const override{
        if (is_super) {
            return "Super " + MemeFighter::GetName();
        }
        return MemeFighter::GetName();
    }

    void SpecialMove(MemeFighter&) override {
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

    ~MemeStoner() = default;

private:
    bool is_super = false;
};

void TakeWeaponOnDeath(MemeFighter& fighter_1, MemeFighter& fighter_2) {
    if ( (fighter_1.IsAlive() && fighter_2.IsAlive()) ||
            (!fighter_1.IsAlive() && !fighter_2.IsAlive()) ) {
        return;
    }
    if (fighter_1.IsAlive() && !fighter_2.IsAlive() &&
            fighter_1.GetWeaponRank() < fighter_2.GetWeaponRank()) {
        std::cout << fighter_1.GetName() << " takes " << fighter_2.GetName()
            << "'s " << fighter_2.GetWeaponName() << " from their still warm corpse." << std::endl;
        fighter_2.GiveWeapon(fighter_1);
    }
    else if (fighter_2.IsAlive() && !fighter_1.IsAlive() &&
            fighter_2.GetWeaponRank() < fighter_1.GetWeaponRank()) {
        std::cout << fighter_2.GetName() << " takes " << fighter_1.GetName()
            << "'s " << fighter_1.GetWeaponName() << " from their still warm corpse." << std::endl;
        fighter_1.GiveWeapon(fighter_2);
    }
}

void Engage( MemeFighter& f1,MemeFighter& f2 )
{
    if (f1.IsAlive() && f2.IsAlive()) {
        // pointers for sorting purposes
        auto* p1 = &f1;
        auto* p2 = &f2;
        // determine attack order
        if( p1->GetInitiative() < p2->GetInitiative() )
        {
            std::swap( p1,p2 );
        }
        // execute attacks
        p1->Attack( *p2 );
        p2->Attack( *p1 );
        // do specials
        p1->SpecialMove(*p2);
        p2->SpecialMove(*p1);
        // try to take a weapon if a fighter died
        TakeWeaponOnDeath(*p1, *p2);
    }
}

int main()
{
    // The following code uses termios.h and uninstd.h
    // to replace _kbhit() and _getch()
    //
    // Retrieve the original terminal settings
    struct termios original_settings;
    tcgetattr(STDIN_FILENO, &original_settings);

    // Create a modified copy of the settings
    struct termios modified_settings = original_settings;

    // Disable canonical mode and echoing
    modified_settings.c_lflag &= ~(ICANON | ECHO);

    // Apply the modified settings
    tcsetattr(STDIN_FILENO, TCSANOW, &modified_settings);

    std::vector<MemeFighter*> team1 = {
        new MemeFrog("Dat Boi", new Bat),
        new MemeStoner("Good Guy Greg", new Fists),
        new MemeFrog("the WB Frog", new Knife)
    };
    std::vector<MemeFighter*> team2 = {
        new MemeStoner("Chong", new Fists),
        new MemeStoner("Scumbag Steve", new Knife),
        new MemeFrog("Pepe", new Bat)
    };

    const auto alive_pred = [](MemeFighter* pf) {return pf->IsAlive();};
    std::mt19937 rng = std::mt19937( std::random_device{}() );
    while(
            std::any_of(team1.begin(), team1.end(), alive_pred) &&
            std::any_of(team2.begin(), team2.end(), alive_pred))
    {
        std::shuffle(team1.begin(), team1.end(), rng);
        std::partition(team1.begin(), team1.end(), alive_pred);
        std::shuffle(team2.begin(), team2.end(), rng);
        std::partition(team2.begin(), team2.end(), alive_pred);

        for (size_t i = 0; i < team1.size(); i++) {
            Engage(*team1[i], *team2[i]);
            std::cout << "-----------------------------------" << std::endl;
        }
        std::cout << "===================================" << std::endl;

        for (size_t i = 0; i < team1.size(); i++) {
            team1[i]->Tick();
            team2[i]->Tick();
        }

        std::cout << "Press any key to continue...";
        getchar();
        std::cout << std::endl << std::endl;
    }

    if( !std::any_of(team1.begin(), team1.end(), alive_pred) )
    {
        std::cout << "Team ONE has won!" << std::endl;
    }
    else if( !std::any_of(team2.begin(), team2.end(), alive_pred) )
    {
        std::cout << "Team TWO has won!" << std::endl;
    }
    else
    {
        std::cout << "Everybody loses... because everyone's dead!" << std::endl;
    }

    for (size_t i = 0; i < team1.size(); i++) {
        delete team1[i];
        delete team2[i];
    }

    getchar();

    // Restore the original terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &original_settings);
    return 0;
}
