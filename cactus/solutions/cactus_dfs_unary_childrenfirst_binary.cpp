#include "cactus.h"

#include <cassert>
#include <algorithm>

int minEdges(int n)
{
    return n - 1;
}

int maxEdges(int n)
{
    return 3 * (n - 1) / 2;
}

void encodeNumber(std::vector<bool>& data, int num, int lb, int ub, bool binary = true)
{
    assert(lb < ub);
    assert(num >= lb);
    assert(num < ub);

    ub -= lb;
    num -= lb;

    if (binary)
    {
        int numBits = 0;
        while ((1 << numBits) < ub) numBits++;

        int maxVal = ub - 1;
        bool sent0 = false;

        for (int b = numBits - 1; b >= 0; b--)
        {
            if (!sent0 && !((maxVal >> b) & 1)) continue;
            bool currB = (num >> b) & 1;
            if (!currB) sent0 = true;
            data.push_back(currB);
        }
    }
    else
    {
        for (int i = 0; i < num; i++)
        {
            data.push_back(1);
        }
        if (num < ub - 1)
        {
            data.push_back(0);
        }
    }
}

int decodeNumber(const std::vector<bool>& data, int& pos, int lb, int ub, bool binary = true)
{
    assert(lb < ub);

    ub -= lb;

    int num = 0;
    if (binary)
    {
        int numBits = 0;
        while ((1 << numBits) < ub) numBits++;

        int maxVal = ub - 1;
        bool got0 = false;

        for (int b = numBits - 1; b >= 0; b--)
        {
            if (!got0 && !((maxVal >> b) & 1)) continue;
            bool currB = data.at(pos++);
            if (!currB) got0 = true;
            num |= currB << b;
        }
    }
    else
    {
        while (num < ub - 1 && data.at(pos++))
        {
            num++;
        }
    }

    num += lb;

    return num;
}

void encDfs(int curr, int par, int depth, const std::vector<std::vector<int>>& adj, std::vector<int>& pars, std::vector<int>& depths)
{
    pars[curr] = par;
    depths[curr] = depth;

    for (int u : adj[curr])
    {
        if (depths[u] != -1) continue;

        encDfs(u, curr, depth + 1, adj, pars, depths);
    }
}

void decDfs(int curr, int par, int depth, const std::vector<std::vector<int>>& children, std::vector<int>& depths)
{
    depths[curr] = depth;

    for (int u : children[curr])
    {
        decDfs(u, curr, depth + 1, children, depths);
    }
}

const int MIN_REL_BACK_EDGE_DEPTH = 2;

int encDfs2(int curr, int minBackEdgeDepth, const std::vector<int>& pars, const std::vector<int>& depths, const std::vector<std::vector<int>>& children, const std::vector<std::vector<int>>& adj, std::vector<bool>& data)
{
    int backEdgeDepth = -1;
    bool nonMineBackEdgeDepth = false;

    for (int u : children[curr])
    {
        int uBackEdgeDepth = encDfs2(u, minBackEdgeDepth, pars, depths, children, adj, data);

        if (uBackEdgeDepth != -1 && uBackEdgeDepth < depths[curr])
        {
            assert(backEdgeDepth == -1);

            backEdgeDepth = uBackEdgeDepth;
            minBackEdgeDepth = depths[curr];
            nonMineBackEdgeDepth = true;
        }
    }

    int maxRelDepth = depths[curr] - minBackEdgeDepth;

    for (int u : adj[curr])
    {
        if (u == pars[curr] || pars[u] == curr) continue;
        if (depths[u] > depths[curr]) continue;

        assert(backEdgeDepth == -1);

        backEdgeDepth = depths[u];
    }

    if (backEdgeDepth != -1 && !nonMineBackEdgeDepth)
    {
        assert(backEdgeDepth >= minBackEdgeDepth);

        int relDepth = depths[curr] - backEdgeDepth;

        assert(relDepth >= MIN_REL_BACK_EDGE_DEPTH);

        data.push_back(1);

        encodeNumber(data, relDepth, MIN_REL_BACK_EDGE_DEPTH, maxRelDepth + 1);
    }
    else if (maxRelDepth >= MIN_REL_BACK_EDGE_DEPTH)
    {
        data.push_back(0);
    }

    return backEdgeDepth;
}

int decDfs2(int curr, int minBackEdgeDepth, const std::vector<int>& pars, const std::vector<int>& depths, const std::vector<std::vector<int>>& children, std::vector<std::pair<int, int>>& edges, const std::vector<bool>& data, int& pos)
{
    int backEdgeDepth = -1;

    for (int u : children[curr])
    {
        int uBackEdgeDepth = decDfs2(u, minBackEdgeDepth, pars, depths, children, edges, data, pos);

        if (uBackEdgeDepth != -1 && uBackEdgeDepth < depths[curr])
        {
            assert(backEdgeDepth == -1);

            backEdgeDepth = uBackEdgeDepth;
            minBackEdgeDepth = depths[curr];
        }
    }

    int maxRelDepth = depths[curr] - minBackEdgeDepth;

    if (maxRelDepth >= MIN_REL_BACK_EDGE_DEPTH)
    {
        if (data.at(pos++))
        {
            int relDepth = decodeNumber(data, pos, MIN_REL_BACK_EDGE_DEPTH, maxRelDepth + 1);
            backEdgeDepth = depths[curr] - relDepth;

            int backNode = curr;
            while (depths[backNode] != backEdgeDepth) backNode = pars[backNode];
    
            edges.push_back({curr, backNode});
        }
    }

    return backEdgeDepth;
}

std::vector<bool> encode(int n, std::vector<std::pair<int, int>> edges)
{
    std::vector<bool> data;

    std::vector<std::vector<int>> adj(n);

    for (auto [u, v] : edges)
    {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    std::vector<int> pars(n, -1);
    std::vector<int> depths(n, -1);
    encDfs(0, -1, 0, adj, pars, depths);

    for (int i = 1; i < n; i++)
    {
        encodeNumber(data, pars[i], 0, n);
    }

    std::vector<std::vector<int>> children(n);
    for (int i = 1; i < n; i++)
    {
        children[pars[i]].push_back(i);
    }

    encDfs2(0, 0, pars, depths, children, adj, data);

    return data;
}

std::vector<std::pair<int, int>> decode(int n, std::vector<bool> data)
{
    int pos = 0;

    std::vector<std::pair<int, int>> edges;

    std::vector<int> pars(n, -1);
    for (int i = 1; i < n; i++)
    {
        pars[i] = decodeNumber(data, pos, 0, n);
        edges.push_back({pars[i], i});
    }

    std::vector<std::vector<int>> children(n);
    for (int i = 1; i < n; i++)
    {
        children[pars[i]].push_back(i);
    }

    std::vector<int> depths(n, -1);
    decDfs(0, -1, 0, children, depths);

    decDfs2(0, 0, pars, depths, children, edges, data, pos);

    return edges;
}
