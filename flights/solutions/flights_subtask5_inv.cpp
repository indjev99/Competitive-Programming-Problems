#include <iostream>
#include <vector>

const int MAX_N = 5e5;
const int MAX_T = 5e5;

int n, m, t;
std::vector<int> edges[MAX_N];
std::pair<int, int> flights[MAX_T];
int ans;

void input()
{
    int num;
    std::cin >> n >> m >> t;
    for (int i = 0; i < n; ++i)
    {
        std::cin >> num;
        edges[i].resize(num);
        for (int j = 0; j < num; ++j)
        {
            std::cin >> edges[i][j];
        }
    }
    for (int i = 0; i < t; ++i)
    {
        std::cin >> flights[i].first >> flights[i].second;
    }
}

void output()
{
    std::cout << ans << std::endl;
}

int dp[MAX_N];

void solve()
{
    ans = 0;
    for (int i = 0; i < t; ++i)
    {
        int curr = flights[i].first;
        int next = flights[i].second;
        int nextLen = dp[curr] + 1;
        ans = std::max(ans, nextLen);
        dp[next] = std::max(dp[next], nextLen);
        for (int other : edges[next])
        {
            dp[other] = std::max(dp[other], nextLen);
        }
    }
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
