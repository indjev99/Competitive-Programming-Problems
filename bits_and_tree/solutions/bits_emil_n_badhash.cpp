#include "bits_and_tree.h"

#include <algorithm>
#include <random>

const int SEED = 0;

const double NUM_TOTAL_BITS = 200 - 1;

const int HASH_BITS = 22;
const int HASH_BASE = 293;

const double NUM_BITS = NUM_TOTAL_BITS - HASH_BITS;

void xorData(std::vector<bool>& data)
{
    std::mt19937 gen(SEED);
    std::uniform_int_distribution<int> distr(0, 1);

    for (int i = 0; i < NUM_BITS; i++)
    {
        data[i] = data[i] ^ distr(gen);
    }
}

bool hashData(std::vector<bool>& data, bool check)
{
    int hash = 0;
    for (int i = 0; i < NUM_BITS; i++)
    {
        hash = hash * HASH_BASE + data[i];
    }

    hash = hash & ((1 << HASH_BITS) - 1);

    for (int i = 0; i < HASH_BITS; i++)
    {
        if (check)
        {
            if (data[NUM_BITS + i] != ((hash >> i) & 1))
                return false;
        }
        else
        {
            data[NUM_BITS + i] = (hash >> i) & 1;
        }
    }

    return true;
}

std::vector<std::pair<int, int>> encode(int n, std::vector<bool> data)
{
    data.resize(NUM_TOTAL_BITS);

    xorData(data);
    hashData(data, false);

    std::vector<std::pair<int, int>> edges;

    int currNode = 0;
    int currChainNode = 0;
    for (bool b : data)
    {
        if (b)
        {
            edges.push_back({currNode, currNode + 1});
            currNode++;
        }
        else
        {
            edges.push_back({currChainNode, currNode + 1});
            currNode++;
            currChainNode = currNode;
        }
    }

    while (currNode < n - 1)
    {
        edges.push_back({currChainNode, currNode + 1});
        currNode++;
        currChainNode = currNode;
    }

    return edges;
}

std::vector<std::vector<int>> adj;

#define B_NONE 0
#define B_ONE 1
#define B_MULTI 2

int dfsEnd(int curr, int par)
{
    if (adj[curr].size() > 3) return B_MULTI;

    int cnt = 0;
    for (int other : adj[curr])
    {
        if (other == par) continue;

        int brNext = dfsEnd(other, curr);

        if (brNext == B_MULTI) return B_MULTI;

        if (brNext == B_ONE) cnt++;
        if (cnt == 2) return B_MULTI;
    }

    if (adj[curr].size() > 2) return B_ONE;
    if (cnt > 0) return B_ONE;

    return B_NONE;
}

std::vector<int> toEnd;

void dfsToEnd(int curr, int par)
{
    toEnd[curr] = par;

    for (int other : adj[curr])
    {
        if (other == par) continue;

        dfsToEnd(other, curr);
    }
}

bool fail = false;

int dfsDecodeSub(int curr, int par)
{
    if (par != toEnd[curr])
    {
        fail = true;
        return 0;
    }

    int cnt = 0;
    int sz = 1;
    for (int other : adj[curr])
    {
        if (other == par) continue;

        if (cnt > 0)
        {
            fail = true;
            return 0;
        }

        cnt++;

        sz += dfsDecodeSub(other, curr);
    }

    return sz;
}

std::vector<int> lens;

void dfsDecode(int curr, int par)
{
    int cnt = 0;
    int len = 0;
    for (int other : adj[curr])
    {
        if (other == par) continue;
        if (other == toEnd[curr]) continue;

        if (cnt > 0)
        {
            fail = true;
            return;
        }

        cnt++;

        len = dfsDecodeSub(other, curr);

        if (fail) return;
    }

    lens.push_back(len);

    if (toEnd[curr] == -1) return;

    dfsDecode(toEnd[curr], curr);
}

std::vector<bool> bits;

void decodeImpl()
{
    bits.clear();

    for (int len : lens)
    {
        for (int i = 0; i < len; i++)
        {
            bits.push_back(1);
        }
        bits.push_back(0);
    }

    if (bits.size() < NUM_TOTAL_BITS)
    {
        bits.clear();
        return;
    }

    bits.resize(NUM_TOTAL_BITS);

    if (!hashData(bits, true))
    {
        bits.clear();
        return;
    }

    bits.resize(NUM_BITS);

    xorData(bits);
}

std::vector<int> ends;

std::vector<bool> decode(int n, std::vector<std::pair<int, int>> tree)
{
    adj.clear();
    adj.resize(n + 1);
    for (auto [f, t] : tree)
    {
        adj[f].push_back(t);
        adj[t].push_back(f);
    }

    toEnd.resize(n + 1);

    for (int ignored = 0; ignored <= n; ignored++)
    {
        if (adj[ignored].size() != 1) continue;

        for (int oth : adj[ignored])
        {
            adj[oth].erase(std::remove(adj[oth].begin(), adj[oth].end(), ignored), adj[oth].end());
        }

        ends.clear();
        for (int i = 0; i <= n; i++)
        {
            if (i == ignored) continue;

            if (adj[i].size() > 2) continue;

            int br = dfsEnd(i, -1);

            if (br == B_MULTI) continue;
    
            ends.push_back(i);
        }

        for (int end : ends)
        {
            std::fill(toEnd.begin(), toEnd.end(), -1);
            dfsToEnd(end, -1);

            for (int start : ends)
            {
                fail = false;
                lens.clear();
                dfsDecode(start, -1);
                if (fail) continue;
                decodeImpl();

                if (bits.size() > 0)
                {
                    return bits;
                }
            }
        }

        for (int i : adj[ignored])
        {
            adj[i].push_back(ignored);
        }
    }

    return {};
}
