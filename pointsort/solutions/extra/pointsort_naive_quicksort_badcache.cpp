#include "pointsort.h"
#include <algorithm>
#include <numeric>
#include <optional>
#include <array>

std::vector<std::vector<std::vector<bool>>> cacheLt;

std::optional<bool> checkOne(int i, int j, int d)
{
    auto& cache = cacheLt[d];

    if (cache[i][j]) return true;
    if (cache[j][i]) return false;

    return std::nullopt;
}

void addEdge(int i, int j, int d)
{
    auto& cache = cacheLt[d];
    cache[i][j] = true;
}

std::vector<bool> compareAndCache(int i, int j)
{
    std::vector<bool> res = compare(i, j);

    for (int d = 0; d < (int) res.size(); d++)
    {
        if (res[d]) addEdge(i, j, d);
        else addEdge(j, i, d);
    }

    return res;
}

bool compareOne(int i, int j, int d)
{
    std::optional<bool> chk = checkOne(i, j, d);
    if (chk.has_value()) return chk.value();

    return compareAndCache(i, j)[d];
}

std::vector<int> quickSort(const std::vector<int>& is, int d)
{
    if (is.size() == 0) return {};

    int pivot = is[0];

    std::vector<int> left;
    std::vector<int> right;

    using Loc = std::pair<bool, int>;

    std::vector<Loc> newLocs(is.size());

    for (int i = 0; i < (int) is.size(); i++)
    {
        if (is[i] == pivot) continue;

        if (compareOne(is[i], pivot, d))
        {
            newLocs[i] = {true, left.size()};
            left.push_back(is[i]);
        }
        else
        {
            newLocs[i] = {false, right.size()};
            right.push_back(is[i]);
        }
    }

    std::vector<int> leftVals = quickSort(left, d);
    std::vector<int> rightVals = quickSort(right, d);

    std::vector<int> vals(is.size());

    for (int p = 0; p < (int) is.size(); p++)
    {
        if (is[p] == pivot)
        {
            vals[p] = left.size();
            continue;
        }

        Loc loc = newLocs[p];

        vals[p] = loc.first ? leftVals[loc.second] : rightVals[loc.second] + left.size() + 1;
    }

    return vals;
}

std::vector<std::vector<int>> pointSort(int n, int k)
{
    cacheLt.clear();
    cacheLt.resize(k, std::vector<std::vector<bool>>(n, std::vector<bool>(n, false)));

    std::vector<int> is(n);
    std::iota(is.begin(), is.end(), 0);

    std::vector<std::vector<int>> points(n);

    for (int d = 0; d < k; d++)
    {
        std::vector<int> vals = quickSort(is, d);

        for (int i = 0; i < n; i++)
        {
            points[is[i]].push_back(vals[i]);
        }
    }

    return points;
}
