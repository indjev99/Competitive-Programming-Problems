#include "hint.h"
#include <algorithm>

const int MAX_N = 1e4;

int dp[MAX_N + 1][MAX_N + 1];

std::vector<bool> genHint(const std::vector<int>& a, const std::vector<int>& b, const std::vector<int>& sol)
{
    return {};
}

std::vector<int> solve(const std::vector<int>& a, const std::vector<int>& b, const std::vector<bool>& hint)
{
    std::vector<int> sol;

    int n = a.size();
    int m = b.size();

    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= m; ++j)
        {
            if (a[i - 1] == b[j - 1]) dp[i][j] = 1 + dp[i - 1][j - 1];
            else dp[i][j] = std::max(dp[i][j - 1], dp[i - 1][j]);
        }
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
