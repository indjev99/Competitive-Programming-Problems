#include "multisort.h"
#include <algorithm>
#include <numeric>
#include <queue>

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

std::vector<int> degree;
std::vector<std::vector<int>> edges;
std::vector<int> covered;

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
    std::queue<int> q;
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

    while (true)
    {
        res.clear();
        std::fill(covered.begin(), covered.end(), 0);
        q = std::queue<int>();
        q.push(minNode);
        bool ambiguous = false;
        while (!q.empty() && compTemp.size() < k)
        {
            if (q.size() > 1) ambiguous = true;
            int curr = q.front();
            q.pop();
            if (!ambiguous) res.push_back(curr);
            else compTemp.push_back(curr);
            for (int i = 0; i < edges[curr].size() && q.size() + compTemp.size() < k; ++i)
            {
                int other = edges[curr][i];
                if (++covered[other] == degree[other]) q.push(other);
            }
        }
        if (!ambiguous) break;
        doCompare();
    }
    return res;
}

std::vector<int> multisort(int n, int k)
{
    compTemp.clear();
    degree.resize(n);
    edges.resize(n);
    covered.resize(n);
    std::fill(degree.begin(), degree.end(), 0);
    std::fill(edges.begin(), edges.end(), std::vector<int>());
    if (k > 40) return topsort(n, k);
    else return dummysort(n, k);
}
