#include "triangle.h"
#include <queue>
#include <vector>
#include <random>
#include <cassert>
#include <iostream>
#include <algorithm>

const int MAXN = 1e3 + 10;
std::vector <int> g[MAXN];
std::vector <int> answer;
int ptr;

void dfs(int node, int par)
{
    answer[node] = ptr++;
    for (const int &u : g[node])
    {
        if (u != par)
        {
            dfs(u, node);
        }
    }
}

std::vector <int> solve(int n)
{
    ptr = 1;
    answer.clear();
    for (int i = 0 ; i < n ; ++i)
    {
        g[i].clear();
    }
    
    int two = -1;
    std::vector <int> nums;
    for (int i = 0 ; i < n && two == -1 ; ++i)
    {
        int cnt = 0;
        for (int j = 0 ; j < n && cnt < 3 ; ++j)
        {
            if (j != i && query(i, i, j))
            {
                cnt++;
            }
        }

        if (cnt == 2)
        {
            two = i;
        }
    }
    
    assert(two != -1);
    for (int i = 0 ; i < n ; ++i)
    {
        for (int j = i + 1 ; j < n ; ++j)
        {
            if (query(two, i, j))
            {
                g[i].push_back(j);
                g[j].push_back(i);
            }
        }
    }
    
    int start;
    if (g[g[two][0]].size() == 1) start = g[two][0];
    else start = g[two][1];
    answer.resize(n);
    dfs(start, -1);
    return answer;
}