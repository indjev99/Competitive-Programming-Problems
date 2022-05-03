#include "shoes.h"

#include <numeric>
#include <algorithm>
#include <math.h>

const int MAX_N = 1e4;
const double threshCoeff = 3;

int n;
std::vector<std::pair<int, int>> pairs;
bool pairFound[MAX_N * 2];

bool rec(const std::vector<int>& all, bool certain)
{
    if (all.size() < 2) return false;
    int thresh = std::max<int>(threshCoeff * sqrt(n - pairs.size()), 2);
    if ((int) all.size() <= thresh && certain == false)
    {
        if (!pairInSet(all)) return false;
        certain = true;
    }
    if (all.size() == 2)
    {
        pairFound[all[0] - 1] = true;
        pairFound[all[1] - 1] = true;
        pairs.push_back({all[0], all[1]});
        return true;
    }

    bool found = false;

    std::vector<int> next;

    for (int i = 0; i < (int) all.size(); ++i)
    {
        if (i % 3 != 2 && !pairFound[all[i] - 1]) next.push_back(all[i]);
    }
    found = rec(next, false) || found;

    next.clear();
    for (int i = 0; i < (int) all.size(); ++i)
    {
        if (i % 3 != 1 && !pairFound[all[i] - 1]) next.push_back(all[i]);
    }
    found = rec(next, false) || found;

    next.clear();
    for (int i = 0; i < (int) all.size(); ++i)
    {
        if (i % 3 != 0 && !pairFound[all[i] - 1]) next.push_back(all[i]);
    }
    found = rec(next, certain && !found) || found;

    return found;
}

std::vector<std::pair<int, int>> guessPairs(int _n)
{
    n = _n;
    pairs.clear();
    std::fill(pairFound, pairFound + 2 * n, false);

    std::vector<int> all(2 * n);
    std::iota(all.begin(), all.end(), 1);
    rec(all, true);

    return pairs;
}
