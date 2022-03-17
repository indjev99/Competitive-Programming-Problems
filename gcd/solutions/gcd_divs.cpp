#include "gcd.h"

#include <vector>
#include <cstddef>

ull currNum = 1;
std::vector<ull> primes;
std::vector<ull> primePwrs;

void computePrimePwr(ull MAX_AB)
{
    bool isPrime;
    do
    {
        ++currNum;
        isPrime = true;
        for (ull prime : primes)
        {
            if (currNum % prime == 0)
            {
                isPrime = false;
                break;
            }
            if (prime * prime > currNum) break;
        }
    }
    while (!isPrime);
    primes.push_back(currNum);

    ull numPwr = 1;
    while (numPwr * currNum < MAX_AB) numPwr *= currNum;
    primePwrs.push_back(numPwr);
}

ull play(ull MAX_X)
{
    ull MAX_AB = MAX_X * 2;

    ull x = 1;
    for (std::size_t i = 0; ; ++i)
    {
        if (i == primes.size()) computePrimePwr(MAX_AB);
        bool shouldStop = x * primes[i] >= MAX_X;
        x *= query(0, primePwrs[i]);
        if (x >= MAX_X || shouldStop) break;
    }
    return x < MAX_X ? x : 0;
}
