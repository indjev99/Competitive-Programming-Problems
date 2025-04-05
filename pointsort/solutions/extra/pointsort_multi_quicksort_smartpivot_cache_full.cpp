#include "pointsort.h"
#include <algorithm>
#include <numeric>
#include <optional>
#include <array>
#include <cmath>

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

std::vector<bool> compareFull(int i, int j, int k)
{
    std::vector<bool> res(k);

    bool cached = true;

    for (int d = 0; d < k; d++)
    {
        std::optional<bool> chk = checkOne(i, j, d);
        if (!chk.has_value())
        {
            cached = false;
            break;
        }

        res[d] = chk.value();
    }

    if (cached) return res;

    return compareAndCache(i, j);
}

int makeMask(const std::vector<bool>& cmp)
{
    int mask = 0;
    for (int d = 0; d < (int) cmp.size(); d++)
    {
        mask |= cmp[d] << d;
    }
    return mask;
}

std::vector<std::vector<int>> quickSort(const std::vector<int>& is, int k);

const std::vector<int> MAX_PIVS_PER_K = {
    1,
    27,
    17,
    11,
    7
};

int selectPivot(const std::vector<int>& is, int k)
{
    int maxPivots =  k < (int) MAX_PIVS_PER_K.size() ? MAX_PIVS_PER_K[k] : MAX_PIVS_PER_K.back();

    int numPivots = is.size() - 1;
    numPivots = (numPivots - 1) / 2 * 2 + 1;
    numPivots = std::min(std::max(numPivots, 1), maxPivots);

    if (numPivots == 1) return is[0];

    std::vector<int> pivots(is.begin(), is.begin() + numPivots);
    std::vector<std::vector<int>> pivotVals = quickSort(pivots, k);

    for (int i = 0; i < numPivots; i++)
    {
        for (int j = 0; j < numPivots; j++)
        {
            for (int d = 0; d < k; d++)
            {
                if (pivotVals[i][d] < pivotVals[j][d])
                {
                    addEdge(pivots[i], pivots[j], d);
                }
            }
        }
    }

    int pivot = -1;
    double bestScore = 0;

    for (int i = 0; i < numPivots; i++)
    {
        double score = 0;
        for (int d = 0; d < k; d++)
        {
            int left = pivotVals[i][d] + 1;
            int right = numPivots - pivotVals[i][d];
            int total = left + right;

            double lp = (double) left / total;
            double rp = (double) right / total;

            score -= lp * log(lp) + rp * log(rp);
        }

        if (score > bestScore)
        {
            bestScore = score;
            pivot = pivots[i];
        }
    }

    return pivot;
}

std::vector<std::vector<int>> quickSort(const std::vector<int>& is, int k)
{
    if (is.size() == 0) return {};

    int pivot = selectPivot(is, k);

    int numQuads = 1 << k;

    std::vector<std::vector<int>> parts(numQuads);

    using Loc = std::pair<int, int>;

    std::vector<Loc> newLocs(is.size());

    for (int i = 0; i < (int) is.size(); i++)
    {
        if (is[i] == pivot) continue;

        int mask = makeMask(compareFull(pivot, is[i], k));

        newLocs[i] = {mask, parts[mask].size()};

        parts[mask].push_back(is[i]);
    }

    std::vector<int> lowCnts(k, 0);
    for (int m = 0; m < numQuads; m++)
    {
        for (int d = 0; d < k; d++)
        {
            if ((m >> d) & 1) continue;
            lowCnts[d] += parts[m].size();
        }
    }

    std::vector<std::vector<std::vector<int>>> partVals(numQuads);

    for (int m = 0; m < numQuads; m++)
    {
        partVals[m] = quickSort(parts[m], k);
    }

    auto mergeVals = [numQuads, &parts, &partVals](const std::vector<int>& quads, int d, int startVal)
    {
        std::vector<std::vector<int>> partOrds(numQuads);
        std::vector<std::vector<Loc>> toMerge;

        for (int m : quads)
        {
            if (parts[m].empty()) continue;

            partOrds[m].resize(parts[m].size());
            for (int i = 0; i < (int) parts[m].size(); i++)
            {
                partOrds[m][partVals[m][i][d]] = i;
            }

            toMerge.push_back(std::vector<Loc>(parts[m].size()));
            for (int i = 0; i < (int) parts[m].size(); i++)
            {
                toMerge.back()[i] = {m, i};
            }
        }

        if (toMerge.empty()) return;

        while (toMerge.size() > 1)
        {
            int minI = -1;
            int minI2 = -1;
            for (int i = 0; i < (int) toMerge.size(); i++)
            {
                if (minI == -1 || toMerge[i].size() < toMerge[minI].size())
                {
                    minI2 = minI;
                    minI = i;
                }
                else if (minI2 == -1 || toMerge[i].size() < toMerge[minI2].size())
                {
                    minI2 = i;
                }
            }

            std::vector<Loc> left = std::move(toMerge[minI]);
            std::vector<Loc> right = std::move(toMerge[minI2]);

            toMerge.erase(toMerge.begin() + std::max(minI, minI2));
            toMerge.erase(toMerge.begin() + std::min(minI, minI2));

            std::vector<Loc> merged(left.size() + right.size());

            std::merge(
                left.begin(), left.end(),
                right.begin(), right.end(),
                merged.begin(),
                [d, &parts, &partOrds](Loc a, Loc b)
                {
                    int aI = parts[a.first][partOrds[a.first][a.second]];
                    int bI = parts[b.first][partOrds[b.first][b.second]];

                    return compareOne(aI, bI, d);
                }
            );

            toMerge.push_back(std::move(merged));
        }

        for (Loc loc : toMerge.front())
        {
            partVals[loc.first][partOrds[loc.first][loc.second]][d] = startVal++;
        }
    };

    for (int d = 0; d < k; d++)
    {
        for (bool hi : {false, true})
        {
            std::vector<int> quads;
            for (int m = 0; m < numQuads; m++)
            {
                if (((m >> d) & 1) != hi) continue;
                quads.push_back(m);
            }

            mergeVals(quads, d, hi ? lowCnts[d] + 1 : 0);
        }
    }

    std::vector<std::vector<int>> vals(is.size());

    for (int i = 0; i < (int) is.size(); i++)
    {
        if (is[i] == pivot)
        {
            vals[i] = lowCnts;
            continue;
        }

        auto [m, p] = newLocs[i];
        vals[i] = partVals[m][p];
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

    std::vector<std::vector<int>> vals = quickSort(is, k);

    for (int i = 0; i < n; i++)
    {
        points[is[i]] = vals[i];
    }

    return points;
}
