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

std::vector<int> quickSort(const std::vector<int>& is, int d);

const int MAX_PIVS = 27;

int selectPivot(const std::vector<int>& is, int d)
{
    int numPivots = is.size() - 1;
    numPivots = (numPivots - 1) / 2 * 2 + 1;
    numPivots = std::min(std::max(numPivots, 1), MAX_PIVS);

    if (numPivots == 1) return is[0];

    std::vector<int> pivots(is.begin(), is.begin() + numPivots);
    std::vector<int> pivotVals = quickSort(pivots, d);

    for (int i = 0; i < numPivots; i++)
    {
        for (int j = 0; j < numPivots; j++)
        {
            if (pivotVals[i] < pivotVals[j])
            {
                addEdge(pivots[i], pivots[j], d);
            }
        }
    }

    int pivotVal = numPivots / 2;

    for (int i = 0; i < numPivots; i++)
    {
        if (pivotVals[i] == pivotVal)
        {
            return pivots[i];
        }
    }

    return is[0];
}

std::vector<int> quickSort(const std::vector<int>& is, int d)
{
    if (is.size() == 0) return {};

    int pivot = selectPivot(is, d);

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
