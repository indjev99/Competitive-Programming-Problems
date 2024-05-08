#include <iostream>
#include <algorithm>
#include <vector>

#include "gpus.h"

int n, m;

std::vector<int> gpuCosts;
std::vector<int> reqTimes;

__int128 ans;

std::vector<int> reqTimeCounts;

int maxTime;
int f;
std::vector<int> usedGpuCounts;
int numUsedGpus;

void initBuild()
{
    f = maxTime + 1;

    int f = maxTime + 1;

    usedGpuCounts.clear();
    usedGpuCounts.resize(n, 0);
    numUsedGpus = 0;

    std::vector<int> doneTimeCounts(f, 0);

    int done = 0;

    for (int i = 0; done < n; i++)
    {
        int constraint = n;
        int freeBefore = n - done;
        for (int t = f - 1; t >= 0 && done < n; t--)
        {
            constraint = std::min(constraint, freeBefore);
            if (constraint == 0) break;

            freeBefore += doneTimeCounts[t] - reqTimeCounts[t];
            done++;
            doneTimeCounts[t]++;
            usedGpuCounts[i]++;
            constraint--;
        }
    }

    for (; numUsedGpus < n; numUsedGpus++)
    {
        if (usedGpuCounts[numUsedGpus] == 0) break;
    }
}

struct Node
{
    int lazy;
    int count;
    long long cost;
    long long cc;
};

int treeRoot;
int treeUpb;
std::vector<Node> tree;

void initTreeImpl(int curr, int cl, int cr)
{
    tree[curr].lazy = 0;

    if (cr - cl == 1)
    {
        tree[curr].count = usedGpuCounts[cl];
        tree[curr].cost = gpuCosts[cl];
        tree[curr].cc = (long long) gpuCosts[cl] * usedGpuCounts[cl];
        return;
    }

    int left = curr * 2 + 1;
    int right = curr * 2 + 2;
    int cm = (cl + cr) / 2;

    initTreeImpl(left, cl, cm);
    initTreeImpl(right, cm, cr);

    tree[curr].count = tree[left].count + tree[right].count;
    tree[curr].cost = tree[left].cost + tree[right].cost;
    tree[curr].cc = tree[left].cc + tree[right].cc;
}

void initTree()
{
    int treeSize = 1;
    while (treeSize < n)
    {
        treeSize *= 2;
    }
    treeSize *= 2;

    tree.resize(treeSize);

    treeRoot = 0;
    treeUpb = n;

    initTreeImpl(treeRoot, 0, treeUpb);
}

void fixLazy(int curr, int cl, int cr)
{
    if (tree[curr].lazy == 0) return;

    int left = curr * 2 + 1;
    int right = curr * 2 + 2;
    int cm = (cl + cr) / 2;

    tree[left].lazy += tree[curr].lazy;
    tree[left].count += (cm - cl) * tree[curr].lazy;
    tree[left].cc += tree[left].cost * tree[curr].lazy;

    tree[right].lazy += tree[curr].lazy;
    tree[right].count += (cr - cm) * tree[curr].lazy;
    tree[right].cc += tree[right].cost * tree[curr].lazy;

    tree[curr].lazy = 0;
}

Node queryTreeImpl(int curr, int cl, int cr, int ql, int qr)
{
    if (cr <= ql || cl >= qr) return {0, 0, 0, 0};

    if (cl >= ql && cr <= qr) return tree[curr];

    fixLazy(curr, cl, cr);

    int left = curr * 2 + 1;
    int right = curr * 2 + 2;
    int cm = (cl + cr) / 2;

    Node l = queryTreeImpl(left, cl, cm, ql, qr);
    Node r = queryTreeImpl(right, cm, cr, ql, qr);

    return {0, l.count + r.count, l.cost + r.cost, l.cc + r.cc};
}

Node queryTree(int ql, int qr)
{
    return queryTreeImpl(treeRoot, 0, treeUpb, ql, qr);
}

void updateTreeImpl(int curr, int cl, int cr, int ul, int ur, int val)
{
    if (cr <= ul || cl >= ur) return;

    if (cl >= ul && cr <= ur)
    {
        tree[curr].lazy += val;
        tree[curr].count += (cr - cl) * val;
        tree[curr].cc += tree[curr].cost * val;
        return;
    }

    fixLazy(curr, cl, cr);

    int left = curr * 2 + 1;
    int right = curr * 2 + 2;
    int cm = (cl + cr) / 2;

    updateTreeImpl(left, cl, cm, ul, ur, val);
    updateTreeImpl(right, cm, cr, ul, ur, val);

    tree[curr].count = tree[left].count + tree[right].count;
    tree[curr].cc = tree[left].cc + tree[right].cc;
}

void updateTree(int ul, int ur, int val)
{
    updateTreeImpl(treeRoot, 0, treeUpb, ul, ur, val);
}

std::pair<int, int> searchQueryTreeImpl(int curr, int cl, int cr, int ql, int qr, int countRight)
{
    if (cr <= ql || cl >= qr) return {-1, 0};

    if (cl >= ql && cr <= qr)
    {
        if (countRight >= cr) return {cr, -1};
        if (countRight + tree[curr].count < cl) return {-1, tree[curr].count};
        if (cr - cl == 1) return {cl, -1};
    }

    fixLazy(curr, cl, cr);

    int left = curr * 2 + 1;
    int right = curr * 2 + 2;
    int cm = (cl + cr) / 2;

    auto [ri, rc] = searchQueryTreeImpl(right, cm, cr, ql, qr, countRight);

    if (ri != -1) return {ri, -1};

    auto [li, lc] = searchQueryTreeImpl(left, cl, cm, ql, qr, countRight + rc);

    if (li != -1) return {li, -1};

    return {-1, lc + rc};
}

int searchQueryTree(int ql, int qr)
{
    return searchQueryTreeImpl(treeRoot, 0, treeUpb, ql, qr, 0).first;
}

void advanceF()
{
    if ((int) numUsedGpus <= m)
    {
        long long s = queryTree(0, numUsedGpus).cc;

        __int128 currAns = (__int128) f * s;

        ans = std::min(ans, currAns);

        // std::clog << f << " * " << s << " = " << currAns << "\n";
    }

    f++;

    int toMove = searchQueryTree(0, numUsedGpus);

    if (toMove == -1) exit(-1);

    if (toMove == 0) return;

    int inSuff = queryTree(toMove, numUsedGpus).count;

    int atLast = queryTree(toMove, toMove + 1).count;

    updateTree(0, toMove, 1);

    int afterLast = inSuff - atLast;
    int toMoveFromLast = toMove - afterLast;

    if (toMoveFromLast < atLast)
    {
        updateTree(toMove, toMove + 1, -toMoveFromLast);
        numUsedGpus = toMove + 1;
    }
    else numUsedGpus = toMove;

    while (numUsedGpus <= treeUpb / 2)
    {
        treeRoot = treeRoot * 2 + 1;
        treeUpb /= 2;
    }
}

void solve()
{
    ans = (__int128) 4 * n * n * n;

    maxTime = reqTimes.back();

    reqTimeCounts.clear();
    reqTimeCounts.resize(maxTime + 1, 0);
    for (int time : reqTimes)
    {
        reqTimeCounts[time]++;
    }

    gpuCosts.resize(n, 0);

    initBuild();

    initTree();

    while (f <= maxTime + n)
    {
        advanceF();
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
