#include "gcd.h"

ull modInv(ull a, ull mod)
{
    ull b = mod;
    ull x = 1, y = 0;

    while (b != 0)
    {
        ull q = a / b;

        ull temp = b;
        b = a - q * b;
        a = temp;

        temp = y;
        y = (mod + x - q * y % mod) % mod;
        x = temp;
    }

    return x;
}

ull crt(ull rem2, ull mod2, ull rem3, ull mod3)
{
    ull mod = mod2 * mod3;

    ull result = rem2 * mod3 % mod * modInv(mod3, mod2) % mod;
    result += rem3 * mod2 % mod * modInv(mod2, mod3) % mod;
    result %= mod;

    return result;
}

ull play(ull MAX_X)
{
    ull MAX_AB = 2 * MAX_X;

    ull knownRem2 = 0;
    ull knownMod2 = 1;

    ull currRem3 = 0;
    ull knownRem3 = 0;
    ull knownMod3 = 1;

    ull knownMod = 1;

    while (knownMod < MAX_X)
    {
        if (knownMod * 6 >= MAX_AB)
        {
            ull newMod2 = knownMod2 * 2;
            ull newMod3 = knownMod3;
            ull newMod = newMod2 * newMod3;

            ull newRem2 = knownRem2;
            ull newRem3 = knownRem3;
            ull newRem = crt(newRem2, newMod2, newRem3, newMod3);

            ull res = query(newMod - newRem, newMod) / knownMod;

            if (res % 2 != 0) knownRem2 += knownMod2;
            knownMod2 *= 2;

            knownMod = knownMod2 * knownMod3;

            continue;
        }

        ull newMod2 = knownMod2 * 2;
        ull newMod3 = knownMod3 * 3;
        ull newMod = newMod2 * newMod3;

        ull newRem2 = knownRem2;
        ull newRem3 = knownRem3 + currRem3 * knownMod3;
        ull newRem = crt(newRem2, newMod2, newRem3, newMod3);

        ull res = query(newMod - newRem, newMod) / knownMod;

        if (res % 2 != 0) knownRem2 += knownMod2;
        knownMod2 *= 2;

        if (res % 3 != 0) ++currRem3;
        if (res % 3 == 0 || currRem3 == 2)
        {
            knownRem3 += currRem3 * knownMod3;
            currRem3 = 0;
            knownMod3 *= 3;
        }

        knownMod = knownMod2 * knownMod3;
    }

    return crt(knownRem2, knownMod2, knownRem3, knownMod3);
}
