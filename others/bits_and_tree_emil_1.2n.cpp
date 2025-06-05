#include "bits_and_tree.h"

#include <algorithm>
#include <random>

const int SEED = 302947;

const int NUM_TOTAL_BITS = 228;

const int HASH_BITS = 23;

const int NUM_BITS = NUM_TOTAL_BITS - HASH_BITS;

std::vector<std::vector<int>> hashPoses;

void precompHashPoses()
{
    std::mt19937 gen(SEED);
    std::uniform_int_distribution<int> distr(0, 1);

    hashPoses.clear();
    hashPoses.resize(HASH_BITS);

    for (int i = 0; i < HASH_BITS; i++)
    {
        for (int j = 0; j < NUM_BITS; j++)
        {
            if (distr(gen)) hashPoses[i].push_back(j);
        }
    }
}

void xorData(std::vector<bool>& data)
{
    std::mt19937 gen(SEED);
    std::uniform_int_distribution<int> distr(0, 1);

    for (int i = 0; i < NUM_BITS; i++)
    {
        bool bit = distr(gen);
        data[i] = data[i] ^ bit;
    }
}

bool hashData(std::vector<bool>& data, bool check)
{
    for (int i = 0; i < HASH_BITS; i++)
    {
        bool bit = 0;
        for (int j : hashPoses[i])
        {
            bit ^= data[j];
        }

        if (check)
        {
            if (data[NUM_BITS + i] != bit) return false;
        }
        else
        {
            data[NUM_BITS + i] = bit;
        }
    }

    return true;
}

std::vector<std::pair<int, int>> encode(int n, std::vector<bool> data)
{
    precompHashPoses();

    data.resize(NUM_TOTAL_BITS);

    xorData(data);
    hashData(data, false);

    int numOnes = 0;
    for (int i = 0; i < NUM_BITS; i++)
    {
        numOnes += data[i];
    }

    // std::cerr << std::endl << "NO: " << numOnes << std::endl;

    std::vector<std::pair<int, int>> edges;

    int currNode = 0;
    int currTreeNode = 0;
    int currChainNode = 0;
    std::vector<int> lastNodes = {0};
    bool useLastNodes = false;
    for (bool b : data)
    {
        if (useLastNodes && lastNodes.empty())
        {
            lastNodes.push_back(currChainNode);
            useLastNodes = false;
        }

        // std::cerr << b << ": ";

        if (useLastNodes)
        {
            if (b)
            {
                // std::cerr << "extra: " << lastNodes.back() << " " << currNode + 1 << std::endl;
                edges.push_back({lastNodes.back(), currNode + 1});
                currNode++;
            }
            // else std::cerr << "none: " << lastNodes.back() << std::endl;

            lastNodes.pop_back();

            continue;
        }

        if (b)
        {
            // std::cerr << "side: " << currTreeNode << " " << currNode + 1 << std::endl;
            edges.push_back({currTreeNode, currNode + 1});
            lastNodes.push_back(currNode + 1);
            currNode++;
            currTreeNode = currNode;
        }
        else
        {
            // std::cerr << "down: " << currChainNode << " " << currNode + 1 << std::endl;
            edges.push_back({currChainNode, currNode + 1});
            currNode++;
            currTreeNode = currNode;
            currChainNode = currNode;
            lastNodes.pop_back();
            useLastNodes = true;
        }
    }

    while (currNode < n - 3)
    {
        // std::cerr << "pad: " << currChainNode << " " << currNode + 1 << std::endl;
        edges.push_back({currChainNode, currNode + 1});
        currNode++;
        currChainNode = currNode;
    }

    // std::cerr << "mark: " << 0 << " " << currNode + 1 << std::endl;
    edges.push_back({0, currNode + 1});
    currNode++;

    // std::cerr << "mark: " << currChainNode << " " << currNode + 1 << std::endl;
    edges.push_back({currChainNode, currNode + 1});
    currNode++;

    return edges;
}

std::vector<std::vector<int>> adj;

#define B_NONE 0
#define B_ONE 1
#define B_MULTI 2

int dfsEnd(int curr, int par)
{
    if (adj[curr].size() > 4) return B_MULTI;

    int cnt = 0;
    for (int other : adj[curr])
    {
        if (other == par) continue;

        int brNext = dfsEnd(other, curr);

        if (brNext == B_MULTI) return B_MULTI;

        if (brNext == B_ONE) cnt++;
        if (cnt == 2) return B_MULTI;
    }

    if (adj[curr].size() > 3) return B_ONE;
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

std::vector<bool> extra;

int dfsDecodeSub(int curr, int par)
{
    if (par != toEnd[curr])
    {
        // std::cerr << "failed: " << "par, toEnd: " << curr << " " << par << " " << toEnd[curr] << std::endl;
        fail = true;
        return 0;
    }

    int cnt = 0;
    int len = 1;
    int cntExtra = 0;
    for (int other : adj[curr])
    {
        if (other == par) continue;

        int lenIn = dfsDecodeSub(other, curr);

        if (fail) return 0;

        // std::cerr << "inside: " << curr << " " << par << ": " << len << " " << cnt << " " << cntExtra << ": " << other << ": " << lenIn << std::endl;

        if (lenIn == 1) cntExtra++;
        else
        {
            len = 1 + lenIn;
            cnt++;
        }

        if (cnt > 1 || cnt + cntExtra > 2)
        {
            fail = true;
            return 0;
        }
    }

    if (cnt == 0 && cntExtra > 0)
    {
        cntExtra--;
        cnt = 1;
        len = 2;
    }

    // std::cerr << "donin: " << curr << " " << par << ": " << len << " " << cnt << " " << cntExtra << std::endl;

    if (cnt > 0)
    {
        extra.push_back(cntExtra > 0);
    }

    return len;
}

std::vector<int> lens;
std::vector<std::vector<bool>> extras;

void dfsDecode(int curr, int par)
{
    extra.clear();

    int cnt = 0;
    int len = 0;
    int cntExtra = 0;
    for (int other : adj[curr])
    {
        if (other == par) continue;
        if (other == toEnd[curr]) continue;

        int lenIn = dfsDecodeSub(other, curr);

        if (fail) return;

        if (lenIn == 1) cntExtra++;
        else
        {
            len = lenIn;
            cnt++;
        }

        if (cnt > 1 || cnt + cntExtra > 2)
        {
            fail = true;
            return;
        }
    }

    if (cnt == 0 && cntExtra > 0)
    {
        cntExtra--;
        cnt = 1;
        len = 1;
    }

    // std::cerr << "doneout: " << curr << " " << par << ": " << len << " " << cnt << " " << cntExtra << std::endl;

    if (cnt > 0)
    {
        extra.push_back(cntExtra > 0);
    }

    lens.push_back(len);
    extras.push_back(extra);

    if (toEnd[curr] == -1) return;

    dfsDecode(toEnd[curr], curr);
}

std::vector<bool> bits;

long long totalHash = 0;

void decodeImpl()
{
    bits.clear();

    for (int t = 0; t < (int) lens.size(); t++)
    {
        int len = lens[t];
        const std::vector<bool>& extra = extras[t];

        // std::cerr << len << ": ";
        // for (bool b : extra) std::cerr << " " << b;
        // std::cerr << std::endl;

        for (int i = 0; i < len; i++)
        {
            bits.push_back(1);
        }
        bits.push_back(0);

        for (bool b : extra)
        {
            bits.push_back(b);
        }
    }

    // for (bool b : bits)
    // {
    //     std::cerr << b;
    // }
    // std::cerr << std::endl;

    if (bits.size() < NUM_TOTAL_BITS)
    {
        bits.clear();
        return;
    }

    bits.resize(NUM_TOTAL_BITS);

    // std::cerr << "HASH" << std::endl;
    totalHash++;

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
    // std::cerr << "decode: " << tree.size() << std::endl;

    precompHashPoses();

    adj.clear();
    adj.resize(n + 1);
    for (auto [f, t] : tree)
    {
        adj[f].push_back(t);
        adj[t].push_back(f);

        // std::cerr << "  " << f << " " << t << std::endl;
    }

    toEnd.resize(n + 1);

    auto eraseNode = [&](int node)
    {
        for (int oth : adj[node])
        {
            adj[oth].erase(std::remove(adj[oth].begin(), adj[oth].end(), node), adj[oth].end());
        }
    };

    auto returnNode = [&](int node)
    {
        for (int i : adj[node])
        {
            adj[i].push_back(node);
        }
    };

    for (int ignored = 0; ignored <= n; ignored++)
    {
        if (adj[ignored].size() != 1) continue;

        eraseNode(ignored);

        // std::cerr << "Ignore: " << ignored << std::endl;
        // std::cerr << "  Ends: ";

        ends.clear();
        for (int i = 0; i <= n; i++)
        {
            if (i == ignored) continue;

            if (adj[i].size() > 1) continue;

            int br = dfsEnd(i, -1);

            if (br == B_MULTI) continue;
    
            // std::cerr << " " << i;
            ends.push_back(i);
        }
        // std::cerr << std::endl;

        for (int endMark : ends)
        {
            int end = adj[endMark][0];

            eraseNode(endMark);

            std::fill(toEnd.begin(), toEnd.end(), -1);
            dfsToEnd(end, -1);

            for (int startMark : ends)
            {
                if (startMark == endMark || startMark == end) continue;

                // std::cerr << "here: " << startMark << " " << endMark << " " << end << std::endl;
                // std::cerr << "here: " << adj[startMark].size() << std::endl;

                int start = adj[startMark][0];

                // std::cerr << "there" << std::endl;

                eraseNode(startMark);

                // std::cerr << "Try: " << ignored << " " << startMark << " " << endMark << ": " << start << " " << end << std::endl;

                // for (int i = 0; i <= n; i++)
                // {
                //     if (i == ignored || i == startMark || i == endMark) continue;
                //     std::cerr << "  " << i << ": ";
                //     for (int j : adj[i]) std::cerr << " " << j;
                //     std::cerr << std::endl;
                // }

                fail = false;
                lens.clear();
                extras.clear();
                dfsDecode(start, -1);

                if (!fail)
                {
                    // std::cerr << "NO FAIL" << std::endl;

                    decodeImpl();

                    if (bits.size() > 0)
                    {
                        // std::cerr << totalHash << std::endl;
                    
                        return bits;
                    }
                }

                // std::cerr << "DONE" << std::endl;

                returnNode(startMark);

                // std::cerr << "aa" << std::endl;
            }

            // std::cerr << "bb" << std::endl;

            returnNode(endMark);

            // std::cerr << "cc" << std::endl;
        }

        returnNode(ignored);
    }

    return {};
}
