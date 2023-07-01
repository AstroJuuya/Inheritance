#pragma once

#include <algorithm>
#include "MemeFighter.h"

class Game
{
public:
    void Run();

private:
    void TakeWeaponOnDeath(MemeFighter& fighter_1, MemeFighter& fighter_2);
    void Engage( MemeFighter& f1,MemeFighter& f2 );

private:    
    static constexpr auto alive_pred = [](MemeFighter* pf) {return pf->IsAlive();};
    std::mt19937 rng = std::mt19937( std::random_device{}() );

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
};
