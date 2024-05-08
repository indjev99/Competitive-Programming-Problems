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

std::pair<int, int> initTreeImpl(int curr, int cl, int cr)
{
    if (cr - cl == 1) return tree[curr] = {0, -cl};

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

    initTreeImpl(0, 0, treeMaxIdx);
}

std::pair<int, int> queryTreeImpl(int curr, int cl, int cr, int ql, int qr)
{
    if (cr <= ql || cl >= qr) return {m, 1};

    if (cl >= ql && cr <= qr) return tree[curr];

    int left = curr * 2 + 1;
    int right = curr * 2 + 2;
    int cm = (cl + cr) / 2;

    return std::min(
        queryTreeImpl(left, cl, cm, ql, qr),
        queryTreeImpl(right, cm, cr, ql, qr)
    );
}

std::pair<int, int> queryTree(int ql)
{
    std::pair<int, int> ans = queryTreeImpl(0, 0, treeMaxIdx, ql, treeMaxIdx);

    return {ans.first, -ans.second};
}

std::pair<int, int> updateTreeImpl(int curr, int cl, int cr, int pos)
{
    if (cr <= pos || cl > pos) return tree[curr];

    if (cr - cl == 1)
    {
        tree[curr].first++;
        return tree[curr];
    }

    int left = curr * 2 + 1;
    int right = curr * 2 + 2;
    int cm = (cl + cr) / 2;

    return tree[curr] = std::min(
        updateTreeImpl(left, cl, cm, pos),
        updateTreeImpl(right, cm, cr, pos)
    );
}

void updateTree(int pos)
{
    updateTreeImpl(0, 0, treeMaxIdx, pos);
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
