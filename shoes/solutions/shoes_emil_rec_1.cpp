#include "shoes.h"

#include <numeric>
#include <algorithm>

const int MAX_N = 1e4;

std::vector<std::pair<int, int>> pairs;
bool pairFound[MAX_N * 2];

void rec(const std::vector<int>& all)
{
    if (all.size() < 2 || !pairInSet(all)) return;
    if (all.size() == 2)
    {
        pairFound[all[0] - 1] = true;
        pairFound[all[1] - 1] = true;
        pairs.push_back({all[0], all[1]});
        return;
    }

    std::vector<int> next;

    for (int i = 0; i < (int) all.size(); ++i)
    {
        if (i % 3 != 2 && !pairFound[all[i] - 1]) next.push_back(all[i]);
    }
    rec(next);

    next.clear();
    for (int i = 0; i < (int) all.size(); ++i)
    {
        if (i % 3 != 1 && !pairFound[all[i] - 1]) next.push_back(all[i]);
    }
    rec(next);

    next.clear();
    for (int i = 0; i < (int) all.size(); ++i)
    {
        if (i % 3 != 0 && !pairFound[all[i] - 1]) next.push_back(all[i]);
    }
    rec(next);
}

std::vector<std::pair<int, int>> guessPairs(int n)
{
    pairs.clear();
    std::fill(pairFound, pairFound + 2 * n, false);

    std::vector<int> all(2 * n);
    std::iota(all.begin(), all.end(), 1);
    rec(all);

    return pairs;
}
