#include "Game.h"

void Game::Run()
{
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
}

void Game::TakeWeaponOnDeath(MemeFighter& fighter_1, MemeFighter& fighter_2) {
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

void Game::Engage( MemeFighter& f1,MemeFighter& f2 )
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
