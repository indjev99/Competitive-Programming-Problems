#include "gcd.h"

ull play(ull MAX_X)
{
    ull maxMod = 1;
    while (maxMod < MAX_X)
    {
        maxMod *= 2;
    }

    ull knownRem = 0;
    ull knownMod = 1;

    while (knownMod < MAX_X)
    {
        ull res = query(maxMod - knownRem, maxMod) / knownMod;
        knownMod *= res;
        if (knownMod < maxMod)
        {
            knownRem += knownMod;
            knownMod *= 2;
        }
    }

    return knownRem;
}
