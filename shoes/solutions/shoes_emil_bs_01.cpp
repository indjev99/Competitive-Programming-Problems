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

int findMatch(const std::vector<int>& unmatched, int x)
{
    int left = 0;
    int right = unmatched.size();

    while (right - left > 1)
    {
        int mid = (left + right) / 2;

        std::vector<int> temp;
        std::copy(unmatched.begin() + mid, unmatched.begin() + right, std::back_inserter(temp));
        temp.push_back(x);

        if (pairInSet(temp)) left = mid;
        else right = mid;
    }

    return left;
}

std::pair<int, bool> findInChunk(const std::vector<int>& unmatched, const std::vector<int>& perm, int i, int chunkLen)
{
    int left = 0;
    int right = chunkLen;

    std::vector<int> temp(unmatched);
    std::copy(perm.begin() + i, perm.begin() + i + right, std::back_inserter(temp));
    if (!pairInSet(temp)) return {i + right - 1, false};

    while (right - left > 1)
    {
        int mid = (left + right) / 2;

        std::vector<int> temp(unmatched);
        std::copy(perm.begin() + i, perm.begin() + i + mid, std::back_inserter(temp));

        if (pairInSet(temp)) right = mid;
        else left = mid;
    }

    return {i + left, true};
}

int getChunkLen(int unmatched, int left)
{
    int upb = (left - unmatched) / 2;

    int target = 0;
    double probUnmatched = 1;

    for (target = 0; target < upb && probUnmatched > 0.6; ++target)
    {
        double currProb = (double) (unmatched + target) / (left - target);
        probUnmatched *= 1 - currProb;
    }

    return std::min(std::max(target, 2), upb);
}

#define NOCHUNK 0

int bestCostChunk(int unmatched, int remaining)
{
    int chunkLen = getChunkLen(unmatched, remaining);

    if (chunkLen <= 1) return NOCHUNK;

    double iterCost = 0;
    double chunkCost = 0;
    double probUnmatched = 1;

    for (int i = 0; i < chunkLen; ++i)
    {
        double currProb = (double) (unmatched + i) / (remaining - i);
        iterCost += probUnmatched * 1;
        probUnmatched *= 1 - currProb;
    }

    chunkCost += 1 + (1 - probUnmatched) * bsCosts[chunkLen];

    return chunkCost < iterCost ? chunkLen : NOCHUNK;
}

std::vector<std::pair<int, int>> guessPairs(int n)
{
    computeBsCosts(n);

    std::vector<std::pair<int, int>> pairs;

    std::vector<int> perm(2 * n);
    std::iota(perm.begin(), perm.end(), 1);

    std::vector<int> left;
    std::vector<int> right;

    for (int i = 0; i < 2 * n; ++i)
    {
        int chunkLen = bestCostChunk(left.size(), 2 * n - i);
        if (chunkLen != NOCHUNK)
        {
            std::pair<int, bool> nextFound = findInChunk(left, perm, i, chunkLen);
            std::copy(perm.begin() + i, perm.begin() + nextFound.first + !nextFound.second, std::back_inserter(left));
            if (nextFound.second) right.push_back(perm[nextFound.first]);
            i = nextFound.first;
        }
        else
        {
            left.push_back(perm[i]);
            if (pairInSet(left))
            {
                left.pop_back();
                right.push_back(perm[i]);
            }
        }
    }

    for (int i = 0; i < n; ++i)
    {
        int match = findMatch(left, right[i]);

        pairs.push_back({left[match], right[i]});
        std::swap(left[match], left.back());
        left.pop_back();
    }

    return pairs;
}
