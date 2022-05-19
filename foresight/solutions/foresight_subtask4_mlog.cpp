#include <iostream>
#include <algorithm>
#include <numeric>
#include <vector>
#include <queue>

const int INF = 1e9;

const int MAX_N = 1e5;
const int MAX_M = 1e6;

const int MAX_N_POW2 = 1 << 17;
const int MAX_M_POW2 = 1 << 20;

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

int edgeArrPos;
int edgeArrEnterPos[MAX_N];
int edgeArrExitPos[MAX_N];
int edgeArr[2 * MAX_M];
std::pair<int, int> edgeArrIdxs[MAX_M];

int edgeTreeNode[2 * MAX_M];
int edgeTree[MAX_M_POW2 * 4];

void initTreeUtil(int curr, int cl, int cr)
{
    if (cl == cr) return;
    if (cl + 1 == cr)
    {
        edgeTreeNode[cl] = curr;
        edgeTree[curr] = edgeArr[cl];
        return;
    }

    initTreeUtil(curr * 2 + 1, cl, (cl + cr) / 2);
    initTreeUtil(curr * 2 + 2, (cl + cr) / 2, cr);
    edgeTree[curr] = std::min(edgeTree[curr * 2 + 1], edgeTree[curr * 2 + 2]);
}

int queryTreeUtil(int curr, int cl, int cr, int l, int r)
{
    if (r <= cl || cr <= l) return INF;
    if (l <= cl && cr <= r) return edgeTree[curr];

    return std::min(
        queryTreeUtil(curr * 2 + 1, cl, (cl + cr) / 2, l, r),
        queryTreeUtil(curr * 2 + 2, (cl + cr) / 2, cr, l, r)
    );
}

void initTree()
{
    initTreeUtil(0, 0, 2 * m);
}

void updateTree(int pos, int val)
{
    int curr = edgeTreeNode[pos];
    edgeTree[curr] = val;

    while (curr != 0)
    {
        curr = (curr - 1) / 2;
        edgeTree[curr] = std::min(edgeTree[curr * 2 + 1], edgeTree[curr * 2 + 2]);
    }
}

int queryTree(int l, int r)
{
    return queryTreeUtil(0, 0, 2 * m, l, r);
}

bool buildDfsVis[MAX_N];
int dsuPar[MAX_N];
int dsuRank[MAX_N];

int dsuFind(int curr)
{
    if (dsuPar[curr] != curr)
    {
        dsuPar[curr] = dsuFind(dsuPar[curr]);
    }
    return dsuPar[curr];
}

int dsuUnite(int curr, int other)
{
    other = dsuFind(other);
    curr = dsuFind(curr);

    if (other == curr) return curr;
    else if (dsuRank[curr] < dsuRank[other])
    {
        dsuPar[curr] = other;
    }
    else if (dsuRank[other] < dsuRank[curr])
    {
        dsuPar[other] = curr;
    }
    else
    {
        dsuPar[curr] = other;
        ++dsuRank[other];
    }

    return dsuPar[curr];
}

int ancestor[MAX_N];
std::vector<int> edgesToRem[MAX_N];

void buildDfs(int curr)
{
    ancestor[curr] = curr;
    edgeArrEnterPos[curr] = edgeArrPos;

    for (int other : children[curr])
    {
        buildDfs(other);
        int root = dsuUnite(curr, other);
        ancestor[root] = curr;
    }

    buildDfsVis[curr] = true;

    for (auto [other, edge] : adj[curr])
    {
        if (edge == parEdge[curr] || edge == parEdge[other]) continue;

        int depth = dist[curr] + dist[other];
        std::pair<int, int>& idx = edgeArrIdxs[edge];
        if (idx.first == -1) idx.first = edgeArrPos;
        else idx.second = edgeArrPos;
        edgeArr[edgeArrPos++] = depth;

        if (buildDfsVis[other])
        {
            int lca = ancestor[dsuFind(other)];
            edgesToRem[lca].push_back(edge);
        }
    }

    edgeArrExitPos[curr] = edgeArrPos;
}

int maxBlockedDist[MAX_N];

void maxBlockedDistDfs(int curr)
{
    for (int other : children[curr])
    {
        maxBlockedDistDfs(other);
    }

    if (curr == t)
    {
        maxBlockedDist[curr] = 0;
        return;
    }

    for (auto edge : edgesToRem[curr])
    {
        updateTree(edgeArrIdxs[edge].first, INF);
        updateTree(edgeArrIdxs[edge].second, INF);
    }

    int minEdgeDepth = queryTree(edgeArrEnterPos[curr], edgeArrExitPos[curr]);
    if (minEdgeDepth == INF) maxBlockedDist[curr] = INF;
    else maxBlockedDist[curr] = minEdgeDepth - dist[curr] + 1;
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
    std::iota(dsuPar, dsuPar + n, 0);
    std::fill(edgeArrIdxs, edgeArrIdxs + m, std::make_pair(-1, -1));
    buildDfs(t);
    initTree();
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
