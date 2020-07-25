#include <iostream>
#include <vector>
#include <math.h>

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

std::vector<int> heavyEdges[MAX_N];

int dpSelf[MAX_N];
int dpMaxAdj[MAX_N];

void solve()
{
    int treshold = sqrt(m);
    for (int i = 0; i < n; ++i)
    {
        for (int other : edges[i])
        {
            if (edges[other].size() > treshold) heavyEdges[i].push_back(other);
        }
    }

    ans = 0;
    for (int i = 0; i < t; ++i)
    {
        int curr = flights[i].first;
        int next = flights[i].second;
        int maxFound = dpSelf[curr];
        if (edges[curr].size() <= treshold)
        {
            for (int other : edges[curr])
            {
                maxFound = std::max(maxFound, dpSelf[other]);
            }
        }
        else maxFound = std::max(maxFound, dpMaxAdj[curr]);
        int nextLen = maxFound + 1;
        ans = std::max(ans, nextLen);
        if (nextLen > dpSelf[next])
        {
            dpSelf[next] = nextLen;
            for (int other : heavyEdges[next])
            {
                dpMaxAdj[other] = std::max(dpMaxAdj[other], nextLen);
            }
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
