#include "fortuna.h"

const int MAX_N = 30;
const int MAX_M = 30;

int n;
int m;

int encode(int curr, int val)
{
    return curr * (curr - 1) / 2 + val;
}

std::pair<int, int> decode(int code)
{
    int curr = 1;
    while (encode(curr + 1, 0) <= code)
    {
        curr++;
    }
    int val = code - encode(curr, 0);
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
        int rem = 1;
        int fin = curr;
        while (fin < m)
        {
            rem *= n;
            fin *= n;
        }

        int rejVal = (m + rem - 1) / rem;

        if (val >= rejVal)
        {
            return {false, 0};
        }

        return {false, encode(rejVal, val)};
    }

    if (val < m)
    {
        return {true, val};
    }

    return {false, 0};
}
