#include <iostream>
#include <algorithm>
#include <numeric>
#include <unordered_set>

#include "lcm.h"

const int MAX_N = 1e5;

std::vector<int> bigPrimes;

void init(int n)
{
    static bool done = false;
    if (done) return;
    done = true;

    std::vector<bool> isPrime(n + 1, true);

    for (int i = 2; i <= n; ++i)
    {
        if (!isPrime[i]) continue;
        if (i > n / 2) bigPrimes.push_back(i);
        for (int j = i + i; j <= n; j += i)
        {
            isPrime[j] = false;
        }
    }
}

long long getLcm(int i, int j)
{
    return lcm(i + 1, j + 1);
}

struct Pair
{
    int i, j;
    long long lcm;

    Pair();

    Pair(int i, int j, long long lcm)
        : i(i)
        , j(j)
        , lcm(lcm)
    {}
};

long long calcLcm(int x, int y)
{
    return (long long) x * y / std::__gcd(x, y);
}

std::vector<int> guessPermutation(int n)
{
    init(n);

    std::vector<Pair> pairs;
    std::vector<int> perm(n, -1);

    int key = -1;
    int bigPrime = -1;

    int i = 0;
    for (i = 0; ; i += 2)
    {
        int j = i + 1;
        long long lcm = getLcm(i, j);
        pairs.emplace_back(i, j, lcm);

        for (int prime : bigPrimes)
        {
            if (lcm % prime == 0)
            {
                bigPrime = prime;
                break;
            }
        }

        if (bigPrime != -1) break;
    }

    int other = pairs.back().lcm / bigPrime;
    pairs.pop_back();

    int checkPos = i + 2 < n ? i + 2 : i - 2;
    long long checkLcm = getLcm(i, checkPos);

    if (checkLcm % bigPrime == 0)
    {
        key = i;
        perm[i] = bigPrime;
        perm[i + 1] = other;
        perm[checkPos] = checkLcm / bigPrime;
    }
    else
    {
        key = i + 1;
        perm[i] = other;
        perm[i + 1] = bigPrime;
        if (checkPos == i + 2) pairs.emplace_back(i, checkPos, checkLcm);
    }

    i = std::max(i + 2, checkPos + 1);

    for (; i < n; ++i)
    {
        long long lcm = getLcm(key, i);
        perm[i] = lcm / bigPrime;
    }

    for (Pair& p : pairs)
    {
        if (perm[p.j] != -1) std::swap(p.i, p.j);

        if (perm[p.i] == -1)
        {
            long long lcm = getLcm(key, p.i);
            perm[p.i] = lcm / bigPrime;
        }
    }

    std::vector<bool> taken(n + 1, false);

    for (int num : perm)
    {
        if (num != -1)
        taken[num] = true;
    }

    std::vector<int> poss;

    for (int j = 1; j <= n; ++j)
    {
        if (!taken[j]) poss.push_back(j);
    }

    bool allKnown = false;
    bool progress = true;

    while (!allKnown)
    {
        bool forceCheck = !progress;
        allKnown = true;
        progress = false;
        for (Pair& p : pairs)
        {
            if (perm[p.j] != -1) continue;

            int cand = -1;
            for (int num : poss)
            {
                if (taken[num]) continue;
                if (calcLcm(perm[p.i], num) == p.lcm)
                {
                    if (cand == -1) cand = num;
                    else
                    {
                        cand = -2;
                        break;
                    }
                }
            }

            if (cand != -2)
            {
                perm[p.j] = cand;
                taken[perm[p.j]] = true;
                progress = true;
            }
            else if (!progress && forceCheck)
            {
                long long lcm = getLcm(key, p.j);
                perm[p.j] = lcm / bigPrime;
                taken[perm[p.j]] = true;
                progress = true;
            }
            else allKnown = false;
        }
    }

    return perm;
}
