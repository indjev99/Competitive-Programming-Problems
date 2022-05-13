#include <iostream>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <unordered_set>
#include <vector>
#include <queue>

const int INF = 1e9;

const int MAX_N = 1e5;
const int MAX_M = 1e6;

const int MAX_N_POW2 = 1 << 17;
const int MAX_M_POW2 = 1 << 20;

int n, m, s, t;
std::pair<int, int> edges[MAX_M];
std::vector<std::pair<int, int>> adj[MAX_N];
std::unordered_set<int> adjSet[MAX_N];

void input(std::istream& in)
{
    in >> n >> m >> s >> t;
    for (int i = 0; i < m; ++i)
    {
        in >> edges[i].first >> edges[i].second;
        adj[edges[i].first].emplace_back(edges[i].second, i);
        adj[edges[i].second].emplace_back(edges[i].first, i);
        adjSet[edges[i].first].insert(edges[i].second);
        adjSet[edges[i].second].insert(edges[i].first);
    }
}

int contAns, contAnsPathLen;
int contAnsPath[MAX_N];

void readOutput(std::istream& out)
{
    out >> contAns;

    if (contAns == -1)
    {
        contAns = INF;
        return;
    }

    out >> contAnsPathLen;
    for (int i = 0; i <= contAnsPathLen; ++i)
    {
        out >> contAnsPath[i];
    }
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

bool checkContAns()
{
    if (contAns != ansDist[s]) return false;
    if (ansDist[s] == INF) return true;
    if (contAnsPathLen == 0) return false;
    if (contAnsPath[0] != s) return false;
    if (contAnsPath[contAnsPathLen] != t) return false;

    for (int i = 0; i <= contAnsPathLen; ++i)
    {
        int curr = contAnsPath[i];
        if (i < contAnsPathLen)
        {
            int next = contAnsPath[i + 1];
            auto& set = adjSet[curr];
            if (set.find(next) == set.end()) return false;
        }
        
        if (i + ansDist[curr] > ansDist[s]) return false;
    }

    return true;
}

int main(int argc, char *argv[])
{
    std::ifstream in(argv[1]);
    std::ifstream out(argv[3]);

    if (!in)
    {
        std::cout << "0\n";
        std::cerr << "In-file " << argv[1] << " not found.\n";
        return 0;
    }
    if (!out)
    {
        std::cout << "0\n";
        std::cerr << "Out-file " << argv[3] << " not found.\n";
        return 0;
    }

    input(in);
    readOutput(out);
    distBfs();
    std::iota(dsuPar, dsuPar + n, 0);
    std::fill(edgeArrIdxs, edgeArrIdxs + m, std::make_pair(-1, -1));
    buildDfs(t);
    initTree();
    maxBlockedDistDfs(t);
    ansDijkstra();
    
    if (checkContAns())
    {
        std::cout << "1\n";
    }
    else
    {
        std::cout << "0\n";
        std::cerr << "Wrong answer.\n";
    }

    return 0;
}
