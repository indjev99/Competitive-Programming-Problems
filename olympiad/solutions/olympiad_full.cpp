#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <stack>

typedef unsigned int uint;

const uint MAX_ST = 75e3;
const uint MAX_NODES = MAX_ST * 2 + 2;

#define STUDENT 0
#define LANGUAGE 1
#define MUSIC 2

struct Edge
{
    uint num;
    uint type;
    uint to;
    bool cap;
    uint revInd;
};

uint SOURCE;
uint SINK;

std::vector<Edge> es[MAX_NODES];
uint numNodes;

uint k;
std::vector<uint> as, bs, al, bl, am, bm;

void input()
{
    uint n, s, t, l, m;
    std::cin >> n >> s >> t;

    for (uint i = 0; i < n; ++i)
    {
        std::cin >> l >> m;
        m += s;
        es[l].push_back({i + 1, STUDENT, m, true, (uint) es[m].size()});
        es[m].push_back({i + 1, STUDENT, l, false, (uint) es[l].size() - 1});
    }

    SOURCE = 0;
    SINK = s + t + 1;
    numNodes = 1;

    for (uint i = 0; i < s; ++i)
    {
        es[SOURCE].push_back({i + 1, LANGUAGE, numNodes, true, (uint) es[numNodes].size()});
        es[numNodes].push_back({i + 1, LANGUAGE, SOURCE, false, (uint) es[SOURCE].size() - 1});
        ++numNodes;
    }

    for (uint i = 0; i < t; ++i)
    {
        es[numNodes].push_back({i + 1, MUSIC, SINK, true, (uint) es[SINK].size()});
        es[SINK].push_back({i + 1, MUSIC, numNodes, false, (uint) es[numNodes].size() - 1});
        ++numNodes;
    }

    ++numNodes;
}

void printAll(std::vector<uint>& vec)
{
    std::sort(vec.begin(), vec.end());
    for (uint x : vec)
    {
        std::cout << x << " ";
    }
    std::cout << "\n";
}

void output()
{
    std::cout << k;
    std::cout << " " << as.size() << " " << bs.size();
    std::cout << " " << al.size() << " " << bl.size();
    std::cout << " " << am.size() << " " << bm.size();
    std::cout << "\n";
    printAll(as);
    printAll(bs);
    printAll(al);
    printAll(bl);
    printAll(am);
    printAll(bm);
}

bool vis[MAX_NODES];

uint level[MAX_NODES];
std::queue<uint> q;

//BFs for max flow
bool mfBFS()
{
    std::fill(vis, vis + numNodes, false);
    level[SOURCE] = 1;
    vis[SOURCE] = true;
    q.push(SOURCE);
    uint reachLevel = numNodes + 1;
    while (!q.empty())
    {
        uint curr = q.front();
        q.pop();
        if (curr == SINK) reachLevel = level[curr];
        uint nextLevel = level[curr] + 1;
        if (nextLevel >= reachLevel) continue;
        for (const Edge& e : es[curr])
        {
            if (e.cap && !vis[e.to])
            {
                level[e.to] = nextLevel;
                vis[e.to] = true;
                q.push(e.to);
            }
        }
    }
    return vis[SINK];
}

uint lastDone[MAX_NODES];

//DFS for max flow
bool mfDFS(uint curr)
{
    if (curr == SINK) return true;
    uint currLev = level[curr];
    for (uint i = lastDone[curr]; i < es[curr].size(); ++i)
    {
        Edge& e = es[curr][i];
        lastDone[curr] = i + 1;
        if (e.cap && level[e.to] == currLev + 1)
        {
            bool flow = mfDFS(e.to);
            if (flow)
            {
                e.cap = false;
                es[e.to][e.revInd].cap = true;
                return flow;
            }
        }
    }
    return false;
}

//finds the max flow and residue graph
void maxFlow()
{
    k = 0;
    while (mfBFS())
    {
        std::fill(lastDone, lastDone + numNodes, 0);
        while (mfDFS(SOURCE))
        {
            ++k;
        }
    }
}

std::stack<uint> st;

//DFS for building the stack
void sccDFS(uint curr)
{
    vis[curr] = true;
    for (const Edge& e : es[curr])
    {
        if (e.cap && !vis[e.to])
        {
            sccDFS(e.to);
        }
    }
    st.push(curr);
}

uint SCCs[MAX_NODES];

//DFS for assigning components
void sccDFS2(uint curr, uint currSCC)
{
    SCCs[curr] = currSCC;
    for (const Edge& e : es[curr])
    {
        if (!e.cap && !SCCs[e.to])
        {
            sccDFS2(e.to, currSCC);
        }
    }
}

//finds the SCCs
void findSCCs()
{
    std::fill(vis, vis + numNodes, false);
    for (uint i = 0; i < numNodes; ++i)
    {
        if (!vis[i]) sccDFS(i);
    }
    uint numSCCs = 1;
    while (!st.empty())
    {
        uint curr = st.top();
        st.pop();
        if (!SCCs[curr])
        {
            sccDFS2(curr, numSCCs++);
        }
    }
}

//finds the answer
void findABs()
{
    for (uint i = 0; i < numNodes; ++i)
    {
        for (const Edge& e : es[i])
        {
            if (e.cap && SCCs[i] != SCCs[e.to])
            {
                if (i > e.to && e.type == STUDENT) as.push_back(e.num);
                else if (i > e.to && e.type == LANGUAGE) al.push_back(e.num);
                else if (i > e.to && e.type == MUSIC) am.push_back(e.num);
                else if (i < e.to && e.type == STUDENT) bs.push_back(e.num);
                else if (i < e.to && e.type == LANGUAGE) bl.push_back(e.num);
                else if (i < e.to && e.type == MUSIC) bm.push_back(e.num);
            }
        }
    }
}

void solve()
{
    //find max flow
    maxFlow();

    //find the SCCs in the residual graph
    findSCCs();

    //find the answer
    findABs();
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    input();
    solve();
    output();

    return 0;
}
