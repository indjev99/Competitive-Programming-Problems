#include <iostream>
#include <stdio.h>
#include <vector>
#include <queue>
#include <string.h>
#include <algorithm>
using namespace std;

int n;
int nA, nB, m;
int k = 0;
vector< pair<int, int> > Graph[150111];

priority_queue< pair<int, int> > pq;
int activeDegree[150111];
int novia[150111];
bool parsed[150111];

void strongPrematching()
{
    int i;

    for (i=1;i<=n;i++)
    {
        activeDegree[i] = (int)Graph[i].size();
        pq.push(make_pair(-activeDegree[i], i));
    }

    int iters = 0;
    while(!pq.empty())
    {
        iters++;

        int deg = -pq.top().first;
        int ver = pq.top().second;
        pq.pop();

        if (parsed[ver] || deg != activeDegree[ver])
            continue;

        parsed[ver] = true;

        ///Find matching
        int mate = -1;
        for (i=0;i<Graph[ver].size();i++)
        {
            if (parsed[ Graph[ver][i].first ])
                continue;

            if (mate == -1 || activeDegree[mate] > activeDegree[ Graph[ver][i].first ])
            {
                mate = Graph[ver][i].first;
            }
        }

        if (mate != -1)
        {
            parsed[mate] = true;

            novia[ver] = mate;
            novia[mate] = ver;

            k++;
        }

        parsed[ver] = true;
        for (i=0;i<Graph[ver].size();i++)
        {
            if (parsed[ Graph[ver][i].first ])
                continue;

            activeDegree[ Graph[ver][i].first ]--;
            pq.push(make_pair(-activeDegree[ Graph[ver][i].first ], Graph[ver][i].first));
        }
    }
}

int UTFO[150111];
int Key = 1000000000;

bool chaseMatch(int ver, int cf)
{
    if (UTFO[ver] >= Key)
        return false;

    UTFO[ver] = Key;

    if (ver > nA)
    {
        if (novia[ver] == 0 || chaseMatch(novia[ver], ver))
        {
            novia[ver] = cf;
            novia[cf] = ver;
            return true;
        }
        else
            return false;
    }
    else
    {
        for (int i = 0; i < Graph[ver].size(); i++)
        {
            if (chaseMatch(Graph[ver][i].first, ver))
                return true;
        }
        return false;
    }
}

bool TFO[150111];
bool goldenEdge[150111];

void finishMatching()
{
    int i, j;
    bool changed = true;

    while(changed)
    {
        changed = false;
        memset(UTFO,0,sizeof(UTFO));

        for (i=1;i<=nA;i++)
        {
            if (novia[i] == 0)
            {
                Key--;
                if (chaseMatch(i, 0))
                {
                    changed = true;

                    Key += 2;
                    k++;
                }
            }
        }
    }


    for (i=1;i<=n;i++)
    {
        for (j=0;j<Graph[i].size();j++)
        {
            if (Graph[i][j].first == novia[i])
            {
                goldenEdge[ Graph[i][j].second ] = true;
                break;
            }
        }
    }
}

bool edgeMarks[150111];
bool nodeMarks[150111];

void reach(int ver, int startSide)
{
    if (TFO[ver])
        return;
    TFO[ver] = true;

    bool forced = (ver <= nA && startSide == 1) || (ver > nA && startSide == 0);

    if (forced)
    {
        for (int i = 0; i < Graph[ver].size(); i++)
        {
            if (Graph[ver][i].first == novia[ver])
            {
                edgeMarks[ Graph[ver][i].second ] = true;
                break;
            }
        }

        reach(novia[ver], startSide);
    }
    else
    {
        nodeMarks[ver] = true;

        for (int i = 0; i < Graph[ver].size(); i++)
        {
            if (Graph[ver][i].first == novia[ver])
                continue;

            edgeMarks[ Graph[ver][i].second ] = true;

            reach(Graph[ver][i].first, startSide);
        }
    }
}

inline int side(int ver)
{
    if (ver <= nA)
        return 0;
    else
        return 1;
}

vector< pair<int,int> > dGraph[150111];
vector< pair<int,int> > rGraph[150111];
vector<int> outOrder;

void DFS(int ver)
{
    if (TFO[ver])
        return;

    TFO[ver] = true;

    int i;

    for (i=0;i<dGraph[ver].size();i++)
    {
        DFS(dGraph[ver][i].first);
    }

    outOrder.push_back(ver);
}

void cover(int ver)
{
    if (UTFO[ver] != 0)
        return;

    UTFO[ver] = Key;

    int i;

    for (i=0;i<rGraph[ver].size();i++)
    {
        if (UTFO[ rGraph[ver][i].first ] != 0)
        {
            if (UTFO[ rGraph[ver][i].first ] == Key)
                edgeMarks[ rGraph[ver][i].second ] = true;

            continue;
        }

        edgeMarks[ rGraph[ver][i].second ] = true;
        cover(rGraph[ver][i].first);
    }
}

void SCC()
{
    int i,j;

    for (i=1;i<=n;i++)
    {
        for (j=0;j<Graph[i].size();j++)
        {
            if (i <= nA && !goldenEdge[ Graph[i][j].second ])
            {
                dGraph[i].push_back(Graph[i][j]);
                rGraph[ Graph[i][j].first ].push_back(make_pair(i, Graph[i][j].second));
            }

            if (i > nA && goldenEdge[ Graph[i][j].second ])
            {
                dGraph[i].push_back(Graph[i][j]);
                rGraph[ Graph[i][j].first ].push_back(make_pair(i, Graph[i][j].second));
            }
        }
    }

    memset(TFO, false, sizeof(TFO));
    for (i=1;i<=n;i++)
    {
        if (!TFO[i])
        {
            DFS(i);
        }
    }

    reverse(outOrder.begin(), outOrder.end());

    memset(UTFO, 0, sizeof(UTFO));
    Key = 1;
    for (i=0;i<outOrder.size();i++)
    {
        if (UTFO[ outOrder[i] ] != Key)
        {
            Key++;
            cover(outOrder[i]);
        }
    }
}

void markReachables()
{
    int i,j;

    for (i=1;i<=nA;i++)
    {
        if (novia[i] == 0)
        {
            reach(i, 0);
        }
    }

    memset(TFO, false, sizeof(TFO));

    for (i=nA+1;i<=n;i++)
    {
        if (novia[i] == 0)
        {
            reach(i, 1);
        }
    }

    ///Left-to-right Loops
    SCC();
}

void printVector(vector<int> &V)
{
    for (int i = 0; i < V.size(); i++)
    {
        printf("%d ", V[i]);
    }
    printf("\n");
}

int main()
{
    int i, j;

    scanf("%d %d %d", &m, &nA, &nB);
    n = nA + nB;

    for (i=1;i<=m;i++)
    {
        int a, b;

        scanf("%d %d", &a, &b);

        Graph[a].push_back(make_pair(b + nA, i));
        Graph[b + nA].push_back(make_pair(a, i));
    }

    //strongPrematching();
    finishMatching();
    markReachables();

    vector<int> An, Bn, Al, Bl, Am, Bm;

    for (i=1;i<=m;i++)
    {
        if (goldenEdge[i] && !edgeMarks[i])
            An.push_back(i);

        if (!goldenEdge[i] && !edgeMarks[i])
            Bn.push_back(i);
    }

    for (i=1;i<=nA;i++)
    {
        if (novia[i] != 0 && !nodeMarks[i])
            Al.push_back(i);

        if (Graph[i].size() == 0)
            Bl.push_back(i);
    }

    for (i=nA+1;i<=n;i++)
    {
        if (novia[i] != 0 && !nodeMarks[i])
            Am.push_back(i-nA);

        if (Graph[i].size() == 0)
            Bm.push_back(i-nA);
    }

    printf("%d %d %d %d %d %d %d\n", k, An.size(), Bn.size(), Al.size(), Bl.size(), Am.size(), Bm.size());

    printVector(An);
    printVector(Bn);
    printVector(Al);
    printVector(Bl);
    printVector(Am);
    printVector(Bm);
}
