#include "hint.h"
#include <algorithm>
#include <math.h>

const int MAX_N = 2e5;
const int MAX_SQRT = 450;
int dp[MAX_SQRT + 1][MAX_N * 2 + MAX_SQRT * 2];
int dpStarts[MAX_SQRT + 1];
int dpBuckets[MAX_N * 2 + MAX_SQRT * 2];

std::vector<bool> genHint(const std::vector<int>& a, const std::vector<int>& b, const std::vector<int>& sol)
{
    std::vector<bool> hint;

    int n = a.size();
    int m = b.size();
    int k = sol.size();

    int i = 0, j = 0;
    std::vector<int> aPos, bPos;

    for (int num : sol)
    {
        while (a[i] != num) ++i;
        while (b[j] != num) ++j;

        aPos.push_back(i);
        bPos.push_back(j);

        ++i;
        ++j;
    }

    int aBuck = 0;
    int bBuck = 0;
    int root = sqrt(n);

    for (int i = 0; i < k; ++i)
    {
        while (aPos[i] >= (aBuck + 1) * n / root)
        {
            ++aBuck;
            hint.push_back(false);
        }

        while (bPos[i] >= (bBuck + 1) * m / root)
        {
            ++bBuck;
            hint.push_back(true);
        }
    }

    return hint;
}

void solveMini(int j0, int aStart, int aEnd, int bStart, int bEnd, const std::vector<int>& a, const std::vector<int>& b, std::vector<int>& sol)
{
    int n = aEnd - aStart;
    int m = bEnd - bStart;

    for (int i = 0; i <= n; ++i)
    {
        for (int j = 0; j <= m; ++j)
        {
            if (i > 0 && j > 0 && a[aStart + i - 1] == b[bStart + j - 1]) dp[i][j0 + j] = 1 + dp[i - 1][j0 + j - 1];
            else if (i > 0 && j > 0) dp[i][j0 + j] = std::max(dp[i][j0 + j - 1], dp[i - 1][j0 + j]);
            else if (i > 0) dp[i][j0 + j] = std::max(dp[i][j0 + j], dp[i - 1][j0 + j]);
            else if (j > 0) dp[i][j0 + j] = std::max(dp[i][j0 + j], dp[i][j0 + j - 1]);
        }
    }
}

std::vector<int> solve(const std::vector<int>& a, const std::vector<int>& b, const std::vector<bool>& hint)
{
    std::vector<int> sol;

    int n = a.size();
    int m = b.size();

    int aBuck = 0;
    int bBuck = 0;
    std::vector<int> borders = {0};

    for (bool bit : hint)
    {
        if (bit) ++bBuck;
        else
        {
            borders.push_back(bBuck);
            ++aBuck;
        }
    }
    borders.push_back(bBuck);

    int numBucks = borders.size() - 1;

    int t = 0;
    int root = sqrt(n);

    dpStarts[0] = 0;

    for (int bucket = 0; bucket < numBucks; ++bucket)
    {
        int aStart = bucket * n / root;
        int aEnd = (bucket + 1) * n / root;

        int bStart = borders[bucket] * m / root;
        int bEnd = (borders[bucket + 1] + 1) * m / root;
        int bLastStart = borders[bucket + 1] * m / root;

        for (int i = 0; i <= bEnd - bStart; ++i)
        {
            dpBuckets[dpStarts[bucket] + i] = bucket;
        }

        solveMini(dpStarts[bucket], aStart, aEnd, bStart, bEnd, a, b, sol);

        int lastBuckSize = bEnd - bLastStart;
        dpStarts[bucket + 1] = dpStarts[bucket] + bEnd - bStart + 1;
        for (int i = 0; i <= lastBuckSize; ++i)
        {
            dp[0][dpStarts[bucket + 1] + i] = dp[aEnd - aStart][dpStarts[bucket + 1] - 1 - lastBuckSize + i];
        }

        ++t;
    }

    int i = numBucks * n / root - (numBucks - 1) * n / root;
    int j = dpStarts[numBucks] - 1;

    while (j > 0)
    {
        int bucket = dpBuckets[j];
        int aStart = bucket * n / root;
        int bStart = borders[bucket] * m / root;
        int bPrevEnd = (borders[bucket] + 1) * m / root;
        int j0 = dpStarts[bucket];

        if (i > 0 && a[aStart + i - 1] == b[bStart + j - j0 - 1])
        {
            sol.push_back(a[aStart + i - 1]);
            --i;
            --j;

        }
        else if (i > 0 && dp[i - 1][j] > dp[i][j - 1]) --i;
        else --j;

        while (j > 0 && (j == j0 || (i == 0 && j - bPrevEnd + bStart - 1 < j0)))
        {

            if (j == j0) i = 0;

            if (i == 0 && j - bPrevEnd + bStart - 1 < j0)
            {
                int aPrevStart = (bucket - 1) * n / root;

                i = aStart - aPrevStart;
                j -= bPrevEnd - bStart + 1;
            }

            bucket = dpBuckets[j];
            aStart = bucket * n / root;
            bStart = borders[bucket] * m / root;
            bPrevEnd = (borders[bucket] + 1) * m / root;
            j0 = dpStarts[bucket];
        }
    }

    std::reverse(sol.begin(), sol.end());

    i = numBucks * n / root - (numBucks - 1) * n / root;
    j = dpStarts[numBucks] - 1;

    return sol;
}
