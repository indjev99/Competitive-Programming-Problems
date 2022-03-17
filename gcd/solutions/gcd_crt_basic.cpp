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

ull rems[NUM_PRIMES];
ull mods[NUM_PRIMES];

ull play(ull MAX_X)
{
    std::fill(rems, rems + NUM_PRIMES, 0);
    std::fill(mods, mods + NUM_PRIMES, 1);

    ull mod = 1;

    for (size_t i = 0; i < NUM_PRIMES && mod < MAX_X; ++i)
    {
        mods[i] = primes[i];
        for (ull rem = 0; rem < primes[i]; ++rem)
        {
            ull res = query(primes[i] - rem, primes[i]);
            if (res == primes[i])
            {
                rems[i] = rem;
                break;
            }
        }
        mod *= primes[i];
    }

    return crt(rems, mods);
}

