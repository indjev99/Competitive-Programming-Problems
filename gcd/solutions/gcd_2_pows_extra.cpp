#include "gcd.h"

ull play(ull MAX_X)
{
    ull maxMod = 27 * 25 * 7 * 11 * 13;
    if (maxMod >= MAX_X / 2) maxMod = 1;
    while (maxMod < MAX_X)
    {
        maxMod *= 2;
    }

    ull knownRem = 0;
    ull knownMod = 1;

    while (knownMod < MAX_X)
    {
        if (maxMod / knownMod % 2 != 0)
        {
            maxMod = knownMod;
            while (maxMod < MAX_X)
            {
                maxMod *= 2;
            }
        }

        ull res = query(maxMod - knownRem, maxMod) / knownMod;
        knownMod *= res;
        if (maxMod / knownMod % 2 == 0)
        {
            knownRem += knownMod;
            knownMod *= 2;
        }
    }

    return knownRem;
}
