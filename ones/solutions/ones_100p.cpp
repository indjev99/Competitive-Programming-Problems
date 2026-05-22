#include "ones.h"
#include <random>
#include <algorithm>
#include <iostream>
#include <assert.h>

std::default_random_engine generator;
std::uniform_int_distribution<int> distr(0, 1);

bool randBit()
{
    return distr(generator);
}

int n;

std::vector<bool> mask;
std::vector<bool> lastMask;
std::vector<bool> currMask;

void randMask()
{
    for (int i = 0; i < n; ++i)
    {
        mask[i] = randBit();
    }
}

std::vector<bool> askMask;

int applyMask()
{
    for (int i = 0; i < n; ++i)
    {
        askMask[i] = lastMask[i] ^ currMask[i];
    }
    lastMask = currMask;
    return flip_bits(askMask);
}

std::pair<int, int> find_longest_subarray_of_ones(int _n)
{
    n = _n;
    mask.resize(n);
    askMask.resize(n);
    currMask.resize(n);
    lastMask.resize(n);
    std::fill(lastMask.begin(), lastMask.end(), false);
    int maxBits = -1;
    int minBits = -1;
    int trials = 0;
    while (true)
    {
        ++trials;
        randMask();
        int newMaxBits = flip_bits(mask);
        if (maxBits != -1 && newMaxBits != maxBits)
        {
            minBits = newMaxBits;
            if (minBits > maxBits)
            {
                std::swap(minBits, maxBits);
                lastMask = mask;
            }
            break;
        }
        else maxBits = newMaxBits;
    }

    int l = 0;
    int r = n + 1 - maxBits;

    while (r - l > 1)
    {
        int m = (l + r) / 2;
        for (int i = 0; i < n; ++i)
        {
            currMask[i] = i >= m ? mask[i] : false;
        }
        int max1 = applyMask();
        if (m > 0) currMask[m - 1] = !currMask[m - 1];
        int max2 = applyMask();
        int max = std::min(max1, max2);
        assert(max <= maxBits);
        if (max < maxBits) r = m;
        else l = m;
    }

    currMask = mask;
    applyMask();

    return {l, l + maxBits - 1};
}