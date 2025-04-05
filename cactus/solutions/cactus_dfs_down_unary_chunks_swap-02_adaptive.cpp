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

void encodeNumber(std::vector<bool>& data, int num, int lb, int ub, bool binary = true, bool chunkUnary = false, bool swap02 = false)
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
    else if (!chunkUnary)
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
    else
    {
        if (swap02 && ub >= 3)
        {
            if (num == 0) num = 2;
            else if (num == 2) num = 0;
        }

        int currNum = 0;
        for (int i = 0; i + 2 <= num; i += 2)
        {
            currNum += 2;
            data.push_back(1);
        }
        if (currNum < ub - 2)
        {
            data.push_back(0);
        }
        if (currNum < ub - 1)
        {
            if (currNum < num)
            {
                currNum++;
                data.push_back(1);
            }
            else data.push_back(0);
        }
        assert(currNum == num);
    }
}

int decodeNumber(const std::vector<bool>& data, int& pos, int lb, int ub, bool binary = true, bool chunkUnary = false, bool swap02 = false)
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
    else if (!chunkUnary)
    {
        while (num < ub - 1 && data.at(pos++))
        {
            num++;
        }
    }
    else
    {
        while (num < ub - 2 && data.at(pos++))
        {
            num += 2;
        }
        if (num < ub - 1)
        {
            if (data.at(pos++)) num++;
        }

        if (swap02 && ub >= 3)
        {
            if (num == 0) num = 2;
            else if (num == 2) num = 0;
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

struct DownTracker
{
    DownTracker(int n, bool enc):
        ub(n + 1),
        cntWithoutCycle(enc ? 0 : -1)
    {}

    int ub;
    int cntWithoutCycle;

    void encodeHasCycle(std::vector<bool>& data, bool hasCycle)
    {
        if (!hasCycle)
        {
            cntWithoutCycle++;
            return;
        }

        encodeNumber(data, cntWithoutCycle, 0, ub, false, true, true);
        cntWithoutCycle = 0;
    }

    void finishEncode(std::vector<bool>& data)
    {
        if (cntWithoutCycle == 0)
        {
            return;
        }

        encodeNumber(data, cntWithoutCycle, 0, ub, false, true, true);
        cntWithoutCycle = 0;
    }

    bool decodeHasCycle(const std::vector<bool>& data, int& pos)
    {
        if (cntWithoutCycle == -1)
        {
            cntWithoutCycle = decodeNumber(data, pos, 0, ub, false, true, true);
        }

        if (cntWithoutCycle > 0)
        {
            cntWithoutCycle--;
            return false;
        }
        else
        {
            cntWithoutCycle = -1;
            return true; 
        }
    }
};

const int MIN_REL_BACK_EDGE_DEPTH = 2;

bool useChunksForRBED = false;

int encDfs2(int curr, int minBackEdgeDepth, const std::vector<int>& pars, const std::vector<int>& depths, const std::vector<std::vector<int>>& children, const std::vector<std::vector<int>>& adj, DownTracker& dt, std::vector<bool>& data)
{
    int maxRelDepth = depths[curr] - minBackEdgeDepth;

    int backEdgeDepth = -1;

    for (int u : adj[curr])
    {
        if (u == pars[curr] || pars[u] == curr) continue;
        if (depths[u] > depths[curr]) continue;

        assert(backEdgeDepth == -1);

        backEdgeDepth = depths[u];
    }

    if (backEdgeDepth != -1)
    {
        assert(backEdgeDepth >= minBackEdgeDepth);

        int relDepth = depths[curr] - backEdgeDepth;

        assert(relDepth >= MIN_REL_BACK_EDGE_DEPTH);

        dt.encodeHasCycle(data, true);

        encodeNumber(data, relDepth, MIN_REL_BACK_EDGE_DEPTH, maxRelDepth + 1, false, useChunksForRBED);
    }
    else if (maxRelDepth >= MIN_REL_BACK_EDGE_DEPTH)
    {
        dt.encodeHasCycle(data, false);
    }

    if (backEdgeDepth != -1)
    {
        minBackEdgeDepth = depths[curr];
    }

    for (int u : children[curr])
    {
        int uBackEdgeDepth = encDfs2(u, minBackEdgeDepth, pars, depths, children, adj, dt, data);

        if (uBackEdgeDepth != -1 && uBackEdgeDepth < depths[curr])
        {
            assert(backEdgeDepth == -1);

            backEdgeDepth = uBackEdgeDepth;
            minBackEdgeDepth = depths[curr];
        }
    }

    return backEdgeDepth;
}

int decDfs2(int curr, int minBackEdgeDepth, const std::vector<int>& pars, const std::vector<int>& depths, const std::vector<std::vector<int>>& children, std::vector<std::pair<int, int>>& edges, DownTracker& dt, const std::vector<bool>& data, int& pos)
{
    int maxRelDepth = depths[curr] - minBackEdgeDepth;

    int backEdgeDepth = -1;

    if (maxRelDepth >= MIN_REL_BACK_EDGE_DEPTH)
    {
        if (dt.decodeHasCycle(data, pos))
        {
            int relDepth = decodeNumber(data, pos, MIN_REL_BACK_EDGE_DEPTH, maxRelDepth + 1, false, useChunksForRBED);
            backEdgeDepth = depths[curr] - relDepth;
        }
    }

    if (backEdgeDepth != -1)
    {
        int backNode = curr;
        while (depths[backNode] != backEdgeDepth) backNode = pars[backNode];

        edges.push_back({curr, backNode});

        minBackEdgeDepth = depths[curr];
    }

    for (int u : children[curr])
    {
        int uBackEdgeDepth = decDfs2(u, minBackEdgeDepth, pars, depths, children, edges, dt, data, pos);

        if (uBackEdgeDepth != -1 && uBackEdgeDepth < depths[curr])
        {
            assert(backEdgeDepth == -1);

            backEdgeDepth = uBackEdgeDepth;
            minBackEdgeDepth = depths[curr];
        }
    }

    return backEdgeDepth;
}

std::vector<bool> encodeImpl(int n, std::vector<std::pair<int, int>> edges)
{
    std::vector<bool> data;

    data.push_back(useChunksForRBED);

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

    DownTracker dt(n, true);
    encDfs2(0, 0, pars, depths, children, adj, dt, data);
    dt.finishEncode(data);

    return data;
}

std::vector<bool> encode(int n, std::vector<std::pair<int, int>> edges)
{
    useChunksForRBED = false;
    std::vector<bool> data0 = encodeImpl(n, edges);

    useChunksForRBED = true;
    std::vector<bool> data1 = encodeImpl(n, edges);

    if (data0.size() <= data1.size())
    {
        return data0;
    }
    else
    {
        return data1;
    }
}

std::vector<std::pair<int, int>> decode(int n, std::vector<bool> data)
{
    int pos = 0;

    useChunksForRBED = data.at(pos++);

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

    DownTracker dt(n, false);
    decDfs2(0, 0, pars, depths, children, edges, dt, data, pos);

    return edges;
}
