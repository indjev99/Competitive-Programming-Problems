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
#define CHECK 0

int findMatch(const std::vector<int>& unmatched, int x, int check)
{
    int left = -1;
    int right = unmatched.size();

    if (check == CERTAIN) left = 0;
    else if (check == CHECK)
    {
        std::vector<int> temp(unmatched);
        temp.push_back(x);
        if (pairInSet(temp)) left = 0;
        else return -1;
    }

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

std::pair<double, int> bestCostIter(int unmatched, int remaining)
{
    if (unmatched == remaining) return {bsCosts[unmatched], CERTAIN};

    double prob = (double) unmatched / remaining; 
    double baseCost = bsCosts[unmatched + 1];
    double checkCost = 1 + prob * bsCosts[unmatched];

    if (baseCost < checkCost) return {baseCost, NOCHECK};
    else return {checkCost, CHECK};
}

int getChunkLen(int unmatched, int remaining)
{
    int upb = (remaining - unmatched) / 2;

    int target = 0;
    double probUnmatched = 1;

    for (target = 0; target < upb && probUnmatched > 0.6; ++target)
    {
        double currProb = (double) (unmatched + target) / (remaining - target);
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
        iterCost += probUnmatched * bestCostIter(unmatched + i, remaining - i).first;
        chunkCost += probUnmatched * currProb * bsCosts[unmatched + i];
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

    std::vector<int> unmatched;

    for (int i = 0; i < 2 * n; ++i)
    {
        int chunkLen = bestCostChunk(unmatched.size(), 2 * n - i);
        if (chunkLen != NOCHUNK)
        {
            std::pair<int, bool> nextFound = findInChunk(unmatched, perm, i, chunkLen);
            std::copy(perm.begin() + i, perm.begin() + nextFound.first + !nextFound.second, std::back_inserter(unmatched));
            i = nextFound.first;
            if (!nextFound.second) continue;
        }

        int check = chunkLen != NOCHUNK ? CERTAIN : bestCostIter(unmatched.size(), 2 * n - i).second;
        int match = findMatch(unmatched, perm[i], check);

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
