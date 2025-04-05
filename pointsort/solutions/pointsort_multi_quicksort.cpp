#include "pointsort.h"
#include <algorithm>
#include <numeric>

bool compareOne(int i, int j, int d)
{
    return compare(i, j)[d];
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

std::vector<std::vector<int>> quickSort(const std::vector<int>& is, int k)
{
    if (is.size() == 0) return {};

    int pivot = is[0];

    int numQuads = 1 << k;

    std::vector<std::vector<int>> parts(numQuads);

    using Loc = std::pair<int, int>;

    std::vector<Loc> newLocs(is.size());

    for (int i = 0; i < (int) is.size(); i++)
    {
        if (is[i] == pivot) continue;

        int mask = makeMask(compare(pivot, is[i]));

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
