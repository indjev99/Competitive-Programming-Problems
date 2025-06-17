#include "subway.h"

#include <algorithm>
#include <queue>
#include <cmath>
#include <random>

constexpr bool SHUFFLE_NODES = true;
constexpr bool SHUFFLE_EDGES = true;

constexpr bool DO_DNC = true;
constexpr bool OPT_DNC_RIGHT = true;

constexpr bool DO_DINIC_DNC = false;

constexpr bool USE_KUHN = true;
constexpr bool USE_DINIC = false;

constexpr bool PREMATCH_KUHN = true;

struct Edge
{
    int idx;
    int from;
    int to;
};

struct DirEdge
{
    int idx;
    int to;
};

int n;
std::vector<Edge> origEdges;

std::vector<std::vector<DirEdge>> adjKuhn;
std::vector<DirEdge> matchKuhn;
std::vector<bool> prematchedKuhn;
std::vector<bool> usedKuhn;
std::vector<int> usedListKuhn;

bool dfsKuhn(int curr)
{
    if (usedKuhn[curr]) return false;

    usedKuhn[curr] = true;
    usedListKuhn.push_back(curr);

    for (const DirEdge& e : adjKuhn[curr])
    {
        int to = e.to - n;
        if (matchKuhn[to].idx == -1 || dfsKuhn(matchKuhn[to].to))
        {
            matchKuhn[to] = {e.idx, curr};
            return true;
        }
    }

    return false;
}

std::vector<int> findMatchingKuhn(const std::vector<Edge>& edges)
{
    adjKuhn.assign(n, {});

    for (const Edge& e : edges)
    {
        adjKuhn[e.from].push_back({e.idx, e.to});
    }

    matchKuhn.assign(n, {-1, -1});

    if (PREMATCH_KUHN)
    {
        prematchedKuhn.assign(n, false);

        for (const Edge& e : edges)
        {
            if (!prematchedKuhn[e.from] && matchKuhn[e.to - n].idx == -1)
            {
                prematchedKuhn[e.from] = true;
                matchKuhn[e.to - n] = {e.idx, e.from};
            }
        }
    }

    usedKuhn.resize(n, false);

    for (int i = 0; i < n; i++)
    {
        if (PREMATCH_KUHN && prematchedKuhn[i]) continue;

        dfsKuhn(i);

        for (int i : usedListKuhn)
        {
            usedKuhn[i] = false;
        }
        usedListKuhn.clear();
    }

    std::vector<int> part(n);
    for (int i = 0; i < n; i++)
    {
        part[i] = matchKuhn[i].idx;
    }

    return part;
}

struct DinicEdge
{
    int idx;
    int to;
    int revPos;
    bool avail;
};

std::vector<std::vector<DinicEdge>> adjDinic;
std::vector<int> levelDinic;
std::vector<int> ptrDinic;
int srcDinic;
int sinkDinic;

void bfsDinic()
{
    std::queue<int> q;

    levelDinic.assign(2 * n + 2, -1);

    levelDinic[srcDinic] = 0;
    q.push(srcDinic);

    while (!q.empty())
    {
        int curr = q.front();
        q.pop();

        if (levelDinic[sinkDinic] != -1 && levelDinic[curr] + 1 >= levelDinic[sinkDinic]) continue;

        for (const auto& e : adjDinic[curr])
        {
            if (!e.avail || levelDinic[e.to] != -1) continue;

            levelDinic[e.to] = levelDinic[curr] + 1;
            q.push(e.to);
        }
    }
}

bool dfsDinic(int curr)
{
    if (curr == sinkDinic) return true;

    for (int& i = ptrDinic[curr]; i < (int) adjDinic[curr].size(); i++)
    {
        auto& e = adjDinic[curr][i];

        if (!e.avail || levelDinic[curr] + 1 != levelDinic[e.to]) continue;

        bool succ = dfsDinic(e.to);

        if (!succ) continue;

        e.avail = false;
        adjDinic[e.to][e.revPos].avail = true;

        return true;
    }

    return false;
}

std::vector<int> findMatchingDinic(const std::vector<Edge>& edges)
{
    adjDinic.assign(2 * n + 2, {});

    srcDinic = 2 * n;
    sinkDinic = 2 * n + 1;

    for (int i = 0; i < n; i++)
    {
        adjDinic[srcDinic].push_back({-1, i, (int) adjDinic[i].size(), true});
        adjDinic[i].push_back({-1, srcDinic, (int) adjDinic[srcDinic].size() - 1, false});

        adjDinic[n + i].push_back({-1, sinkDinic, (int) adjDinic[sinkDinic].size(), true});
        adjDinic[sinkDinic].push_back({-1, n + i, (int) adjDinic[n + i].size() - 1, false});
    }

    for (const Edge& e : edges)
    {
        adjDinic[e.from].push_back({e.idx, e.to, (int) adjDinic[e.to].size(), true});
        adjDinic[e.to].push_back({-1, e.from, (int) adjDinic[e.from].size() - 1, false});
    }

    int flow = 0;
    while (flow < n)
    {
        bfsDinic();

        ptrDinic.assign(2 * n + 2, 0);

        while (flow < n && dfsDinic(srcDinic))
        {
            flow++;
        }
    }

    std::vector<int> part(n);

    for (int i = 0; i < n; i++)
    {
        for (const auto& e : adjDinic[i])
        {
            if (e.idx >= 0 && !e.avail)
            {
                part[i] = e.idx;
                break;
            }
        }
    }

    return part;
}

std::vector<int> findMatching(const std::vector<Edge>& edges)
{
    if (USE_KUHN) return findMatchingKuhn(edges);
    else if (USE_DINIC) return findMatchingDinic(edges);
    else exit(1);
}

std::vector<int> edgeVal;

std::pair<std::vector<Edge>, std::vector<Edge>> splitEdgesDinic(std::vector<Edge> edges)
{
    int d = edges.size() / n;
    int halfD = d / 2;

    adjDinic.assign(2 * n + 2, {});

    srcDinic = 2 * n;
    sinkDinic = 2 * n + 1;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < halfD; j++)
        {
            adjDinic[srcDinic].push_back({-1, i, (int) adjDinic[i].size(), true});
            adjDinic[i].push_back({-1, srcDinic, (int) adjDinic[srcDinic].size() - 1, false});

            adjDinic[n + i].push_back({-1, sinkDinic, (int) adjDinic[sinkDinic].size(), true});
            adjDinic[sinkDinic].push_back({-1, n + i, (int) adjDinic[n + i].size() - 1, false});
        }
    }

    for (const Edge& e : edges)
    {
        adjDinic[e.from].push_back({e.idx, e.to, (int) adjDinic[e.to].size(), true});
        adjDinic[e.to].push_back({-1, e.from, (int) adjDinic[e.from].size() - 1, false});
    }

    int flow = 0;
    while (flow < n * halfD)
    {
        bfsDinic();

        ptrDinic.assign(2 * n + 2, 0);

        while (flow < n * halfD && dfsDinic(srcDinic))
        {
            flow++;
        }
    }

    for (int i = 0; i < n; i++)
    {
        for (const auto& e : adjDinic[i])
        {
            if (e.idx >= 0)
            {
                if (!e.avail) edgeVal[e.idx] = 1;
                else edgeVal[e.idx] = 2;
            }
        }
    }

    std::vector<Edge> left;
    std::vector<Edge> right;

    left.reserve(n * halfD);
    right.reserve(n * (d - halfD) / 2);

    for (const Edge& e : edges)
    {
        if (edgeVal[e.idx] == 1) left.push_back(e);
        else right.push_back(e);

        edgeVal[e.idx] = 0;
    }

    return {std::move(left), std::move(right)};
}

std::vector<std::vector<DirEdge>> adjSplit;

void dfsSplit(int curr, int half = 1)
{
    auto& edges = adjSplit[curr];

    while (!edges.empty() && edgeVal[edges.back().idx])
    {
        edges.pop_back();
    }

    if (edges.empty()) return;

    int next = edges.back().to;
    edgeVal[edges.back().idx] = half;
    edges.pop_back();

    dfsSplit(next, half == 1 ? 2 : 1);
}

std::pair<std::vector<Edge>, std::vector<Edge>> splitEdges(std::vector<Edge> edges)
{
    int d = edges.size() / n;

    adjSplit.assign(2 * n, {});

    for (const Edge& e : edges)
    {
        adjSplit[e.from].push_back({e.idx, e.to});
        adjSplit[e.to].push_back({e.idx, e.from});
    }

    for (int i = 0; i < 2 * n; i++)
    {
        while (!adjSplit[i].empty())
        {
            dfsSplit(i);
        }
    }

    std::vector<Edge> left;
    std::vector<Edge> right;

    left.reserve(n * d / 2);
    right.reserve(n * d / 2);

    for (const Edge& e : edges)
    {
        if (edgeVal[e.idx] == 1) left.push_back(e);
        else right.push_back(e);

        edgeVal[e.idx] = 0;
    }

    return {std::move(left), std::move(right)};
}

std::vector<std::vector<int>> solve(std::vector<Edge> edges)
{
    int d = edges.size() / n;

    if (d == 0) return {};

    if (d == 1)
    {
        std::vector<int> part(n);
        for (int i = 0; i < n; i++)
        {
            part[i] = edges[i].idx;
        }
        return {part};
    }

    if (DO_DNC && d % 2 == 0)
    {
        auto [left, right] = splitEdges(std::move(edges));

        std::vector<std::vector<int>> parts = solve(std::move(left));

        if (OPT_DNC_RIGHT)
        {
            int rd = d / 2;
            int pow2 = 1;
            while (pow2 < rd) pow2 *= 2;
            int moveCnt = pow2 - rd;

            for (int i = 0; i < moveCnt; i++)
            {
                for (int idx : parts.back())
                {
                    right.push_back(origEdges[idx]);
                }

                parts.pop_back();
            }
        }

        std::vector<std::vector<int>> rightParts = solve(std::move(right));

        for (auto& part : rightParts)
        {
            parts.push_back(std::move(part));
        }

        return parts;
    }

    if (DO_DINIC_DNC)
    {
        auto [left, right] = splitEdgesDinic(std::move(edges));

        std::vector<std::vector<int>> parts = solve(std::move(left));
        std::vector<std::vector<int>> rightParts = solve(std::move(right));

        for (auto& part : rightParts)
        {
            parts.push_back(std::move(part));
        }

        return parts;
    }

    std::vector<int> part = findMatching(edges);

    for (int idx : part)
    {
        edgeVal[idx] = 1;
    }

    for (int i = 0; i < (int) edges.size(); i++)
    {
        if (edgeVal[edges[i].idx])
        {
            std::swap(edges[i], edges.back());
            edges.pop_back();
            i--;
        }
    }

    for (int idx : part)
    {
        edgeVal[idx] = 0;
    }

    std::vector<std::vector<int>> parts = solve(std::move(edges));

    parts.push_back(std::move(part));

    return parts;
}

bool assign_roads(int _n, int m, std::vector<int> a, std::vector<int> b)
{
    std::mt19937 generator(0);

    n = _n;

    if (m % n != 0) return false;

    edgeVal.assign(m, 0);

    int d = m / n;

    std::vector<int> nodeIds(n);
    std::iota(nodeIds.begin(), nodeIds.end(), 0);

    if (SHUFFLE_NODES)
    {
        std::shuffle(nodeIds.begin(), nodeIds.end(), generator);
    }

    std::vector<Edge> edges;
    edges.resize(m);

    std::vector<int> degs(2 * n);

    for (int i = 0; i < m; i++)
    {
        edges[i] = {i, nodeIds[a[i]], n + nodeIds[b[i]]};
        degs[edges[i].from]++;
        degs[edges[i].to]++;
    }

    for (int i = 0; i < 2 * n; i++)
    {
        if (degs[i] != d) return false;
    }

    origEdges = edges;

    if (SHUFFLE_EDGES)
    {
        std::shuffle(edges.begin(), edges.end(), generator);
    }

    std::vector<std::vector<int>> parts = solve(std::move(edges));

    for (int i = 0; i < d; i++)
    {
        answer(i, parts[i]);
    }

    return true;
}
