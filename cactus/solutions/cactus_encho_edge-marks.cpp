#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <set>
#include <assert.h>
using namespace std;

vector<bool> toBinary(int x, int len = 14)
{
    vector<bool> coded;
    for (int i = 0; i < len; i++)
    {
        coded.push_back(x % 2);
        x /= 2;
    }
    reverse(coded.begin(), coded.end());

    return coded;
}

void appendBinary(vector<bool>& res, int numToAppend)
{
    vector<bool> tmp = toBinary(numToAppend);
    res.insert(res.end(), tmp.begin(), tmp.end());
}

int fromBinary(const vector<bool>& x, int len = 14)
{
    int num = 0;
    for (int i = 0; i < len; i++)
    {
        num = num * 2 + int(x[i]);
    }
    return num;
}

int readBinary(const vector<bool>& raw, int idx)
{
    vector<bool> tmp;
    for (int i = 0; i < 14; i++)
    {
        tmp.push_back(raw[idx + i]);
    }
    return fromBinary(tmp);
}

vector<int> Graph[20111];
int father[20111];
bool TFO[20111];
int depth[20111];
int backEdge[20111];

bool upEdgeMarked[20111];

void DFS(int ver, int dad)
{
    if (ver == 0)
        depth[ver] = 0;
    else
        depth[ver] = depth[dad] + 1;

    TFO[ver] = true;
    father[ver] = dad;
    for (auto nver : Graph[ver])
    {
        if (nver == dad)
            continue;

        if (TFO[nver])
        {
            /// Back edge
            if (depth[nver] < depth[ver])
            {
                assert(!upEdgeMarked[ver]);
                upEdgeMarked[ver] = true;

                int lastVerOnPath = ver;
                while(father[lastVerOnPath] != nver)
                    lastVerOnPath = father[lastVerOnPath];

                assert(!upEdgeMarked[lastVerOnPath]);
                upEdgeMarked[lastVerOnPath] = true;

                backEdge[ver] = nver;
            }
            continue;
        }

        DFS(nver, ver);
    }
}

vector<bool> encode(int n, vector<pair<int, int>> edges)
{
    vector<bool> result;

    for (int i = 0; i < n; i++)
    {
        Graph[i].clear();
        father[i] = -1;
        TFO[i] = false;
        upEdgeMarked[i] = false;
    }

    for (auto [a, b] : edges)
    {
        Graph[a].push_back(b);
        Graph[b].push_back(a);
    }

    DFS(0, -1);

    for (int i = 1; i < n; i++)
    {
        appendBinary(result, father[i]);
    }

    for (int i = 1; i < n; i++)
    {
        result.push_back(upEdgeMarked[i]);
    }

    return result;
}
vector<int> matchStack;

void makePairsDfs(int ver, int dad, vector<pair<int, int>>& edges)
{
    for (auto nver : Graph[ver])
    {
        if (nver == dad)
            continue;

        if (upEdgeMarked[nver])
        {
            matchStack.push_back(ver);
        }

        makePairsDfs(nver, ver, edges);

        if (upEdgeMarked[nver])
        {
            if (!matchStack.empty() && matchStack.back() == ver)
            {
                assert(matchStack.size() > 1);

                int a = nver;
                matchStack.pop_back();
                int b = matchStack.back();
                matchStack.pop_back();

                edges.push_back({a, b});
            }
        }
    }
}

vector<pair<int, int>> decode(int n, vector<bool> data)
{
    vector<pair<int, int>> edges;
    int ptr = 0;

    for (int i = 0; i < n; i++)
    {
        Graph[i].clear();
        upEdgeMarked[i] = false;
    }
    matchStack.clear();

    for (int i = 1; i < n; i++)
    {
        int parent = readBinary(data, ptr);
        ptr += 14;

        edges.push_back({i, parent});
        Graph[i].push_back(parent);
        Graph[parent].push_back(i);
    }

    for (int i = 1; i < n; i++)
    {
        upEdgeMarked[i] = data[ptr];
        ptr++;
    }

    makePairsDfs(0, -1, edges);
    assert(matchStack.empty());

    return edges;
}
