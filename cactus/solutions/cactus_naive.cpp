#include "cactus.h"

#include <cassert>

int minEdges(int n)
{
    return n - 1;
}

int maxEdges(int n)
{
    return 3 * (n - 1) / 2;
}

void encodeNumber(std::vector<bool>& data, int num, int lb, int ub, bool binary = true)
{
    assert(lb < ub);
    assert(num >= lb);
    assert(num < ub);

    ub -= lb;
    num -= lb;

    if (binary)
    {
        int numBits = 0;
        while ((1 << numBits) < ub) numBits++;

        int maxVal = ub - 1;
        bool sent0 = false;

        for (int b = numBits - 1; b >= 0; b--)
        {
            if (!sent0 && !((maxVal >> b) & 1)) continue;
            bool currB = (num >> b) & 1;
            if (!currB) sent0 = true;
            data.push_back(currB);
        }
    }
    else
    {
        for (int i = 0; i < num; i++)
        {
            data.push_back(1);
        }
        if (num < ub - 1)
        {
            data.push_back(0);
        }
    }
}

int decodeNumber(const std::vector<bool>& data, int& pos, int lb, int ub, bool binary = true)
{
    assert(lb < ub);

    ub -= lb;

    int num = 0;
    if (binary)
    {
        int numBits = 0;
        while ((1 << numBits) < ub) numBits++;

        int maxVal = ub - 1;
        bool got0 = false;

        for (int b = numBits - 1; b >= 0; b--)
        {
            if (!got0 && !((maxVal >> b) & 1)) continue;
            bool currB = data.at(pos++);
            if (!currB) got0 = true;
            num |= currB << b;
        }
    }
    else
    {
        while (num < ub - 1 && data.at(pos++))
        {
            num++;
        }
    }

    num += lb;

    return num;
}

std::vector<bool> encode(int n, std::vector<std::pair<int, int>> edges)
{
    std::vector<bool> data;

    int m = edges.size();

    encodeNumber(data, m, minEdges(n), maxEdges(n) + 1);

    for (auto [u, v] : edges)
    {
        encodeNumber(data, u, 0, n);
        encodeNumber(data, v, 0, n);
    }

    return data;
}

std::vector<std::pair<int, int>> decode(int n, std::vector<bool> data)
{
    int pos = 0;

    std::vector<std::pair<int, int>> edges;

    int m = decodeNumber(data, pos, minEdges(n), maxEdges(n) + 1);

    edges.resize(m);

    for (auto& [u, v] : edges)
    {
        u = decodeNumber(data, pos, 0, n);
        v = decodeNumber(data, pos, 0, n);
    }

    return edges;
}
