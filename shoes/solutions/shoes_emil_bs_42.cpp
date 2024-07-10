#include "shoes.h"

#include <numeric>
#include <iterator>
#include <algorithm>
#include <math.h>

const int MAX_N = 5e3;
const double EPS = 1e-6;

double bsCosts[MAX_N * 2 + 1];
double extraBsCosts[MAX_N * 2 + 1];
double extraBsMids[MAX_N * 2 + 1];

double probs[MAX_N + 2];
double suffProbs[MAX_N + 2];
double chunkBsCosts[MAX_N + 2][MAX_N + 2];
double chunkBsMids[MAX_N + 2][MAX_N + 2];

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

int findMatch(const std::vector<int>& unmatched, int x, int relProb)
{
    int left = -1;
    int right = unmatched.size();

    if (relProb == 0) left = 0;

    while (right - left > 1)
    {
        int mid = left == -1 ? extraBsMids[right] : (left + right) / 2;

        std::vector<int> temp;
        std::copy(unmatched.begin() + mid, unmatched.begin() + right, std::back_inserter(temp));
        temp.push_back(x);

        if (pairInSet(temp)) left = mid;
        else right = mid;
    }

    return left;
}

int findInChunk(const std::vector<int>& unmatched, const std::vector<int>& perm, int i)
{   
    int left = 0;
    int right = (perm.size() - i - unmatched.size()) / 2 + 1;

    while (right - left > 1)
    {
        int mid = chunkBsMids[left][right];

        std::vector<int> temp(unmatched);
        std::copy(perm.begin() + i, perm.begin() + i + mid, std::back_inserter(temp));

        if (pairInSet(temp)) right = mid;
        else left = mid;
    }

    return i + left;
}

void computeExtraBsCost(int i, int relProb)
{
    if (i == 0 || (relProb == 0 && i == 1))
    {
        extraBsCosts[i] = 0;
        return;
    }

    int mid = std::max((i - relProb) / 2, 0);
    double leftProb = (double) (relProb + mid) / (relProb + i);
    double rightProb = (double) (i - mid) / (relProb + i);

    computeExtraBsCost(mid, relProb);

    extraBsCosts[i] = 1 + leftProb * extraBsCosts[mid] + rightProb * bsCosts[i - mid];
    extraBsMids[i] = mid;

    double checkCost = 1 + (double) i / (relProb + i) * bsCosts[i];
    if (relProb > 0 && checkCost < extraBsCosts[i])
    {
        extraBsCosts[i] = checkCost;
        extraBsMids[i] = 0;
    }
}

void computeChunkBsCost(int from, int to)
{
    if (to - from == 1)
    {
        chunkBsCosts[from][to] = 0;
        return;
    }

    double inProb = suffProbs[from] - suffProbs[to];

    if (inProb < EPS)
    {
        int mid = (from + to) / 2;

        computeChunkBsCost(from, mid);
        computeChunkBsCost(mid, to);

        chunkBsCosts[from][to] = 1 + ((mid - from) * chunkBsCosts[from][mid] + (to - mid) * chunkBsCosts[mid][to]) / (to - from);
        chunkBsMids[from][to] = mid;

        return;
    }

    int left = from;
    int right = to;

    while (right - left > 1)
    {
        int mid = (left + right) / 2;

        if (suffProbs[mid] - suffProbs[to] > inProb / 2) left = mid;
        else right = mid;
    }

    int mid1 = left;
    double mid1Prob = suffProbs[mid1] - suffProbs[to];

    int mid2 = left + 1;
    double mid2Prob = suffProbs[mid2] - suffProbs[to];

    int mid = mid2 == to || (mid1 != from && mid1Prob - inProb / 2 < inProb / 2 - mid2Prob) ? mid1 : mid2;
    double leftProb = suffProbs[from] - suffProbs[mid];
    double rightProb = suffProbs[mid] - suffProbs[to];

    computeChunkBsCost(from, mid);
    computeChunkBsCost(mid, to);

    chunkBsCosts[from][to] = 1 + (leftProb * chunkBsCosts[from][mid] + rightProb * chunkBsCosts[mid][to]) / inProb;
    chunkBsMids[from][to] = mid;
}

bool bestCostChunkBs(int unmatched, int remaining)
{
    int upb = (remaining - unmatched) / 2;

    double iterCost = 0;
    double chunkCost = 0;
    double probUnmatched = 1;

    for (int i = 0; i <= upb && probUnmatched > EPS; ++i)
    {
        double currProb = (double) (unmatched + i) / (remaining - i);
        probs[i] = probUnmatched * currProb;
        suffProbs[i] = probUnmatched;

        computeExtraBsCost(unmatched + i, remaining - i - unmatched - i);
        iterCost += probUnmatched * extraBsCosts[unmatched + i];
        chunkCost += probs[i] * bsCosts[unmatched + i];
        probUnmatched *= 1 - currProb;
    }

    computeChunkBsCost(0, upb + 1);
    chunkCost += chunkBsCosts[0][upb + 1];

    return chunkCost < iterCost;
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
        bool chunkBs = bestCostChunkBs(unmatched.size(), 2 * n - i);
        if (chunkBs)
        {
            int next = findInChunk(unmatched, perm, i);
            std::copy(perm.begin() + i, perm.begin() + next, std::back_inserter(unmatched));
            i = next;
        }

        int relProb = chunkBs ? 0 : 2 * n - i - unmatched.size();
        computeExtraBsCost(unmatched.size(), relProb);
        int match = findMatch(unmatched, perm[i], relProb);
  
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
