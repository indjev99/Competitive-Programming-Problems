#include <iostream>
#include <unordered_set>

const int MAX_N = 5e5;
const int MAX_T = 5e5;

int n, m, t;
std::unordered_set<int> edges[MAX_N];
std::pair<int, int> flights[MAX_T];
int ans;

void input()
{
    int num, a;
    std::cin >> n >> m >> t;
    for (int i = 0; i < n; ++i)
    {
        std::cin >> num;
        edges[i].reserve(num);
        for (int j = 0; j < num; ++j)
        {
            std::cin >> a;
            edges[i].insert(a);
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

int dp[MAX_T];

void solve()
{
    ans = 0;
    for (int i = 0; i < t; ++i)
    {
        int next = flights[i].second;
        int nextLen = dp[i] + 1;
        ans = std::max(ans, nextLen);
        for (int j = i + 1; j < t; ++j)
        {
            int other = flights[j].first;
            if (nextLen > dp[j] && (next == other || edges[next].find(other) != edges[next].end()))
                dp[j] = nextLen;
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
