#include "gcd.h"

#include <algorithm>
#include <vector>
#include <cmath>
#include <list>

const size_t NUM_PRIMES = 8;
const size_t MAX_PWR = 5;
const ull primes[NUM_PRIMES] = {2, 3, 5, 7, 11, 13, 17, 19};

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

double expInf(size_t i, size_t pwr)
{
    if (pwr == 1) return log2(primes[i]) / (primes[i] - currRems[i] - 1);
    else return log2(primes[i]) / (primes[i] - currRems[i]) / (primes[i] - 1) / pow(primes[i], pwr - 2);
}

struct Elem
{
    ull newMod;
    double expInf;
    size_t i;
    ull primePwr;
    size_t prev;
};

bool cmpNewMod(const Elem& left, const Elem& right)
{
    return left.newMod < right.newMod;
}

double expInfs[NUM_PRIMES][MAX_PWR];

bool included[NUM_PRIMES];
ull newRems[NUM_PRIMES];
ull newMods[NUM_PRIMES];

std::vector<Elem> elems; 
std::list<size_t> bestElems;

ull play(ull MAX_X)
{
    ull MAX_AB = 2 * MAX_X;

    std::fill(currRems, currRems + NUM_PRIMES, 0);
    std::fill(knownRems, knownRems + NUM_PRIMES, 0);
    std::fill(knownMods, knownMods + NUM_PRIMES, 1);

    ull knownMod = 1;

    while (knownMod < MAX_X)
    {
        for (size_t i = 0; i < NUM_PRIMES; ++i)
        {
            for (size_t pwr = 1; pwr <= MAX_PWR; ++pwr)
            {
                expInfs[i][pwr - 1] = expInf(i, pwr);
            }
        }

        elems.clear();
        bestElems.clear();
        elems.push_back({1, 0, 0, 0});
        bestElems.push_front(0);
        for (size_t i = 0; i < NUM_PRIMES; ++i)
        {
            ull primePwr = knownMods[i];
            double expInf = 0;
            size_t oldElemsSize = elems.size();

            for (size_t pwr = 1; pwr <= MAX_PWR; ++pwr)
            {
                primePwr *= primes[i];
                expInf += expInfs[i][pwr - 1];
                size_t prevElemsSize = elems.size();

                for (auto it = bestElems.begin(); it != bestElems.end(); ++it)
                {
                    Elem& elem = elems[*it];
                    if (elem.newMod * primePwr >= MAX_AB) break;
                    elems.push_back({elem.newMod * primePwr, elem.expInf + expInf, i, primePwr, *it});
                }

                std::inplace_merge(elems.begin() + oldElemsSize, elems.begin() + prevElemsSize, elems.end(), cmpNewMod);
            }

            auto it = bestElems.begin();
            for (size_t k = oldElemsSize; k < elems.size(); ++k)
            {
                Elem& elem = elems[k];
                while (it != bestElems.end() && elems[*it].newMod < elem.newMod) ++it;
                if (it != bestElems.begin() && elems[*std::prev(it)].expInf >= elem.expInf) continue;
                while (it != bestElems.end() && elems[*it].expInf <= elem.expInf) it = bestElems.erase(it);
                bestElems.insert(it, k);
            }
        }

        std::fill(included, included + NUM_PRIMES, false);
        std::fill(newRems, newRems + NUM_PRIMES, 0);
        std::fill(newMods, newMods + NUM_PRIMES, 1);

        ull newMod = 1;
        ull baseMod = 1;

        for (auto k = *std::prev(bestElems.end()); k != 0; )
        {
            Elem& elem = elems[k];
            size_t i = elem.i;
            ull primePwr = elem.primePwr;

            included[i] = true;
            newRems[i] += knownRems[i] + currRems[i] * knownMods[i];
            newMods[i] *= primePwr;
            newMod *= primePwr;
            baseMod *= knownMods[i];

            k = elem.prev;
        }

        ull newRem = crt(newRems, newMods);
        ull res = query(newMod - newRem, newMod) / baseMod;

        for (size_t i = 0; i < NUM_PRIMES; ++i)
        {
            if (!included[i]) continue;
            if (res % primes[i] != 0) ++currRems[i];
            if (res % primes[i] == 0 || currRems[i] == primes[i] - 1)
            {
                knownRems[i] += currRems[i] * knownMods[i];
                currRems[i] = 0;
                if (res % primes[i] != 0) knownMods[i] *= primes[i];
                else
                {
                    while (res % primes[i] == 0)
                    {
                        knownMods[i] *= primes[i];
                        res /= primes[i];
                    }
                    if (knownMods[i] < newMods[i])
                    {
                        ++currRems[i];
                        if (currRems[i] == primes[i] - 1)
                        {
                            knownRems[i] += currRems[i] * knownMods[i];
                            knownMods[i] *= primes[i];
                            currRems[i] = 0;
                        }
                    }
                }
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
