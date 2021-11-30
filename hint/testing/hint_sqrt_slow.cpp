#include "hint.h"
#include <algorithm>
#include <unordered_map>
#include <math.h>

const int MAX_N = 2e5;
std::unordered_map<int, int> dp[MAX_N + 1];

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

void solveMini(int aStart, int aEnd, int bStart, int bEnd, const std::vector<int>& a, const std::vector<int>& b, std::vector<int>& sol)
{
    for (int i = aStart; i <= aEnd; ++i)
    {
        for (int j = bStart; j <= bEnd; ++j)
        {
            if (i > aStart && j > bStart && a[i - 1] == b[j - 1]) dp[i][j] = 1 + dp[i - 1][j - 1];
            else if (i > 0 && j > 0) dp[i][j] = std::max(dp[i][j - 1], dp[i - 1][j]);
            else if (i > 0) dp[i][j] = dp[i - 1][j];
            else if (j > 0) dp[i][j] = dp[i][j - 1];
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

    for (int bucket = 0; bucket < numBucks; ++bucket)
    {
        int aStart = bucket * n / root;
        int aEnd = (bucket + 1) * n / root;

        int bStart = borders[bucket] * m / root;
        int bEnd = (borders[bucket + 1] + 1) * m / root;

        solveMini(aStart, aEnd, bStart, bEnd, a, b, sol);

        ++t;
    }

    int i = n, j = m;

    while (i != 0 && j != 0)
    {
        if (a[i - 1] == b[j - 1])
        {
            sol.push_back(a[i - 1]);
            --i;
            --j;
        }
        else if (dp[i - 1][j] > dp[i][j - 1]) --i;
        else --j;
    }

    std::reverse(sol.begin(), sol.end());

    return sol;
}
