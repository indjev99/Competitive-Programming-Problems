#include <iostream>
#include <vector>
#include <queue>

const int INF = 1e9;

const int MAX_N = 1e5;
const int MAX_M = 1e6;

int n, m, s, t;
std::vector<std::pair<int, int>> adj[MAX_N];

void input()
{
    int a, b;
    std::cin >> n >> m >> s >> t;
    for (int i = 0; i < m; ++i)
    {
        std::cin >> a >> b;
        adj[a].emplace_back(b, i);
        adj[b].emplace_back(a, i);
    }
}

int ans, ansPathLen;
int ansPath[MAX_N];

void output()
{
    if (ans == INF)
    {
        std::cout << -1 << "\n";
        return;
    }

    std::cout << ans << " " << ansPathLen << "\n";
    for (int i = 0; i <= ansPathLen; ++i)
    {
        if (i > 0) std::cout << " ";
        std::cout << ansPath[i];
    }
    std::cout << std::endl;
}

int dist[MAX_N];
int parEdge[MAX_N];

void distBfs()
{
    std::queue<int> q;
    std::fill(dist, dist + n, INF);

    dist[t] = 0;
    q.push(t);

    while (!q.empty())
    {
        int curr = q.front();
        q.pop();

        for (auto [other, edge] : adj[curr])
        {
            if (dist[other] == INF)
            {
                dist[other] = dist[curr] + 1;
                parEdge[other] = edge;
                q.push(other);
            }
        }
    }
}

int blockedDist[MAX_N];

void blockedBfs(int blocked, int node)
{
    std::queue<int> q;
    std::fill(blockedDist, blockedDist + n, INF);

    for (int i = 0; i < n; ++i)
    {
        if (dist[i] > dist[node] || i == node) continue;
        blockedDist[i] = dist[i];
        if (dist[i] == dist[node]) q.push(i);
    }
    blockedDist[t] = 0;

    while (!q.empty() && blockedDist[node] == INF)
    {
        int curr = q.front();
        q.pop();

        for (auto [other, edge] : adj[curr])
        {
            if (blockedDist[other] == INF && edge != blocked)
            {
                blockedDist[other] = blockedDist[curr] + 1;
                q.push(other);
            }
        }
    }
}

int maxBlockedDist[MAX_N];

void findMaxBlockedDists()
{
    for (int i = 0; i < n; ++i)
    {
        if (i == t) continue;

        maxBlockedDist[i] = INF;

        for (auto [other, edge] : adj[i])
        {
            if (edge == parEdge[i] || other == i) continue;
            if (dist[other] <= dist[i])
            {
                maxBlockedDist[i] = std::min(maxBlockedDist[i], dist[other] + 1);
            }
        }

        if (maxBlockedDist[i] == INF)
        {
            blockedBfs(parEdge[i], i);
            maxBlockedDist[i] = blockedDist[i];
        }
    }
}

int ansDist[MAX_N];
int ansPar[MAX_N];

std::vector<int> pq[MAX_N * 2];

void ansDijkstra()
{
    std::fill(ansDist, ansDist + n, INF);

    ansDist[t] = 0;
    pq[0].push_back(t);

    for (int d = 0; d < 2 * n; ++d)
    {
        for (int curr : pq[d])
        {
            if (ansDist[curr] < d) continue;
            for (auto [other, edge] : adj[curr])
            {
                int newDist = std::max(maxBlockedDist[other], d + 1);
                if (ansDist[other] > newDist)
                {
                    ansDist[other] = newDist;
                    ansPar[other] = curr;
                    pq[newDist].push_back(other);
                }
            }
        }
    }
}

void buildSol()
{
    ans = ansDist[s];

    if (ans == INF) return;

    int curr = s;
    while (curr != t)
    {
        ansPath[ansPathLen++] = curr;
        curr = ansPar[curr];
    }
    ansPath[ansPathLen] = curr;
}

void solve()
{
    distBfs();
    findMaxBlockedDists();
    ansDijkstra();
    buildSol();
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
