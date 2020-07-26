#include "multisort.h"
#include <algorithm>
#include <iostream>
#include <random>

static std::mt19937 generator;

static const int MAX_N = 20000;
static int n, k;

static int val[MAX_N];

static bool valid;
static long long cnt;

static void initVals()
{
    for (int i = 0; i < n; ++i)
    {
        val[i] = i;
    }
    std::shuffle(val, val + n, generator);
}

static bool cmpByValue(int a, int b)
{
    return val[a] < val[b];
}

void compare(std::vector<int>& elems)
{
    if ((int) elems.size() > k)
    {
        valid = false;
        return;
    }
    ++cnt;
    std::sort(elems.begin(), elems.end(), cmpByValue);
}

static void oneTest(bool printCorrect=true)
{
    valid = true;
    cnt = 0;
    initVals();
    std::vector<int> res = multisort(n, k);
    if (!valid)
    {
        std::cout << "Used invalid comparisons." << std::endl;
        return;
    }
    if ((int) res.size() != n)
    {
        std::cout << "Returned too many or too few numbers." << std::endl;
        return;
    }
    for (int i = 0; i < n; ++i)
    {
        if (res[i] < 0 || res[i] >= n)
        {
            std::cout << "Out of range." << std::endl;
            return;
        }
    }
    for (int i = 1; i < n; ++i)
    {
        if (val[res[i]] < val[res[i - 1]])
        {
            std::cout << "Not sorted." << std::endl;
            return;
        }
    }
    if (printCorrect) std::cout << "Correctly sorted with " << cnt << " comparisons." << std::endl;
}

long long findMax(int iters=10)
{
    long long worst = 0;
    for (int i = 0; i < iters; ++i)
    {
        oneTest(false);
        worst = std::max(worst, cnt);
    }
    return worst;
}

long long findAvg(int iters=10)
{
    long long total = 0;
    for (int i = 0; i < iters; ++i)
    {
        oneTest(false);
        total += cnt;
    }
    return (total + iters / 2) / iters;
}

int main()
{
    n = 20000;
    int seed;
    std::cin >> k >> seed;
    generator.seed(seed);
    oneTest();
    return 0;
}
