#pragma once

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
