#include "shoes.h"

#include <numeric>
#include <iterator>
#include <algorithm>
#include <math.h>

const int MAX_N = 5e3;

double bsCosts[MAX_N * 2 + 1];

void computeBsCosts(int n)
{
    bsCosts[0] = 0;
    bsCosts[1] = 0;
    for (int i = 2; i <= 2 * n; ++i)
    {
        int mid = i / 2;
        bsCosts[i] = 1 + (mid * bsCosts[mid] + (i - mid) * bsCosts[i - mid]) / i;
    }
}

#define CERTAIN -2
#define NOCHECK -1

int findMatch(const std::vector<int>& unmatched, int x, int checkDepth)
{
    int left = -1;
    int right = unmatched.size();
    int depth = 0;

    if (checkDepth == CERTAIN) left = 0;

    while (right - left > 1)
    {
        int mid = left == -1 && depth == checkDepth ? 0 : (left + right) / 2;

        std::vector<int> temp;
        std::copy(unmatched.begin() + mid, unmatched.begin() + right, std::back_inserter(temp));
        temp.push_back(x);

        if (pairInSet(temp)) left = mid;
        else right = mid;

        ++depth;
    }

    return left;
}

std::pair<double, int> bestCostIter(int unmatched, int remaining)
{
    if (unmatched == remaining) return {bsCosts[unmatched], CERTAIN};

    double baseProb = (double) unmatched / remaining;
    double baseCost = bsCosts[unmatched + 1];

    double bestCost = baseCost;
    int bestDepth = NOCHECK;

    int curr = unmatched;
    int depth = 0;

    while (curr > 0)
    {
        double currProb = (double) curr / remaining;
        double currCost = baseProb * baseCost + currProb * (1 + bsCosts[curr] - bsCosts[curr + 1]) + (1 - baseProb) * (depth + 1);

        if (currCost < bestCost)
        {
            bestCost = currCost;
            bestDepth = depth;
        }

        curr = (curr - 1) / 2;
        ++depth;
    }

    return {bestCost, bestDepth};
}

std::vector<std::pair<int, int>> guessPairs(int n)
{
    computeBsCosts(n);

    std::vector<std::pair<int, int>> pairs;

    std::vector<int> perm(2 * n);
    std::iota(perm.begin(), perm.end(), 1);

    std::vector<int> unmatched;

    for (int i = 0; i < 2 * n; ++i)
    {
        int checkDepth = bestCostIter(unmatched.size(), 2 * n - i).second;
        int match = findMatch(unmatched, perm[i], checkDepth);

        if (match == -1) unmatched.push_back(perm[i]);
        else
        {
            pairs.push_back({unmatched[match], perm[i]});
            std::swap(unmatched[match], unmatched.back());
            unmatched.pop_back();
        }
    }

    return pairs;
}