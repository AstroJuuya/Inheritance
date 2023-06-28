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
