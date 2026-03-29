#include <iostream>
#include <stdio.h>
#include <vector>
using namespace std;
typedef long long llong;

const int MAXN = 5111;

int n, m;
vector<pair<int, llong>> Graph[MAXN];
llong dist[MAXN];

vector<int> newGraph[MAXN];
int inEdges[MAXN];

int main()
{
    scanf("%d %d", &n, &m);

    for (int i = 1; i <= m; i++)
    {
        int a, b, c;

        scanf("%d %d %d", &a, &b, &c);

        Graph[a].push_back({b, c});
    }

    int iter;
    for (iter = 1; iter <= n + 1; iter++)
    {
        bool changes = false;
        for (int j = 0; j < n; j++)
        {
            for (auto [v, d] : Graph[j])
            {
                if (dist[j] + d < dist[v])
                {
                    changes = true;
                    dist[v] = dist[j] + d;
                }
            }
        }

        if (!changes)
            break;
    }

    if (iter > n)
    {
        printf("1\n");
        return 0;
    }

    for (int i = 0; i < n; i++)
    {
        for (auto [v, d] : Graph[i])
        {
            if (dist[i] + d == dist[v])
            {
                newGraph[i].push_back(v);
                inEdges[v]++;
            }
        }
    }

    vector<int> q;
    for (int i = 0; i < n; i++)
    {
        if (inEdges[i] == 0)
        {
            q.push_back(i);
        }
    }

    int visited = 0;
    while(!q.empty())
    {
        visited++;
        int ver = q.back();
        q.pop_back();

        for (auto v : newGraph[ver])
        {
            inEdges[v]--;

            if (inEdges[v] == 0)
            {
                q.push_back(v);
            }
        }
    }

    if (visited == n)
        printf("0\n");
    else
        printf("1\n");

    return 0;
}
