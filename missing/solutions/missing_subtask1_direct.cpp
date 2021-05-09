#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

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

int dist[MAX_N];

int findDist(int from, int to)
{
    std::queue<int> q;
    std::fill(dist, dist + n, -1);

    dist[from] = 0;
    q.push(from);

    while (!q.empty())
    {
        int curr = q.front();
        q.pop();

        for (int next = 0, j = 0; next < n; ++next)
        {
            if (j < missing[curr].size() && missing[curr][j] == next)
            {
                ++j;
                continue;
            }

            if (dist[next] == -1)
            {
                dist[next] = dist[curr] + 1;
                q.push(next);
            }
        }
    }

    return dist[to];
}

void solve()
{
    for (int i = 0; i < n; ++i)
    {
        std::sort(missing[i].begin(), missing[i].end());
    }

    for (int i = 0; i < q; ++i)
    {
        qs[i].ans = findDist(qs[i].from, qs[i].to);
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
