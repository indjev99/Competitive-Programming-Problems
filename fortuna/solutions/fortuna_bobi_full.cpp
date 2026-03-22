#include "fortuna.h"
#include <iostream>
#include <numeric>
#include <vector>

int n, m;
void setup(int N, int M)
{
    n = N;
    m = M;
}

int sumTO(int val)
{
    return val * (val + 1) / 2;
}

std::pair <bool, int> proc(int x, int r)
{
    int gen = 1;
    while (sumTO(gen) <= x)
    {
        gen++;
    }

    int val = x - sumTO(gen - 1);
    val = r * gen + val;
    gen *= n;
    if (gen >= m)
    {
        if (val + (gen % m) < gen)
        {
            return {true, val % m};
        }

        val -= gen - (gen % m);
        gen %= m;
    }

    return {false, sumTO(gen - 1) + val};
}
