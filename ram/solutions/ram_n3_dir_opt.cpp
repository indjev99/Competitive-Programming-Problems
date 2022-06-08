#include <iostream>
#include <algorithm>
#include <functional>

const int MAX_N = 1e6;

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

long long suffSum[MAX_N];
long long dp[2][MAX_N];

void solve()
{
    std::sort(r, r + n);

    suffSum[n - 1] = r[n - 1];
    for (int i = n - 2; i >= 0; --i)
    {
        suffSum[i] = suffSum[i + 1] + r[i];
    }

    bool curr = false;

    for (int i = 0; i < n; ++i)
    {
        dp[curr][i] = r[n - 1] * (n - i);
    }

    for (int t = 2; t <= k; ++t)
    {
        curr = !curr;

        for (int i = n - 1; i > n - 1 - t; --i)
        {
            dp[curr][i] = suffSum[i];
        }
    
        for (int i = n - 1 - t; i >= 0; --i)
        {
            dp[curr][i] = dp[!curr][i];

            for (int j = i + 1; j < n; ++j)
            {
                if (suffSum[j] + r[j - 1] * (j - i) >= dp[curr][i]) break;
                dp[curr][i] = std::min(dp[curr][i], dp[!curr][j] + r[j - 1] * (j - i));
            }
        }
    }

    ans = dp[curr][0];
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
