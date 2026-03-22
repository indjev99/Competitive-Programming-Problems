#include "fortuna.h"

const int MAX_N = 30;
const int MAX_M = 30;

int n;
int m;

int encode(int curr, int val)
{
    return (curr - 1) * m + val;
}

std::pair<int, int> decode(int code)
{
    int curr = code / m + 1;
    int val = code % m;
    return {curr, val};
}

void setup(int _n, int _m)
{
    n = _n;
    m = _m;
}

std::pair<bool, int> proc(int x, int r)
{
    auto [curr, val] = decode(x);

    curr = curr * n;
    val = val * n + r;

    if (curr < m)
    {
        return {false, encode(curr, val)};
    }

    int d = curr / m;
    int max = m * d;

    if (val < max)
    {
        return {true, val % m};
    }

    return {false, encode(curr % m, val % m)};
}
