#include <iostream>
#include <stdio.h>
#include <vector>
#include <assert.h>
#include <random>
#include <string.h>
#include "puzzle.h"
using namespace std;

const int MAXN = 100;
const vector<int> mvx = {-1, 0, 1, 0};
const vector<int> mvy = {0, 1, 0, -1};
const vector<vector<int>> mv = {
    {-1, 0}, {0, 1}, {1, 0}, {0, -1}
};

struct Node
{
    int req = 0;
    vector<int> nbNode;
    vector<int> nbEdge;

};

struct Edge
{
    int a, b;
    vector<int>* edgeCrosses;
    int uses = 2;
    int taken = 0;
};

vector<int> globalCtrs = {0, 0, 0};

struct State
{
    vector<Node> nodes;
    vector<Edge> edges;

    vector<int> father;
    vector<int> freeSize;
    int components = 0;

    vector<int> infq;
    vector<bool> queued;

    bool isBad = false;
    bool isDone = false;

    inline bool terminal()
    {
        return isBad || isDone;
    }

    inline void q(int v)
    {
        if (queued.empty())
        {
            queued.resize(nodes.size(), false);
        }
        if (!queued[v])
        {
            queued[v] = true;
            infq.push_back(v);
        }
    }

    void doFullInferences()
    {
        if (queued.empty())
            queued.resize(nodes.size(), false);
        for (int i = 0; i < nodes.size(); i++)
        {
            queued[i] = false;
            if (nodes[i].req > 0)
            {
                infq.push_back(i);
                queued[i] = true;
            }
        }

        doInferences();
    }

    void doInferences()
    {
        globalCtrs[0]++;

        while(!infq.empty())
        {
            if (terminal())
            {
                infq.clear();
                return;
            }

            int v = infq.back();
            infq.pop_back();
            queued[v] = false;

            assert(nodes[v].req >= 0);
            if (nodes[v].req == 0)
                continue;

            auto& nbNodes = nodes[v].nbNode;
            auto& nbEdges = nodes[v].nbEdge;

            int nbReq = 0;
            for (int i = 0; i < nbNodes.size(); i++)
            {
                nbReq += min(nodes[ nbNodes[i] ].req, edges[ nbEdges[i] ].uses);
            }

            if (nbReq < nodes[v].req)
            {
                isBad = true;
                infq.clear();
                return;
            }
            else if (nbReq == nodes[v].req)
            {
                for (int i = 0; i < nbEdges.size(); i++)
                {
                    int usages = min(nodes[ nbNodes[i] ].req, edges[ nbEdges[i] ].uses);
                    for (int j = 0; j < usages; j++)
                    {
                        applyEdge(nbEdges[i], true);
                        if (terminal())
                        {
                            infq.clear();
                            return;
                        }
                    }
                }
            }
            else if (nbReq - 1 == nodes[v].req)
            {
                for (int i = 0; i < nbNodes.size(); i++)
                {
                    int uses = min(nodes[ nbNodes[i] ].req, edges[ nbEdges[i] ].uses);

                    if (nbReq - uses < nodes[v].req)
                    {
                        applyEdge(nbEdges[i], true);
                        break;
                    }
                }
            }
        }

        infq.clear();
        ///queued.clear();
        for (int i = 0; i < queued.size(); i++)
        {
            queued[i] = false;
        }
    }

    int Find(int ver)
    {
        if (father[ver] == -1)
            return ver;
        else
        {
            father[ver] = Find(father[ver]);
            return father[ver];
        }
    }

    void killEdge(int idx)
    {
        if (isBad)
            return;
        edges[idx].uses = 0;
        q(edges[idx].a);
        q(edges[idx].b);
        doInferences();
    }

    void qnbs(int v)
    {
        for (int nb : nodes[v].nbNode)
        {
            q(nb);
        }
    }

    void applyEdge(int idx, bool recursive=false)
    {
        if (isBad)
            return;
        assert(edges[idx].uses > 0);
        edges[idx].uses--;
        edges[idx].taken++;

        int a = edges[idx].a;
        int b = edges[idx].b;

        assert(nodes[a].req > 0);
        assert(nodes[b].req > 0);
        nodes[a].req--;
        nodes[b].req--;

        q(a);
        q(b);
        qnbs(a);
        qnbs(b);

        int r1 = Find(a);
        int r2 = Find(b);

        //cout << freeSize[r1] << " --> ";

        if (r1 != r2)
        {
            father[r2] = r1;
            freeSize[r1] += freeSize[r2];
            components--;
        }

        freeSize[r1] -= 2;

        //cout << freeSize[r1] << endl;

        if (freeSize[r1] == 0)
        {
            if (components == 1)
                isDone = true;
            else
                isBad = true;
            return;
        }

        for (int cx : *edges[idx].edgeCrosses)
        {
            if (edges[cx].taken > 0)
            {
                isBad = true;
                return;
            }

            //if (edges[cx].uses == 0)
            //    continue;

            edges[cx].uses = 0;
            q(edges[cx].a);
            q(edges[cx].b);
        }

        if (!recursive)
            doInferences();
    }
};
int n, m, k;
vector<vector<char>> res;
vector<vector<int>> crosses;
vector<pair<int, int>> nodePos;
int nodeIdGrid[MAXN][MAXN];

void cleanup()
{
    crosses.clear();
    memset(nodeIdGrid, -1, sizeof(nodeIdGrid));
    res.clear();
    nodePos.clear();

}

void buildBasics(State& state)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (nodeIdGrid[i][j] == -1)
                continue;
            for (int dir = 1; dir <= 2; dir++)
            {
                int x = i + mvx[dir];
                int y = j + mvy[dir];

                while(x >= 0 && x < n && y >= 0 && y < m)
                {
                    if (nodeIdGrid[x][y] != -1)
                    {
                        Edge e;
                        e.a = nodeIdGrid[i][j];
                        e.b = nodeIdGrid[x][y];
                        state.edges.push_back(e);

                        state.nodes[e.a].nbNode.push_back(e.b);
                        state.nodes[e.a].nbEdge.push_back(state.edges.size() - 1);
                        state.nodes[e.b].nbNode.push_back(e.a);
                        state.nodes[e.b].nbEdge.push_back(state.edges.size() - 1);
                        break;
                    }

                    x += mvx[dir];
                    y += mvy[dir];
                }
            }
        }
    }

    crosses.resize(state.edges.size());
    for (int i = 0; i < state.edges.size(); i++)
    {
        state.edges[i].edgeCrosses = &crosses[i];
    }

    vector<vector<int>> tempCross(n);
    for (int i = 0; i < n; i++)
    {
        tempCross[i].resize(m, -1);
    }

    for (int i = 0; i < state.edges.size(); i++)
    {
        auto [ax, ay] = nodePos[ state.edges[i].a ];
        auto [bx, by] = nodePos[ state.edges[i].b ];

        if (ax == bx)
        {
            for (int j = min(ay, by) + 1; j < max(ay, by); j++)
            {
                tempCross[ax][j] = i;
            }
        }
    }

    for (int i = 0; i < state.edges.size(); i++)
    {
        auto [ax, ay] = nodePos[ state.edges[i].a ];
        auto [bx, by] = nodePos[ state.edges[i].b ];

        if (ay == by)
        {
            for (int j = min(ax, bx) + 1; j < max(ax, bx); j++)
            {
                if (tempCross[j][ay] != -1)
                {
                    crosses[i].push_back(tempCross[j][ay]);
                    crosses[ tempCross[j][ay] ].push_back(i);
                }
            }
        }
    }
}

bool shortForce(State& state)
{
    bool progress = true;
    bool fakeProgress = true;
    while(progress)
    {
        progress = false;
        fakeProgress = false;

        for (int i = 0; i < state.edges.size(); i++)
        {
            if (state.edges[i].uses > 0)
            {
                int a = state.edges[i].a;
                int b = state.edges[i].b;

                if (state.nodes[a].req == 0 || state.nodes[b].req == 0)
                    continue;

                /// Try proving used
                State testState = state;
                testState.killEdge(i);
                if (testState.isBad)
                {
                    //cout << "Proved used" << endl;
                    state.applyEdge(i);
                    if (state.isBad)
                        return false;
                    progress = true;
                    continue;
                }


                /// Try proving not used
                testState = state;
                testState.applyEdge(i);
                if (testState.isBad)
                {
                    //cout << "Proved not used" << endl;
                    state.killEdge(i);
                    if (state.isBad)
                        return false;
                    progress = true;
                    continue;
                }
                else if (testState.edges[i].uses > 0 && testState.nodes[a].req > 0 && testState.nodes[b].req > 0)
                {
                    /// Try proving not double
                    testState.applyEdge(i);
                    if (testState.isBad)
                    {
                        state.edges[i].uses = 1;
                        state.q(state.edges[i].a);
                        state.q(state.edges[i].b);
                        state.doInferences();
                        if (state.isBad)
                            return false;
                        progress = true;
                        continue;
                    }
                }
            }
        }
    }

    return true;
}

int splits = 0;
int curSplits = 0;
int lim = 200;
mt19937 mt(1337);
bool batrak(State& state)
{
    if (!shortForce(state))
    {
        return false;
    }

    for (int tries = 0; tries <= 3; tries++)
    {
        if (state.isDone)
            return true;
        if (state.isBad)
            return false;

        vector<int> relevant;
        for (int i = 0; i < state.edges.size(); i++)
        {
            if (state.edges[i].uses > 0 && state.nodes[ state.edges[i].a ].req > 0 && state.nodes[ state.edges[i].b ].req > 0)
            {
                relevant.push_back(i);
            }
        }

        if (relevant.empty())
            return false;

        int chosen = relevant[ mt() % relevant.size() ];

        State branched = state;
        branched.applyEdge(chosen);

        splits++;
        curSplits++;

        ///cout << splits << " | " << lim << " | " << state.components << " " << state.freeSize[ state.Find(0) ] << endl;

        if (curSplits > lim)
            return false;

        if (!branched.isBad && batrak(branched))
        {
            //cout << "Worked great, happy" << endl;
            state = branched;
            return true;
        }
        else
        {
            if (curSplits > lim)
                return false;
            curSplits++;
            if (curSplits > lim)
                return false;

            state.edges[chosen].uses = 0;
            state.q( state.edges[chosen].a );
            state.q( state.edges[chosen].b );
            state.doInferences();

            if (state.isBad)
                return false;
            assert(!state.isBad);

            return batrak(state);
        }
    }

    return false;
}

void init(int numSubtests, double timeLimit)
{

}

bool solve(vector<vector<int>>& puzzle)
{
    cleanup();

    State state;

    k = 0;
    n = puzzle.size();
    res.resize(n);
    for (int i = 0; i < n; i++)
    {
        m = puzzle[i].size();
        res[i].resize(m, ' ');

        for (int j = 0; j < m; j++)
        {
            if (puzzle[i][j] != 0)
            {
                res[i][j] = puzzle[i][j] + '0';
                nodeIdGrid[i][j] = k;
                k++;
                Node node;
                node.req = puzzle[i][j];
                nodePos.push_back({i, j});
                state.nodes.push_back(node);
                state.father.push_back(-1);
                state.freeSize.push_back(node.req);
                state.components++;
            }
        }
    }

    buildBasics(state);
    state.doFullInferences();
    assert(!state.isBad);
    assert(shortForce(state));

    State origState = state;
    while(!batrak(state))
    {
        lim += 5;
        curSplits = 0;
        state = origState;
    }
    //cout << "Correct with " << curSplits << endl;
    assert(batrak(state));

    for (auto& edge : state.edges)
    {
        auto [x1, y1] = nodePos[ edge.a ];
        auto [x2, y2] = nodePos[ edge.b ];

        if (edge.taken == 0)
            continue;

        if (x1 == x2)
        {
            int val = HOR_SINGLE;
            char sym = '-';
            if (edge.taken == 2)
            {
                sym = '=';
                val = HOR_DOUBLE;
            }
            for (int i = min(y1, y2) + 1; i < max(y1, y2); i++)
            {
                res[x1][i] = sym;
                puzzle[x1][i] = val;
            }
        }
        else
        {
            int val = VERT_SINGLE;
            char sym = '|';
            if (edge.taken == 2)
            {
                sym = '$';
                val = VERT_DOUBLE;
            }
            for (int i = min(x1, x2) + 1; i < max(x1, x2); i++)
            {
                res[i][y1] = sym;
                puzzle[i][y1] = val;
            }
        }
    }

    return true;
}

/**
int main()
{
    //freopen("Hashi_Puzzles\\200\\Hs_24_200_75_05_005.has", "r", stdin);

    int n, m, x;
    cin >> n >> m >> x;

    n *= 2;
    m *= 2;
    vector<vector<char>> grid(n);
    for (int i = 0; i < n; i += 2)
    {
        grid[i].resize(m, '0');
        grid[i + 1].resize(m, '0');
        for (int j = 0; j < m; j += 2)
        {
            cin >> grid[i][j];
        }
    }

    vector<vector<char>> res = solvePuzzle(grid);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            cout << res[i][j];
        }
        cout << endl;
    }

    cout << "Splits: " << splits << endl;
    cout << "Ctrs:";
    for (auto x : globalCtrs)
    {
        cout << " " << x;
    }
    cout << endl;

    return 0;
}
**/
