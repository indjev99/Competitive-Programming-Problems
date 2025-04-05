#include "pointsort.h"
#include <algorithm>
#include <numeric>
#include <optional>
#include <array>

std::vector<std::vector<std::vector<bool>>> cacheLt;
std::vector<std::vector<std::array<std::vector<int>, 2>>> cachedEdges;

std::optional<bool> checkOne(int i, int j, int d)
{
    auto& cache = cacheLt[d];
    auto& cacheE = cachedEdges[d];

    if (cache[i][j]) return true;
    if (cache[j][i]) return false;

    if (cacheE[j][0].size() < cacheE[i][1].size())
    {
        for (int s : cacheE[j][0])
        {
            if (cache[i][s]) return true;
        }
    }
    else
    {
        for (int s : cacheE[i][1])
        {
            if (cache[s][j]) return true;
        }
    }

    if (cacheE[i][0].size() < cacheE[j][1].size())
    {
        for (int s : cacheE[i][0])
        {
            if (cache[j][s]) return false;
        }
    }
    else
    {
        for (int s : cacheE[j][1])
        {
            if (cache[s][i]) return false;
        }
    }

    return std::nullopt;
}

void addEdge(int i, int j, int d)
{
    auto& cache = cacheLt[d];
    auto& cacheE = cachedEdges[d];

    if (cache[i][j]) return;

    cache[i][j] = true;
    cacheE[i][1].push_back(j);
    cacheE[j][0].push_back(i);
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

template <class It, class Cmp>
void mergeBS(It s1, It e1, It s2, It e2, It out, Cmp cmp)
{
    int sz1 = e1 - s1;
    int sz2 = e2 - s2;

    if (sz1 > sz2)
    {
        std::swap(s1, s2);
        std::swap(e1, e2);
        std::swap(sz1, sz2);
    }

    if (sz1 == 0)
    {
        while (s2 != e2)
        {
            *out = *s2;
            out++;
            s2++;
        }

        return;
    }

    int mi1 = sz1 / 2;

    It m1 = s1 + mi1;

    It m2 = std::lower_bound(s2, e2, *m1, cmp);

    int mi2 = m2 - s2;

    int mi = mi1 + mi2;

    *(out + mi) = *m1;

    merge(s1, m1, s2, m2, out, cmp);
    merge(m1 + 1, e1, m2, e2, out + mi + 1, cmp);
}

std::vector<int> mergeSort(const std::vector<int>& is, int d)
{
    if (is.size() == 0) return {};
    if (is.size() == 1) return {0};

    std::vector<int> left(is.begin(), is.begin() + is.size() / 2);
    std::vector<int> right(is.begin() + is.size() / 2, is.end());

    std::vector<int> leftVals = mergeSort(left, d);
    std::vector<int> rightVals = mergeSort(right, d);

    std::vector<int> leftOrd(left.size());
    std::vector<int> rightOrd(right.size());

    using Loc = std::pair<bool, int>;

    std::vector<Loc> leftLocs(left.size());
    std::vector<Loc> rightLocs(right.size());

    for (int i = 0; i < (int) left.size(); i++)
    {
        leftOrd[leftVals[i]] = i;
        leftLocs[i] = {true, i};
    }

    for (int i = 0; i < (int) right.size(); i++)
    {
        rightOrd[rightVals[i]] = i;
        rightLocs[i] = {false, i};
    }

    std::vector<Loc> mergedLocs(is.size());

    mergeBS(
        leftLocs.begin(), leftLocs.end(),
        rightLocs.begin(), rightLocs.end(),
        mergedLocs.begin(),
        [d, &left, &right, &leftOrd, &rightOrd](Loc a, Loc b)
        {
            auto& aPart = a.first ? left : right;
            auto& bPart = b.first ? left : right;

            auto& aOrd = a.first ? leftOrd : rightOrd;
            auto& bOrd = b.first ? leftOrd : rightOrd;
    
            int aI = aPart[aOrd[a.second]];
            int bI = bPart[bOrd[b.second]];

            return compareOne(aI, bI, d);
        }
    );

    std::vector<int> vals(is.size());

    for (int p = 0; p < (int) is.size(); p++)
    {
        Loc loc = mergedLocs[p];
        int i = loc.first ? leftOrd[loc.second] : rightOrd[loc.second] + left.size();
        vals[i] = p;
    }

    return vals;
}

std::vector<std::vector<int>> pointSort(int n, int k)
{
    cacheLt.clear();
    cachedEdges.clear();
    cacheLt.resize(k, std::vector<std::vector<bool>>(n, std::vector<bool>(n, false)));
    cachedEdges.resize(k, std::vector<std::array<std::vector<int>, 2>>(n));

    std::vector<int> is(n);
    std::iota(is.begin(), is.end(), 0);

    std::vector<std::vector<int>> points(n);

    for (int d = 0; d < k; d++)
    {
        std::vector<int> vals = mergeSort(is, d);

        for (int i = 0; i < n; i++)
        {
            points[is[i]].push_back(vals[i]);
        }
    }

    return points;
}
