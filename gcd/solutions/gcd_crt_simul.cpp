#include "gcd.h"

#include <algorithm>

const size_t NUM_PRIMES = 12;
const ull primes[NUM_PRIMES] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};

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

ull crt(ull rems[], ull mods[])
{
    ull mod = 1;
    for (size_t i = 0; i < NUM_PRIMES; ++i)
    {
        mod *= mods[i];
    }

    ull result = 0;
    for (size_t i = 0; i < NUM_PRIMES; ++i)
    {
        if (mods[i] == 1) continue;

        ull others = mod / mods[i];
        result += rems[i] * others % mod * modInv(others, mods[i]) % mod;
        result %= mod;
    }

    return result;
}

bool foundRems[NUM_PRIMES];
ull rems[NUM_PRIMES];
ull mods[NUM_PRIMES];

ull play(ull MAX_X)
{
    ull MAX_AB = 2 * MAX_X;

    std::fill(foundRems, foundRems + NUM_PRIMES, false);
    std::fill(mods, mods + NUM_PRIMES, 1);

    ull mod = 1;

    while (mod < MAX_X)
    {
        size_t bestI = 0;
        ull bestPrimePwr = 0;
        for (size_t i = 0; i < NUM_PRIMES; ++i)
        {
            ull newPrimePwr = mods[i] * primes[i];
            if (i == 0 || newPrimePwr < bestPrimePwr)
            {
                bestI = i;
                bestPrimePwr = newPrimePwr;
            }
        }

        mods[bestI] *= primes[bestI];
        mod *= primes[bestI];
    }

    while (mod >= MAX_AB)
    {
        for (size_t i = 0; i < NUM_PRIMES; ++i)
        {
            if (mods[i] != 1 && mod / primes[i] >= MAX_X)
            {
                mods[i] /= primes[i];
                mod /= primes[i];
                break;
            }
        }
    }

    size_t numFound = 0;
    for (ull rem = 0; numFound < NUM_PRIMES; ++rem)
    {
        ull res = query(mod - rem, mod);
        for (size_t i = 0; i < NUM_PRIMES; ++i)
        {
            if (!foundRems[i] && res % mods[i] == 0)
            {
                foundRems[i] = true;
                ++numFound;
                rems[i] = rem;
            }
        }
    }

    return crt(rems, mods);
}

