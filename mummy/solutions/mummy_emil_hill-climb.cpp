#include "mummy.h"

#include <algorithm>
#include <numeric>
#include <random>

std::mt19937 rng;

int randNum(int lb, int ub)
{
    std::uniform_int_distribution<int> distr(lb, ub - 1);
    return distr(rng);
}

int n;
std::vector<int> perm;

std::vector<int> findPerm(int _n)
{
    rng.seed(0);

    n = _n;
    perm.resize(n);
    std::iota(perm.begin(), perm.end(), 0);
    std::shuffle(perm.begin(), perm.end(), rng);

    int m = numMatches(perm);
    while (m < n)
    {
        int i = randNum(0, n);
        int j = randNum(0, n);
        if (i == j) continue;

        std::swap(perm[i], perm[j]);

        int m2 = numMatches(perm);

        if (m2 > m)
        {
            m = m2;
            continue;
        }

        std::swap(perm[i], perm[j]);
    }

    return perm;
}
