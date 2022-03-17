#include "gcd.h"

ull play(ull MAX_X)
{
    ull MAX_AB = 2 * MAX_X;

    ull maxMod6 = 1;
    while (maxMod6 < MAX_X && maxMod6 * 6 < MAX_AB)
    {
        maxMod6 *= 6;
    }

    ull maxMod2 = maxMod6;
    while (maxMod2 < MAX_X)
    {
        maxMod2 *= 2;
    }

    ull knownRem = 0;
    ull knownMod = 1;

    ull nextRes = 0;

    while (knownMod < MAX_X)
    {
        if (knownMod >= maxMod6)
        {
            ull res = query(maxMod2 - knownRem, maxMod2) / knownMod;
            knownMod *= res;
            if (knownMod < maxMod2)
            {
                knownRem += knownMod;
                knownMod *= 2;
            }

            continue;
        }

        ull res = nextRes == 0 ? query(maxMod6 - knownRem, maxMod6) / knownMod : nextRes;

        if (res % 6 == 0) knownRem += 0 * knownMod;
        else if (res % 3 == 0) knownRem += 3 * knownMod;
        else if (res % 2 == 0)
        {
            res = query(maxMod6 - knownRem - 2 * knownMod, maxMod6) / knownMod;
            if (res % 6 == 0) knownRem += 2 * knownMod;
            else knownRem += 4 * knownMod;
        }
        else
        {
            res = query(maxMod6 - knownRem - 1 * knownMod, maxMod6) / knownMod;
            if (res % 6 == 0) knownRem += 1 * knownMod;
            else knownRem += 5 * knownMod;
        }
        
        if (res % 6 == 0)
        {
            while (res % 6 == 0)
            {
                knownMod *= 6;
                res /= 6;
            }
            nextRes = res;
        }
        else
        {
            knownMod *= 6;
            nextRes = 0;
        }
    }

    return knownRem;
}
