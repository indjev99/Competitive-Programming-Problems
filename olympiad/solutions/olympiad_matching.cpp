#include <iostream>
#include <algorithm>
#include <vector>
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

uint s, t;
std::vector<uint> ses[MAX_ST];

uint SOURCE;
uint SINK;

std::vector<Edge> es[MAX_NODES];
uint numNodes;

uint k;
std::vector<uint> as, bs, al, bl, am, bm;

void input()
{
    uint n, l, m;
    std::cin >> n >> s >> t;

    for (uint i = 0; i < n; ++i)
    {
        std::cin >> l >> m;
        ses[l - 1].push_back(m - 1);
        m += s;
        es[l].push_back({i + 1, STUDENT, m, true, (uint) es[m].size()});
        es[m].push_back({i + 1, STUDENT, l, true, (uint) es[l].size() - 1});
    }

    SOURCE = 0;
    SINK = s + t + 1;
    numNodes = 1;

    for (uint i = 0; i < s; ++i)
    {
        es[SOURCE].push_back({i + 1, LANGUAGE, numNodes, true, (uint) es[numNodes].size()});
        es[numNodes].push_back({i + 1, LANGUAGE, SOURCE, true, (uint) es[SOURCE].size() - 1});
        ++numNodes;
    }

    for (uint i = 0; i < t; ++i)
    {
        es[numNodes].push_back({i + 1, MUSIC, SINK, true, (uint) es[SINK].size()});
        es[SINK].push_back({i + 1, MUSIC, numNodes, true, (uint) es[numNodes].size() - 1});
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

uint NONE;
uint dirMatch[MAX_ST], revMatch[MAX_ST];

//DFS for max matching
bool mmDFS(uint curr)
{
    vis[curr] = true;
    for (uint next : ses[curr])
    {
        if (revMatch[next] == NONE)
        {
            dirMatch[curr] = next;
            revMatch[next] = curr;
            return true;
        }
    }
    for (uint next : ses[curr])
    {
        if (!vis[revMatch[next]] && mmDFS(revMatch[next]))
        {
            dirMatch[curr] = next;
            revMatch[next] = curr;
            return true;
        }
    }
    return false;
}

//finds the matching
void maxMatching()
{
    NONE = s + t;
    std::fill(dirMatch, dirMatch + s, NONE);
    std::fill(revMatch, revMatch + t, NONE);

    k = 0;
    bool updated = true;
    while (updated)
    {
        updated = false;
        std::fill(vis, vis + s, false);
        for (uint i = 0; i < s; ++i)
        {
            if (dirMatch[i] == NONE && !vis[i] && mmDFS(i))
            {
                ++k;
                updated = true;
            }
        }
    }
}

//finds the max flow residue graph from the matching
void findResidue()
{
    for (uint i = 0; i < numNodes; ++i)
    {
        for (Edge& e : es[i])
        {
            if (i < e.to && e.type == STUDENT) e.cap = dirMatch[i - 1] != e.to - s - 1;
            else if (i < e.to && e.type == LANGUAGE) e.cap = dirMatch[e.to - 1] == NONE;
            else if (i < e.to && e.type == MUSIC) e.cap = revMatch[i - s - 1] == NONE;
            else if (i > e.to && e.type == STUDENT) e.cap = revMatch[i - s - 1] == e.to - 1;
            else if (i > e.to && e.type == LANGUAGE) e.cap = dirMatch[i - 1] != NONE;
            else if (i > e.to && e.type == MUSIC) e.cap = revMatch[e.to - s - 1] != NONE;
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
    //find max matching
    maxMatching();

    //find the residue graph
    findResidue();

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
