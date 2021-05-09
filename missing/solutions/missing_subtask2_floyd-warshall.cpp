#include <iostream>
#include <algorithm>
#include <vector>

const int MAX_N = 1e4;
const int MAX_Q = 1e6;

struct Query
{
    int idx;
    int from, to;
    int ans;
};

int n, q;
std::vector<int> missing[MAX_N];
Query qs[MAX_Q];

void input()
{
    int m, from, to;
    std::cin >> n >> m;
    for (int i = 0; i < m; ++i)
    {
        std::cin >> from >> to;
        missing[from].push_back(to);
        missing[to].push_back(from);
    }
    std::cin >> q;
    for (int i = 0; i < q; ++i)
    {
        qs[i].idx = i;
        std::cin >> qs[i].from >> qs[i].to;
    }
}

void output()
{
    for (int i = 0; i < q; ++i)
    {
        std::cout << qs[i].ans << "\n";
    }
}

const int SMALL_N = 5e3;

int dist[SMALL_N][SMALL_N];

void findDists()
{
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0, t = 0; j < n; ++j)
        {
            if (i == j) dist[i][j] = 0;
            else if (t < missing[i].size() && missing[i][t] == j)
            {
                ++t;
                dist[i][j] = n;
            }
            else dist[i][j] = 1;
        }
    }

    for (int k = 0; k < n; ++k)
    {
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                if (dist[i][k] + dist[k][j] < dist[i][j])
                {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            if (dist[i][j] == n) dist[i][j] = -1;
        }
    }
}

void solve()
{
    for (int i = 0; i < n; ++i)
    {
        std::sort(missing[i].begin(), missing[i].end());
    }

    findDists();

    for (int i = 0; i < q; ++i)
    {
        qs[i].ans = dist[qs[i].from][qs[i].to];
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
