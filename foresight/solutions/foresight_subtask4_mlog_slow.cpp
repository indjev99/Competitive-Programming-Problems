#include <iostream>
#include <algorithm>
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

int queryTreeUtil(int curr, int cl, int cr, int l, int r, int* tree)
{
    if (r <= cl || cr <= l) return INF;
    if (l <= cl && cr <= r) return tree[curr];

    return std::min(
        queryTreeUtil(curr * 2 + 1, cl, (cl + cr) / 2, l, r, tree),
        queryTreeUtil(curr * 2 + 2, (cl + cr) / 2, cr, l, r, tree)
    );
}

void updateTree(int pos, int val, int* tree, int treeSize)
{
    int curr = 0;
    int cl = 0;
    int cr = treeSize;

    while (cr - cl > 1)
    {
        int mid = (cl + cr) / 2;
        if (mid <= pos)
        {
            cl = mid;
            curr = curr * 2 + 2;
        }
        else
        {
            cr = mid;
            curr = curr * 2 + 1;
        }
    }

    tree[curr] = val;

    while (curr != 0)
    {
        curr = (curr - 1) / 2;
        tree[curr] = std::min(tree[curr * 2 + 1], tree[curr * 2 + 2]);
    }
}

int queryTree(int l, int r, int* tree, int treeSize)
{
    return queryTreeUtil(0, 0, treeSize, l, r, tree);
}

int edgeArrPos;
std::pair<int, int> edgeArrIdxs[MAX_M];
int edgeTree[MAX_M_POW2 * 4];

int lcaArrPos;
int nodeLcaArrIdx[MAX_N];
int lcaTree[MAX_N_POW2 * 4];

int currDfsOrd;
int dfsOrdToNode[MAX_N];

std::vector<int> edgesToRem[MAX_N];

int maxBlockedDist[MAX_N];

void maxBlockedDistDfs(int curr)
{
    int enterEdgeArrPos = edgeArrPos;

    int nodeDfsOrd = currDfsOrd++;
    dfsOrdToNode[nodeDfsOrd] = curr;

    nodeLcaArrIdx[curr] = lcaArrPos;
    updateTree(lcaArrPos++, nodeDfsOrd, lcaTree, 2 * n - 1);

    for (auto [other, edge] : adj[curr])
    {
        if (edge == parEdge[curr] || edge == parEdge[other]) continue;

        int depth = dist[curr] + dist[other];
        std::pair<int, int>& idx = edgeArrIdxs[edge];
        if (idx.first == -1) idx.first = edgeArrPos;
        else idx.second = edgeArrPos;
        updateTree(edgeArrPos++, depth, edgeTree, 2 * m);

        if (nodeLcaArrIdx[other] != -1)
        {
            int lcaOrd = queryTree(nodeLcaArrIdx[other], nodeLcaArrIdx[curr] + 1, lcaTree, 2 * n - 1);
            edgesToRem[dfsOrdToNode[lcaOrd]].push_back(edge);
        }
    }

    for (int other : children[curr])
    {
        maxBlockedDistDfs(other);
        updateTree(lcaArrPos++, nodeDfsOrd, lcaTree, 2 * n - 1);
    }

    if (curr == t)
    {
        maxBlockedDist[curr] = 0;
        return;
    }

    for (auto edge : edgesToRem[curr])
    {
        updateTree(edgeArrIdxs[edge].first, INF, edgeTree, 2 * m);
        updateTree(edgeArrIdxs[edge].second, INF, edgeTree, 2 * m);
    }

    int minEdgeDepth = queryTree(enterEdgeArrPos, edgeArrPos, edgeTree, 2 * m);
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
    std::fill(nodeLcaArrIdx, nodeLcaArrIdx + n, -1);
    std::fill(edgeArrIdxs, edgeArrIdxs + m, std::make_pair(-1, -1));
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
