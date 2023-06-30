#include <iostream>
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

class MemeFighter
{
public:
    virtual const std::string GetName() const { return m_name; }
    int GetHealth() const { return m_hp; }
    int GetInitiative() { return m_speed + m_dice.Roll(2); } // speed + 2d6
    bool IsAlive() const { return m_hp > 0; }

    void Punch(MemeFighter& other) {
        //  damage opponent by power + 2d6
        if (other.IsAlive() && IsAlive()) {
            int damage = m_power + m_dice.Roll(2);
            std::cout << GetName() << " punches " << other.GetName() << '.' << std::endl;
            other.Damage(damage);
        }
    }

    virtual void Tick() {
        // recover 1d6 hp every turn
        if (IsAlive()) {
        int amount = m_dice.Roll(1);
        std::cout << GetName() << " recovers " << amount << " HP." << std::endl;
        Heal(amount);
        }
    }

    virtual void Damage(int amount) {
        m_hp -= amount;
        std::cout << GetName() << " loses " << amount << " HP." << std::endl;
        if (!IsAlive()) {
            std::cout << GetName() << " stumbles and keels over dead." << std::endl;
        }
    }
    void Heal(int amount) { m_hp += amount; }

protected:
    MemeFighter(std::string name, int hp, int speed, int power)
        :
            m_name(name),
            m_hp(hp),
            m_speed(speed),
            m_power(power)
    {}

protected:
    Dice m_dice;
    std::string m_name = "Empty";
    int m_hp = 1;
    int m_speed = 1;
    int m_power = 1;
};

class MemeFrog : public MemeFighter
{
public:
    MemeFrog(std::string name)
        :
            MemeFighter(name, 69, 7, 14)
    {}

    void SpecialMove(MemeFighter& other) {
        // 1/3 chance to deal 3d6 + 20 damage
        if(other.IsAlive()) {
            if(m_dice.Roll(1) <= 2) {
                int damage = m_dice.Roll(3) + 20;
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
        Damage(m_dice.Roll(1));
    }
};

class MemeStoner : public MemeFighter
{
public:
    MemeStoner(std::string name)
    :
        MemeFighter(name, 80, 4, 10)
    {};

    const std::string GetName() const override{
        if (is_super) {
            return "Super " + MemeFighter::GetName();
        }
        return MemeFighter::GetName();
    }

    void SpecialMove() {
        // 1/2 chance to power up and increase health, speed, power
        // and add "Super" to their name
        if (IsAlive()) {
            if (is_super) {
                std::cout << MemeFighter::GetName() << " smokes another fat one " << std::endl;
                return;
            }
            if (m_dice.Roll(1) <= 3) {
                m_hp += 10;
                m_speed += 3;
                m_power += (int)(69 / 42);
                is_super = true;
                std::cout << MemeFighter::GetName()
                    << " smokes a fat one and becomes " << GetName() << "!" << std::endl;
            }
            else {
                std::cout << GetName() << "'s lighter isn't working." << std::endl;
            }
        }
    }

private:
    bool is_super = false;
};

void Engage( MemeFighter& f1,MemeFighter& f2 )
{
	// pointers for sorting purposes
	auto* p1 = &f1;
	auto* p2 = &f2;
	// determine attack order
	if( p1->GetInitiative() < p2->GetInitiative() )
	{
		std::swap( p1,p2 );
	}
	// execute attacks
	p1->Punch( *p2 );
	p2->Punch( *p1 );
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


	MemeFrog f1( "Dat Boi" );
	MemeStoner f2( "Good Guy Greg" );

    
	while( f1.IsAlive() && f2.IsAlive() )
	{
		// trade blows
		Engage( f1,f2 );
		// special moves
		f2.SpecialMove();
		f1.SpecialMove( f2 );
		// end of turn maintainence
		f1.Tick();
		f2.Tick();

		std::cout << "Press any key to continue...";
		getchar();
		std::cout << std::endl << std::endl;
	}

	if( f1.IsAlive() )
	{
		std::cout << f1.GetName() << " is victorious!";
	}
	else
	{
		std::cout << f2.GetName() << " is victorious!";
	}
    
    getchar();

    // Restore the original terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &original_settings);
	return 0;
}
