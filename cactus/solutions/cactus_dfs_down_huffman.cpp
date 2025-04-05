#include "cactus.h"

#include <cassert>
#include <algorithm>
#include <queue>
#include <map>

int minEdges(int n)
{
    return n - 1;
}

int maxEdges(int n)
{
    return 3 * (n - 1) / 2;
}

struct Node
{
    bool leaf;
    int val = -1;
    int child0 = -1;
    int child1 = -1;
    int par = -1;
    bool bit = false;
};

struct Huffman
{
    int root;
    std::map<int, int> valNodes;
    std::vector<Node> nodes;
};

void encodeNumber(std::vector<bool>& data, int num, int lb, int ub, const Huffman* huff = nullptr)
{
    assert(lb < ub);
    assert(num >= lb);
    assert(num < ub);

    ub -= lb;
    num -= lb;

    if (!huff)
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
        int curr = huff->valNodes.at(num);
        std::vector<bool> code;
        while (curr != huff->root)
        {
            code.push_back(huff->nodes.at(curr).bit);
            curr = huff->nodes.at(curr).par;
        }
        std::reverse(code.begin(), code.end());
        for (bool b : code)
        {
            data.push_back(b);
        }
    }
}

int decodeNumber(const std::vector<bool>& data, int& pos, int lb, int ub, const Huffman* huff = nullptr)
{
    assert(lb < ub);

    ub -= lb;

    int num = 0;
    if (!huff)
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
        int curr = huff->root;
        while (!huff->nodes.at(curr).leaf)
        {
            bool b = data.at(pos++);
            if (b) curr = huff->nodes.at(curr).child1;
            else curr = huff->nodes.at(curr).child0;
        }
        num = huff->nodes.at(curr).val;
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

std::map<int, int> occsDown, occsUp;

Huffman huffDown, huffUp;

bool dryRun = true;

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

        if (dryRun) occsDown[cntWithoutCycle]++;
        else encodeNumber(data, cntWithoutCycle, 0, ub, &huffDown);
        cntWithoutCycle = 0;
    }

    void finishEncode(std::vector<bool>& data)
    {
        if (cntWithoutCycle == 0)
        {
            return;
        }

        if (dryRun) occsDown[cntWithoutCycle]++;
        else encodeNumber(data, cntWithoutCycle, 0, ub, &huffDown);
        cntWithoutCycle = 0;
    }

    bool decodeHasCycle(const std::vector<bool>& data, int& pos)
    {
        if (cntWithoutCycle == -1)
        {
            cntWithoutCycle = decodeNumber(data, pos, 0, ub, &huffDown);
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

        if (dryRun) occsUp[relDepth - MIN_REL_BACK_EDGE_DEPTH]++;
        else encodeNumber(data, relDepth, MIN_REL_BACK_EDGE_DEPTH, maxRelDepth + 1, &huffUp);
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
            int relDepth = decodeNumber(data, pos, MIN_REL_BACK_EDGE_DEPTH, maxRelDepth + 1, &huffUp);
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

void encodeOccs(int n, const std::map<int, int>& occs, std::vector<bool>& data)
{
    encodeNumber(data, occs.size(), 0, n);

    for (auto [num, cnt] : occs)
    {
        encodeNumber(data, num, 0, n);
        encodeNumber(data, cnt, 0, n);
    }
}

void decodeOccs(int n, std::map<int, int>& occs, const std::vector<bool>& data, int& pos)
{
    int size = decodeNumber(data, pos, 0, n);

    for (int i = 0; i < size; i++)
    {
        int num = decodeNumber(data, pos, 0, n);
        int cnt = decodeNumber(data, pos, 0, n);
        occs[num] = cnt;
    }
}

Huffman buildHuffman(const std::map<int, int>& occs)
{
    Huffman huff;

    std::priority_queue<std::pair<int, int>> cntToNodeIdx;

    for (auto [num, cnt] : occs)
    {
        huff.nodes.push_back({true, num});
        cntToNodeIdx.push({-cnt, huff.nodes.size() - 1});
        huff.valNodes[num] = huff.nodes.size() - 1;
    }

    while (cntToNodeIdx.size() > 1)
    {
        auto [lc, l] = cntToNodeIdx.top();
        cntToNodeIdx.pop();

        huff.nodes.at(l).par = huff.nodes.size();
        huff.nodes.at(l).bit = false;

        auto [rc, r] = cntToNodeIdx.top();
        cntToNodeIdx.pop();

        huff.nodes.at(r).par = huff.nodes.size();
        huff.nodes.at(r).bit = true;

        huff.nodes.push_back({false, -1, l, r});
        cntToNodeIdx.push({lc + rc, huff.nodes.size() - 1});
    }

    if (!cntToNodeIdx.empty())
    {
        huff.root = cntToNodeIdx.top().second;
    }
    else
    {
        huff.root = -1;
    }

    return huff;
}

std::vector<bool> encode(int n, std::vector<std::pair<int, int>> edges)
{
    occsUp.clear();
    occsDown.clear();

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

    {
        dryRun = true;
        DownTracker dt(n, true);
        encDfs2(0, 0, pars, depths, children, adj, dt, data);
        dt.finishEncode(data);
        dryRun = false;
    }

    encodeOccs(n, occsDown, data);
    encodeOccs(n, occsUp, data);

    huffDown = buildHuffman(occsDown);
    huffUp = buildHuffman(occsUp);

    DownTracker dt(n, true);
    encDfs2(0, 0, pars, depths, children, adj, dt, data);
    dt.finishEncode(data);

    return data;
}

std::vector<std::pair<int, int>> decode(int n, std::vector<bool> data)
{
    occsUp.clear();
    occsDown.clear();

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

    decodeOccs(n, occsDown, data, pos);
    decodeOccs(n, occsUp, data, pos);

    huffDown = buildHuffman(occsDown);
    huffUp = buildHuffman(occsUp);

    DownTracker dt(n, false);
    decDfs2(0, 0, pars, depths, children, edges, dt, data, pos);

    return edges;
}
