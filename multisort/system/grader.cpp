#include "multisort.h"
#include <algorithm>
#include <iostream>
#include <random>

static std::mt19937 generator;

static const int MAX_N = 20000;
static int n, k, seed;

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

static const long long HASH_BASE = 947;
static const long long HASH_MOD = 1190518999;

static long long hashWithInput(long long val)
{
    long long hsh = (val + HASH_MOD) % HASH_MOD;
    hsh = (hsh * HASH_BASE + k) % HASH_MOD;
    hsh = (hsh * HASH_BASE + seed) % HASH_MOD;
    return hsh;
}

int main()
{
    n = 20000;
    std::cin >> k >> seed;
    generator.seed(seed);

    valid = true;
    cnt = 0;
    initVals();
    std::vector<int> res = multisort(n, k);
    if (!valid)
    {
        std::cout << -4 << " " << hashWithInput(-4) << "\n";
        return 0;
    }
    if ((int) res.size() != n)
    {
        std::cout << -3 << " " << hashWithInput(-3) << "\n";
        return 0;
    }
    for (int i = 0; i < n; ++i)
    {
        if (res[i] < 0 || res[i] >= n)
        {
            std::cout << -2 << " " << hashWithInput(-2) << "\n";
            return 0;
        }
    }
    for (int i = 1; i < n; ++i)
    {
        if (val[res[i]] < val[res[i - 1]])
        {
            std::cout << -1 << " " << hashWithInput(-1) << "\n";
            return 0;
        }
    }
    std::cout << cnt << " " << hashWithInput(cnt) << "\n";
    return 0;
}
