#include "gcd.h"

ull play(ull MAX_X)
{
    ull knownRem = 0;
    ull knownMod = 1;

    while (knownMod < MAX_X)
    {
        ull res = query(knownMod * 2 - knownRem, knownMod * 2) / knownMod;
        if (res == 1) knownRem += knownMod;
        knownMod *= 2;
    }

    return knownRem;
}
