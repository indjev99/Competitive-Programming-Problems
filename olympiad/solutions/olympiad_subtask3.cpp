#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

typedef unsigned int uint;

const uint MAX_ST = 75e3;
const uint MAX_NODES = MAX_ST * 2 + 2;

#define STUDENT 0
#define LANGUAGE 1
#define MUSIC 2

struct Edge
{
    uint to;
    bool cap;
    uint revInd;
};

struct Element
{
    uint num;
    uint type;
    bool alwaysPos;
    bool neverPos;
    bool confirmed;
    uint edgeI, edgeJ;
    uint from, to; //for students only
    Edge* edge;
};

std::vector<Element> elems;

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
        elems.push_back({i + 1, STUDENT, true, true, false, l, (uint) es[l].size(), n + l - 1, n + m - 1});
        es[l].push_back({m, true, (uint) es[m].size()});
        es[m].push_back({l, false, (uint) es[l].size() - 1});
    }

    SOURCE = 0;
    SINK = s + t + 1;
    numNodes = 1;

    for (uint i = 0; i < s; ++i)
    {
        elems.push_back({i + 1, LANGUAGE, true, true, false, SOURCE, (uint) es[SOURCE].size()});
        es[SOURCE].push_back({numNodes, true, (uint) es[numNodes].size()});
        es[numNodes].push_back({SOURCE, false, (uint) es[SOURCE].size() - 1});
        ++numNodes;
    }

    for (uint i = 0; i < t; ++i)
    {
        elems.push_back({i + 1, MUSIC, true, true, false, numNodes, (uint) es[numNodes].size()});
        es[numNodes].push_back({SINK, true, (uint) es[SINK].size()});
        es[SINK].push_back({numNodes, false, (uint) es[numNodes].size() - 1});
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
    uint reachLevel = numNodes + 10;
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
    return reachLevel < numNodes + 10;
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

//resets the capacities for the max flow
void resetCaps()
{
    for (uint i = 0; i < numNodes; ++i)
    {
        for (Edge& e : es[i])
        {
            e.cap = i < e.to;
        }
    }
}

//rules out invalid options based on current max flow
void gatherInfo()
{
    for (Element& elem : elems)
    {
        if (elem.edge->cap) elem.alwaysPos = false;
        else elem.neverPos = false;
    }
}

void solve()
{
    //link up pointers for convenience
    for (Element& elem : elems)
    {
        elem.edge = &es[elem.edgeI][elem.edgeJ];
    }

    //mark all nodes with no edges as impossible
    for (Element& elem : elems)
    {
        if (elem.type == STUDENT)
        {
            elems[elem.from].neverPos = false;
            elems[elem.to].neverPos = false;
        }
        else if (elem.neverPos)
        {
            elem.alwaysPos = false;
            elem.confirmed = true;
        }
    }

    //find max flow
    maxFlow();
    uint bestK = k;

    gatherInfo();

    //main part of the solution
    for (Element& elem : elems)
    {
        if (elem.confirmed) continue;

        //test if the element is always needed
        if (elem.alwaysPos)
        {
            //remove the edge and then check if that lowers the max flow
            resetCaps();
            elem.edge->cap = false;
            maxFlow();
            if (k < bestK) elem.neverPos = false;
            else
            {
                elem.alwaysPos = false;
                elem.edge->cap = true;
                gatherInfo();
            }
        }

        //test if the elements is never needed
        //note, this is always an edge

        //if an edge has a node that can be unused, then we can use it
        if (elem.neverPos && (!elems[elem.from].alwaysPos || !elems[elem.to].alwaysPos))
        {
            elem.neverPos = false;
        }

        //otherwise we need to do the full test
        if (elem.neverPos)
        {

            //force the edge in by removing it and its nodes
            //and then check if the flow + 1 is decreased
            resetCaps();
            elem.edge->cap = false;
            elems[elem.from].edge->cap = false;
            elems[elem.to].edge->cap = false;
            maxFlow();
            if (k + 1 < bestK) elem.alwaysPos = false;
            else
            {
                elem.neverPos = false;
                gatherInfo();
            }
        }

        //if an edge is always used then so are its nodes
        if (elem.type == STUDENT && elem.alwaysPos)
        {
            elems[elem.from].confirmed = true;
            elems[elem.to].confirmed = true;
        }

        elem.confirmed = true;
    }

    //collect the answers
    for (const Element& elem : elems)
    {
        if (elem.alwaysPos && elem.type == STUDENT) as.push_back(elem.num);
        else if (elem.alwaysPos && elem.type == LANGUAGE) al.push_back(elem.num);
        else if (elem.alwaysPos && elem.type == MUSIC) am.push_back(elem.num);
        else if (elem.neverPos && elem.type == STUDENT) bs.push_back(elem.num);
        else if (elem.neverPos && elem.type == LANGUAGE) bl.push_back(elem.num);
        else if (elem.neverPos && elem.type == MUSIC) bm.push_back(elem.num);
    }

    k = bestK;
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
