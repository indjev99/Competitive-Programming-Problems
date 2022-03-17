#include "gcd.h"

#include <algorithm>
#include <cmath>

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

ull currRems[NUM_PRIMES];
ull knownRems[NUM_PRIMES];
ull knownMods[NUM_PRIMES];

double expInf(size_t i)
{
    return log2(primes[i]) / (primes[i] - currRems[i] - 1);
}

double expInfs[NUM_PRIMES];

bool cmpExpInf(size_t left, size_t right)
{
    return expInfs[left] > expInfs[right];
}

size_t primeOrd[NUM_PRIMES];

bool included[NUM_PRIMES];
ull newRems[NUM_PRIMES];
ull newMods[NUM_PRIMES];

ull play(ull MAX_X)
{
    ull MAX_AB = 2 * MAX_X;

    for (size_t i = 0; i < NUM_PRIMES; ++i)
    {
        primeOrd[i] = i;
    }

    std::fill(currRems, currRems + NUM_PRIMES, 0);
    std::fill(knownRems, knownRems + NUM_PRIMES, 0);
    std::fill(knownMods, knownMods + NUM_PRIMES, 1);

    ull knownMod = 1;

    while (knownMod < MAX_X)
    {
        for (size_t i = 0; i < NUM_PRIMES; ++i)
        {
            expInfs[i] = expInf(i);
        }

        std::sort(primeOrd, primeOrd + NUM_PRIMES, cmpExpInf);

        std::fill(included, included + NUM_PRIMES, false);
        std::copy(knownRems, knownRems + NUM_PRIMES, newRems);
        std::copy(knownMods, knownMods + NUM_PRIMES, newMods);

        ull newMod = knownMod;

        for (size_t t = 0; t < NUM_PRIMES; ++t)
        {
            size_t i = primeOrd[t];

            if (newMod * primes[i] >= MAX_AB) continue;

            newMod *= primes[i];
            newMods[i] *= primes[i];

            included[i] = true;
            newRems[i] += currRems[i] * knownMods[i];
        }

        ull newRem = crt(newRems, newMods);
        ull res = query(newMod - newRem, newMod) / knownMod;

        for (size_t i = 0; i < NUM_PRIMES; ++i)
        {
            if (!included[i]) continue;
            if (res % primes[i] != 0) ++currRems[i];
            if (res % primes[i] == 0 || currRems[i] == primes[i] - 1)
            {
                knownRems[i] += currRems[i] * knownMods[i];
                currRems[i] = 0;
                knownMods[i] *= primes[i];
            }
        }

        knownMod = 1;
        for (size_t i = 0; i < NUM_PRIMES; ++i)
        {
            knownMod *= knownMods[i];
        }
    }

    return crt(knownRems, knownMods);
}
