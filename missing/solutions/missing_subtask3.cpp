#include <iostream>
#include <algorithm>
#include <numeric>
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

int unvisCnt;
int unvis[MAX_N];
bool isNotNext[MAX_N];

void findDists(int from)
{
    std::queue<int> q;
    std::fill(dist, dist + n, -1);

    unvisCnt = n;
    std::iota(unvis, unvis + n, 0);
    std::fill(isNotNext, isNotNext + n, false);

    dist[from] = 0;
    std::swap(unvis[from], unvis[--unvisCnt]);
    q.push(from);

    while (!q.empty())
    {
        int curr = q.front();
        q.pop();

        for (int notNext : missing[curr])
        {
            isNotNext[notNext] = true;
        }

        for (int i = 0; i < unvisCnt; ++i)
        {
            int next = unvis[i];
            if (isNotNext[next])
            {
                isNotNext[next] = false;
                continue;
            }

            dist[next] = dist[curr] + 1;
            std::swap(unvis[i], unvis[--unvisCnt]);
            q.push(next);
            --i;
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