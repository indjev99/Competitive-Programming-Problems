#include "mummy.h"

#include <algorithm>
#include <iostream>
#include <string>

static int n;
static std::vector<int> realPerm;
static std::vector<bool> taken;
static int queries;

static void myAssert(bool cond, const char* msg)
{
    if (cond) return;

    std::cout << msg << std::endl;

    exit(0);
}

int numMatches(const std::vector<int>& perm)
{
    queries++;

    myAssert((int) perm.size() == n, "Invalid permutation");

    std::fill(taken.begin(), taken.end(), false);
    for (int i = 0; i < n; i++)
    {
        myAssert(perm[i] >= 0 && perm[i] < n, "Invalid permutation");
        myAssert(!taken[perm[i]], "Invalid permutation");

        taken[perm[i]] = true;
    }

    int matches = 0;
    for (int i = 0; i < n; i++)
    {
        if (realPerm[i] == perm[i])
        {
            matches++;
        }
    }
    return matches;
}

static void oneTest()
{
    realPerm.resize(n);
    for (int i = 0; i < n; i++)
    {
        std::cin >> realPerm[i];
    }

    taken.resize(n);

    std::vector<int> perm = findPerm(n);

    myAssert(perm == realPerm, "Wrong answer");
}

int main()
{
    int t;

    std::cin >> n;
    std::cin >> t;

    for (int i = 0; i < t; i++)
    {
        oneTest();
    }

    std::cout << "Average queries: " << (double) queries / t << std::endl;

    return 0;
}
