#include "fortuna.h"

const int MAX_N = 30;
const int MAX_M = 30;

int n;
int m;

int encode(int curr, int val)
{
    int pwr = 0;
    int cc = 1;
    while (cc < curr)
    {
        pwr++;
        cc *= n;
    }
    return pwr * m + val;
}

std::pair<int, int> decode(int code)
{
    int pwr = code / m;
    int val = code % m;
    int curr = 1;
    while (pwr > 0)
    {
        pwr--;
        curr *= n;
    }
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

    curr = curr % m;
    val = val % m;

    while (true)
    {
        int next = 1;
        while (next * n <= curr)
        {
            next *= n;
        }

        int d2 = curr / next;
        if (val < next * d2)
        {
            curr = next;
        }
        else
        {
            curr %= next;
        }
        val %= next;

        if (curr == next) break;
    }

    return {false, encode(curr, val)};
}
