#include <iostream>
#include <algorithm>
#include <functional>

const int MAX_N = 1e4;

int n, k;
long long r[MAX_N];

long long ans;

void input()
{
    std::cin >> n >> k;
    for (int i = 0; i < n; ++i)
    {
        std::cin >> r[i];
    }
}

void output()
{
    std::cout << ans << "\n";
}

long long dp[MAX_N + 1][MAX_N];

void solve()
{
    std::sort(r, r + n, std::greater<long long>());

    for (int i = 0; i < n; ++i)
    {
        dp[1][i] = r[0] * (i + 1);
    }

    for (int t = 2; t <= k; ++t)
    {
        for (int i = 0; i < n; ++i)
        {
            dp[t][i] = dp[t - 1][i];

            for (int j = 0; j < i; ++j)
            {
                dp[t][i] = std::min(dp[t][i], dp[t - 1][j] + r[j + 1] * (i - j));
            }
        }
    }

    ans = dp[k][n - 1];
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    input();
    solve();
    output();

    return 0;
}
