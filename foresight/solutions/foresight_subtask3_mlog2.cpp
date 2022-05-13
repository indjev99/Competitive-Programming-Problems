#include <iostream>
#include <memory>
#include <vector>
#include <queue>
#include <set>

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
std::vector<int> children[MAX_N];

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
                children[curr].push_back(other);
                parEdge[other] = edge;
                q.push(other);
            }
        }
    }
}

void addDepthEdge(std::set<std::pair<int, int>>& set, int depth, int edge)
{
    std::pair<int, int> depthEdge = {depth, edge};
    auto [it, inserted] = set.insert(depthEdge);
    if (!inserted) set.erase(it);
}

int maxBlockedDist[MAX_N];

std::unique_ptr<std::set<std::pair<int, int>>> maxBlockedDistDfs(int curr)
{
    std::vector<std::unique_ptr<std::set<std::pair<int, int>>>> childrenSets;
    int maxChild = 0;

    for (int other : children[curr])
    {
        childrenSets.push_back(maxBlockedDistDfs(other));
        if (childrenSets.back()->size() > childrenSets[maxChild]->size())
        {
            maxChild = childrenSets.size() - 1;
        }
    }

    if (curr == t)
    {
        maxBlockedDist[curr] = 0;
        return nullptr;
    }

    if (childrenSets.empty())
    {
        childrenSets.push_back(std::make_unique<std::set<std::pair<int, int>>>());
    }
    else std::swap(childrenSets[0], childrenSets[maxChild]);

    for (int i = 1; i < (int) childrenSets.size(); ++i)
    {
        for (auto [depth, edge] : *childrenSets[i])
        {
            addDepthEdge(*childrenSets[0], depth, edge);
        }
    }

    for (auto [other, edge] : adj[curr])
    {
        if (edge == parEdge[curr] || edge == parEdge[other]) continue;

        int depth = dist[curr] + dist[other];
        addDepthEdge(*childrenSets[0], depth, edge);
    }

    if (childrenSets[0]->empty()) maxBlockedDist[curr] = INF;
    else maxBlockedDist[curr] = childrenSets[0]->begin()->first - dist[curr] + 1;

    return std::move(childrenSets[0]);
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
    maxBlockedDistDfs(t);
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
