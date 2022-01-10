#include <iostream>
#include <limits.h>

const int MAX_N = 1e6;
const long long NEG_INF = INT_MIN;

int n;
long long storageCost;
long long prices[MAX_N];

long long ans;

void input()
{
    std::cin >> n >> storageCost;
    for (int i = 0; i < n; ++i)
    {
        std::cin >> prices[i]; 
    }
}

void output()
{
    std::cout << ans << std::endl;
}

long long dp[2][MAX_N + 1];

void solve()
{
    bool curr = false;
    for (int i = 1; i <= n; ++i)
    {
        int maxPosition = std::min(i, n - i);
        for (int j = 0; j <= maxPosition; ++j)
        {
            dp[!curr][j] = NEG_INF;
            int minPrevPosition = std::max(j - 1, 0);
            int maxPrevPosition = std::min(j + 1, i - 1);
            for (int k = minPrevPosition; k <= maxPrevPosition; ++k)
            {
                dp[!curr][j] = std::max(dp[!curr][j], dp[curr][k] + (k - j) * prices[i - 1] - k * storageCost);
            }
        }
        curr = !curr;
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