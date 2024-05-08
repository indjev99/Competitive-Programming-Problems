#include <iostream>
#include <algorithm>
#include <vector>

#include "gpus.h"

int n, m;

std::vector<int> gpuCosts;
std::vector<int> reqTimes;

__int128 ans;

int treeMaxIdx;
std::vector<std::pair<int, int>> tree;
std::vector<int> posToNode;

std::pair<int, int> initTreeImpl(int curr, int cl, int cr)
{
    if (cr - cl == 1)
    {
        posToNode[cl] = curr;
        return tree[curr] = {0, -cl};
    }

    int left = curr * 2 + 1;
    int right = curr * 2 + 2;
    int cm = (cl + cr) / 2;

    return tree[curr] = std::min(
        initTreeImpl(left, cl, cm),
        initTreeImpl(right, cm, cr)
    );
}

void initTree(int f)
{
    treeMaxIdx = f;

    int treeSize = 1;
    while (treeSize < treeMaxIdx)
    {
        treeSize *= 2;
    }
    treeSize *= 2;

    tree.resize(treeSize);
    posToNode.resize(treeMaxIdx);

    initTreeImpl(0, 0, treeMaxIdx);
}

std::pair<int, int> queryTree(int ql)
{
    int curr = 0;
    int cl = 0;
    int cr = treeMaxIdx;

    std::pair<int, int> ans = {m, 1};

    while (cl < ql)
    {
        int left = curr * 2 + 1;
        int right = curr * 2 + 2;
        int cm = (cl + cr) / 2;

        if (cm <= ql)
        {
            curr = right;
            cl = cm;
        }
        else
        {
            ans = std::min(ans, tree[right]);
            curr = left;
            cr = cm;
        }
    }

    ans = std::min(ans, tree[curr]);

    return {ans.first, -ans.second};
}

void updateTree(int pos)
{
    int curr = posToNode[pos];

    tree[curr].first++;

    while (curr != 0)
    {
        curr = (curr - 1) / 2;
        int left = curr * 2 + 1;
        int right = curr * 2 + 2;
        tree[curr] = std::min(tree[left], tree[right]);
    }
}

void testF(int f)
{
    long long s = 0;

    initTree(f);

    for (int time : reqTimes)
    {
        auto [minCount, minT] = queryTree(time);

        if (minCount == m || minT == -1) return;
    
        s += gpuCosts[minCount];
        updateTree(minT);
    }

    __int128 currAns = (__int128) f * s;

    ans = std::min(ans, currAns);

    // std::clog << f << " * " << s << " = " << currAns << "\n";
}

void solve()
{
    ans = (__int128) 4 * n * n * n;

    std::reverse(reqTimes.begin(), reqTimes.end());

    int maxTime = reqTimes.front();

    for (int f = maxTime + 1; f <= maxTime + n; f++)
    {
        testF(f);
    }
}

__int128 solveGpus(std::vector<int>& _gpuCosts, std::vector<int>& _reqTimes)
{
    m = _gpuCosts.size();
    n = _reqTimes.size();

    gpuCosts = std::move(_gpuCosts);
    reqTimes = std::move(_reqTimes);

    solve();

    return ans;
}
