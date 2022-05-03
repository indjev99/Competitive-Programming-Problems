#include "shoes.h"

#include <numeric>
#include <iterator>
#include <algorithm>
#include <math.h>

const int MAX_N = 5e3;

double bsCosts[MAX_N * 2 + 1];
double extraBsCosts[MAX_N * 2 + 1];
double extraBsMids[MAX_N * 2 + 1];

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

std::vector<std::pair<int, int>> guessPairs(int n)
{
    computeBsCosts(n);

    std::vector<std::pair<int, int>> pairs;

    std::vector<int> perm(2 * n);
    std::iota(perm.begin(), perm.end(), 1);

    std::vector<int> unmatched;

    for (int i = 0; i < 2 * n; ++i)
    {
        int remaining = 2 * n - i;
        int relProb = remaining - unmatched.size();
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
