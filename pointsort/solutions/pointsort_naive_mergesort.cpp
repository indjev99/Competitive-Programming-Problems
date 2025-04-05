#include "pointsort.h"
#include <algorithm>
#include <numeric>

bool compareOne(int i, int j, int d)
{
    return compare(i, j)[d];
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

    std::merge(
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
