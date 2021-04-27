#include <iostream>
#include <algorithm>
#include <vector>
#include <stack>

#define WHITE 0
#define GRAY 1
#define BLACK 2

const int INF = 1e9;

const int MAX_N = 5e5;
const int MAX_K = 3;

int n, k;
int r[MAX_K];
int pos[MAX_N];

bool ans;

void input()
{
    std::cin >> n >> k;
    for (int i = 0; i < k; ++i)
    {
        std::cin >> r[i];
    }
    for (int i = 0; i < n; ++i)
    {
        std::cin >> pos[i];
    }
}

void output()
{
    std::cout << ans << std::endl;
}

const int MAX_NODES = MAX_N * 2;

std::vector<int> edges[MAX_NODES];

void addImplication(int from, int to)
{
    edges[from].push_back(to);
    edges[to ^ 1].push_back(from ^ 1);
}

void resetEdges()
{
    for (int i = 0; i < 2 * n; ++i)
    {
        edges[i].clear();
    }
}

void createEdges()
{
    for (int i = 0; i < n; ++i)
    {
        for (int j = i + 1; j < n; ++j)
        {
            if (pos[j] - pos[i] < r[WHITE])
            {
                addImplication(2 * i, 2 * j ^ 1);
            }
            if (pos[j] - pos[i] < r[GRAY])
            {
                addImplication(2 * i ^ 1, 2 * j);
            }
            else break;
        }
    }
}

int currT;
int visT[MAX_NODES];
std::stack<int> exitStack;
int nodeSCC[MAX_NODES];

void dirDFS(int node)
{
    visT[node] = currT;
    for (int next : edges[node])
    {
        if (visT[next] < currT) dirDFS(next);
    }
    exitStack.push(node);
}

void revDFS(int node, int scc)
{
    visT[node] = currT;
    nodeSCC[node] = scc;
    for (int negNext : edges[node ^ 1])
    {
        int next = negNext ^ 1;
        if (visT[next] < currT) revDFS(next, scc);
    }
}

void findSCCs()
{
    ++currT;
    for (int i = 0; i < 2 * n; ++i)
    {
        if (visT[i] < currT) dirDFS(i);
    }

    ++currT;
    for (int i = 0; i < 2 * n; ++i)
    {
        int node = exitStack.top();
        exitStack.pop();
        if (visT[node] < currT) revDFS(node, node);
    }
}

bool checkConflicts()
{
    for (int i = 0; i < 2 * n; i += 2)
    {
        if (nodeSCC[i] == nodeSCC[i ^ 1]) return false;
    }
    return true;
}

bool check()
{
    resetEdges();
    createEdges();
    findSCCs();
    return checkConflicts();
}

void solve()
{
    std::sort(r, r + k);
    ans = check();
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int t;
    std::cin >> t;
    for (int i = 0; i < t; ++i)
    {
        input();
        solve();
        output();
    }

    return 0;
}
