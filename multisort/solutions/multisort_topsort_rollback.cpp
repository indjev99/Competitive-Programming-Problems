#include "multisort.h"
#include <algorithm>
#include <numeric>
#include <stack>

std::vector<int> compTemp;

bool cmpByAsking(int a, int b)
{
    compTemp = {a, b};
    compare(compTemp);
    return a == compTemp[0];
}

std::vector<int> dummysort(int n, int k)
{
    std::vector<int> res(n);
    std::iota(res.begin(), res.end(), 0);
    std::sort(res.begin(), res.end(), cmpByAsking);
    return res;
}

#define SAVE 0
#define ENQUEUE 1
#define DEQUEUE 2
#define DEG_DEC 3

struct Change
{
    int type, arg;
    Change(int type=0, int arg=0):
        type(type),
        arg(arg) {}
};

std::vector<int> degree;
std::vector<std::vector<int>> edges;

int doCompare()
{
    compare(compTemp);
    for (int i = 0; i < compTemp.size() - 1; ++i)
    {
        if (compTemp[i + 1] == compTemp[i]) continue;
        edges[compTemp[i]].push_back(compTemp[i + 1]);
        ++degree[compTemp[i + 1]];
    }
    int minNode = compTemp[0];
    compTemp.clear();
    return minNode;
}

std::vector<int> topsort(int n, int k)
{
    int qPos = 0;
    std::vector<int> q;
    std::stack<Change> history;
    std::vector<int> res;

    int minNode = 0;
    for (int i = 1; i < n; ++i)
    {
        compTemp.push_back(i);
        if (compTemp.size() == k - 1 || (i == n - 1 && !compTemp.empty()))
        {
            compTemp.push_back(minNode);
            minNode = doCompare();
        }
    }

    q.push_back(minNode);
    while (true)
    {
        history.push(Change(SAVE));
        bool ambiguous = false;
        while (qPos < q.size() && compTemp.size() < k)
        {
            if (!ambiguous && q.size() <= qPos + 1) history.push(Change(SAVE));
            else ambiguous = true;
            int curr = q[qPos++];
            if (!ambiguous) res.push_back(curr);
            else compTemp.push_back(curr);
            history.push(Change(DEQUEUE));
            for (int i = 0; i < edges[curr].size() && q.size() + compTemp.size() < qPos + k; ++i)
            {
                int other = edges[curr][i];
                --degree[other];
                history.push(Change(DEG_DEC, other));
                if (degree[other] == 0)
                {
                    q.push_back(other);
                    history.push(Change(ENQUEUE));
                }
            }
        }
        if (!ambiguous) break;
        doCompare();
        Change change;
        do
        {
            change = history.top();
            if (change.type == SAVE) res.pop_back();
            else if (change.type == ENQUEUE) q.pop_back();
            else if (change.type == DEQUEUE) --qPos;
            else if (change.type == DEG_DEC) ++degree[change.arg];
            history.pop();
        }
        while (change.type != SAVE);
    }
    return res;
}

std::vector<int> multisort(int n, int k)
{
    compTemp.clear();
    degree.resize(n);
    edges.resize(n);
    std::fill(degree.begin(), degree.end(), 0);
    std::fill(edges.begin(), edges.end(), std::vector<int>());
    if (k > 2) return topsort(n, k);
    else return dummysort(n, k);
}
