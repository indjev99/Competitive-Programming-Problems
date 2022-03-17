#include "gcd.h"

ull play(ull MAX_X)
{
    ull MAX_AB = 2 * MAX_X;

    ull knownRem = 0;
    ull knownMod = 1;

    while (knownMod < MAX_X)
    {
        if (knownMod * 6 >= MAX_AB)
        {
            ull res = query(knownMod * 2 - knownRem, knownMod * 2) / knownMod;
            if (res == 1) knownRem += knownMod;
            knownMod *= 2;

            continue;
        }

        ull res = query(knownMod * 6 - knownRem, knownMod * 6) / knownMod;

        if (res % 6 == 0) knownRem += 0 * knownMod;
        else if (res % 3 == 0) knownRem += 3 * knownMod;
        else if (res % 2 == 0)
        {
            res = query(knownMod * 6 - knownRem - 2 * knownMod, knownMod * 6) / knownMod;
            if (res % 6 == 0) knownRem += 2 * knownMod;
            else knownRem += 4 * knownMod;
        }
        else
        {
            res = query(knownMod * 6 - knownRem - 1 * knownMod, knownMod * 6) / knownMod;
            if (res % 6 == 0) knownRem += 1 * knownMod;
            else knownRem += 5 * knownMod;
        }

        knownMod *= 6;
    }

    return knownRem;
}
