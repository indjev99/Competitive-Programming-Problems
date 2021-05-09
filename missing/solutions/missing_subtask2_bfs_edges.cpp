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
std::vector<int> edges[MAX_N];

void findDists(int from)
{
    std::queue<int> q;
    std::fill(dist, dist + n, -1);

    dist[from] = 0;
    q.push(from);

    while (!q.empty())
    {
        int curr = q.front();
        q.pop();

        for (int next : edges[curr])
        {
            if (dist[next] == -1)
            {
                dist[next] = dist[curr] + 1;
                q.push(next);
            }
        }
    }
}

bool cmpSrc(const Query& a, const Query& b)
{
    return a.from < b.from;
}

bool cmpIdx(const Query& a, const Query& b)
{
    return a.idx < b.idx;
}

void solve()
{
    for (int i = 0; i < n; ++i)
    {
        std::sort(missing[i].begin(), missing[i].end());

        for (int next = 0, j = 0; next < n; ++next)
        {
            if (j < missing[i].size() && missing[i][j] == next)
            {
                ++j;
                continue;
            }

            edges[i].push_back(next);
        }
    }

    std::sort(qs, qs + q, cmpSrc);

    for (int i = 0; i < q; ++i)
    {
        if (i == 0 || qs[i].from != qs[i - 1].from) findDists(qs[i].from);
        qs[i].ans = dist[qs[i].to];
    }

    std::sort(qs, qs + q, cmpIdx);
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
