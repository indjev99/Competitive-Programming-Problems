#include <iostream>
#include <vector>
#include <queue>

struct Edge
{
    int to;
    long long w;
};

int n, m;
std::vector<std::vector<Edge>> es;

bool ans;

void input()
{
    std::cin >> n >> m;
    es.resize(n);

    for (int i = 0; i < m; i++)
    {
        int from, to, w;
        std::cin >> from >> to >> w;
        es[from].push_back({to, w});
    }
}

void output()
{
    std::cout << ans << std::endl;
}

void solve()
{
    ans = false;

    std::vector<bool> inQ(n, true);
    std::vector<long long> dist(n, 0);
    std::vector<int> visCnt(n, 0);
    std::queue<int> q;

    for (int i = 0; i < n; i++)
    {
        q.push(i);
    }

    while (!q.empty())
    {
        int curr = q.front();
        q.pop();
        inQ[curr] = false;

        visCnt[curr]++;
        if (visCnt[curr] == n + 1)
        {
            ans = true;
            return;
        }

        long long d = dist[curr];

        for (auto [to, w] : es[curr])
        {
            if (d + w < dist[to])
            {
                dist[to] = d + w;
                if (!inQ[to])
                {
                    q.push(to);
                    inQ[to] = true;
                }
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
