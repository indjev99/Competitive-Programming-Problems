#include <iostream>
#include <set>

const int MAX_N = 5e5;
const int MAX_T = 5e5;

int n, m, t;
std::set<int> edges[MAX_N];
std::pair<int, int> flights[MAX_T];
int ans;

void input()
{
    int num, a;
    std::cin >> n >> m >> t;
    for (int i = 0; i < n; ++i)
    {
        std::cin >> num;
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
        int curr = flights[i].first;
        int maxFound = 0;
        for (int j = 0; j < i; ++j)
        {
            int other = flights[j].second;
            if (dp[j] > maxFound && (curr == other || edges[curr].find(other) != edges[curr].end()))
                maxFound = dp[j];
        }
        int nextLen = maxFound + 1;
        ans = std::max(ans, nextLen);
        dp[i] = nextLen;
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
