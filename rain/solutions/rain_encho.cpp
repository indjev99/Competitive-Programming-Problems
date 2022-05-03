#include <iostream>
#include <stdio.h>
#include <vector>
#include <set>
using namespace std;

const int MAXN = 3000;
const int MAX_COMPS = MAXN * MAXN / 2;

int n, m, h, w;
char grid[MAXN + 1][MAXN + 1];

bool validCell(int x, int y)
{
    return x >= 1 && x <= n && y >= 1 && y <= m && grid[x][y] != '.';
}

bool TFO[MAXN + 1][MAXN + 1];

bool okayNextCell(int x, int y)
{
    return validCell(x, y) && !TFO[x][y];
}

int id = 0;
int mark[MAXN + 1][MAXN + 1];

void DFS(int x, int y)
{
    std::vector<std::pair<int, int>> stack;

    TFO[x][y] = true;
    mark[x][y] = id;

    stack.push_back({x, y});

    while (!stack.empty())
    {
        auto [x, y] = stack.back();
        stack.pop_back();

        if (okayNextCell(x+1, y))
        {
            TFO[x+1][y] = true;
            mark[x+1][y] = id;
            stack.push_back({x+1, y});
        }
        if (okayNextCell(x-1, y))
        {
            TFO[x-1][y] = true;
            mark[x-1][y] = id;
            stack.push_back({x-1, y});
        }
        if (okayNextCell(x, y+1))
        {
            TFO[x][y+1] = true;
            mark[x][y+1] = id;
            stack.push_back({x, y+1});
        }
        if (okayNextCell(x, y-1))
        {
            TFO[x][y-1] = true;
            mark[x][y-1] = id;
            stack.push_back({x, y-1});
        }
    }
}

vector< vector<int> > comps[MAX_COMPS];
int startRow[MAX_COMPS];
int minCol[MAX_COMPS], maxCol[MAX_COMPS];


int lazyRoll[MAXN + 2][MAXN + 2];

void unroll()
{
    int i, j;

    for (i=1;i<=n;i++)
    {
        for (j=1;j<=m;j++)
        {
            lazyRoll[i][j] += lazyRoll[i][j-1];
        }
    }

    for (i=1;i<=m;i++)
    {
        for (j=1;j<=n;j++)
        {
            lazyRoll[j][i] += lazyRoll[j-1][i];
        }
    }
}

int counters[MAXN + 1];
set<int> vals;
void initCounters(int sz)
{
    int i;

    for (i=0;i<=sz;i++)
    {
        counters[i] = 0;
    }
    vals.clear();
}

void addRec(int x, int y1, int y2, int val)
{
    y1 = max(1, y1);
    y2 = min(m, y2);

    if (y1 > y2)
        return;

    //printf("%d to [%d, %d] at row %d\n", val, y1, y2, x);

    lazyRoll[x][y1] += val;
    lazyRoll[x][y2+1] -= val;
}

void toggleSafetyIfApplicable(int x, int y, int prevy, int val)
{
    if (y - w + 1 <= prevy)
    {
        addRec(x, y - w + 1, prevy, val);
    }
}

void removeVal(int x, int y)
{
    addRec(x, y - w + 1, y, -1);

    auto it = vals.find(y);

    auto nxt = it;
    nxt++;

    if (it != vals.begin())
    {
        auto prv = it;
        prv--;

        toggleSafetyIfApplicable(x, y, (*prv), 1);
    }

    if (nxt != vals.end())
    {
        toggleSafetyIfApplicable(x, (*nxt), y, 1);

        if (it != vals.begin())
        {
            auto prv = it;
            prv--;

            toggleSafetyIfApplicable(x, (*nxt), (*prv), -1);
        }
    }

    vals.erase(it);
}

void addVal(int x, int y)
{
    addRec(x, y - w + 1, y, 1);

    auto it = vals.insert(y).first;

    int prevy = -1;
    if (it != vals.begin())
    {
        auto prv = it;
        prv--;
        prevy = (*prv);

        toggleSafetyIfApplicable(x, y, prevy, -1);
    }

    it++;
    if (it != vals.end())
    {
        int nxty = (*it);

        if (prevy != -1)
            toggleSafetyIfApplicable(x, nxty, prevy, 1);
        toggleSafetyIfApplicable(x, nxty, y, -1);
    }
}

void incVal(int cid, int x, int y)
{
    int cntr = y - minCol[cid];

    counters[cntr]++;
    if (counters[cntr] == 1)
        addVal(x, y);
}

void decVal(int cid, int x, int y)
{
    int cntr = y - minCol[cid];

    counters[cntr]--;
    if (counters[cntr] == 0)
        removeVal(x, y);
}

int main()
{
    int i, j, in;

    scanf("%d %d %d %d", &n, &m, &h, &w);

    for (i=1;i<=n;i++)
    {
        for (j=1;j<=m;j++)
        {
            char inp[2];
            scanf("%s", inp);

            grid[i][j] = inp[0];
        }
    }

    for (i=1;i<=n;i++)
    {
        for (j=1;j<=m;j++)
        {
            if (okayNextCell(i, j))
            {
                id++;
                minCol[id] = m + 1;
                maxCol[id] = 0;

                DFS(i, j);
            }
        }
    }

    for (i=1;i<=n;i++)
    {
        for (j=1;j<=m;j++)
        {
            int mrk = mark[i][j];

            if (mrk == 0)
                continue;

            if (startRow[mrk] == 0)
                startRow[mrk] = i;

            minCol[mrk] = min(minCol[mrk], j);
            maxCol[mrk] = max(maxCol[mrk], j);

            int relativeRow = i - startRow[mrk];

            if (comps[mrk].size() < relativeRow + 1)
                comps[mrk].push_back(vector<int>());

            comps[mrk][relativeRow].push_back(j);
        }
    }

    for (i=1;i<=id;i++)
    {
        initCounters(maxCol[i] - minCol[i] + 1);
        for (j=0;j<comps[i].size();j++)
        {
            int x = startRow[i] + j;

            for (in=0;in<comps[i][j].size();in++)
            {
                int y = comps[i][j][in];

                incVal(i, x, y);
            }

            if (j >= h)
            {
                for (in=0;in<comps[i][j-h].size();in++)
                {
                    int y = comps[i][j-h][in];

                    decVal(i, x, y);
                }
            }
        }

        int contJ = max(j, h);

        for (j=contJ;j-h<comps[i].size();j++)
        {
            int x = startRow[i] + j;

            for (in=0;in<comps[i][j-h].size();in++)
            {
                int y = comps[i][j-h][in];

                decVal(i, x, y);
            }
        }
    }

    unroll();

    int maxVal = 0;
    for (i=1;i<=n;i++)
    {
        for (j=1;j<=m;j++)
        {
            if (lazyRoll[i][j] > maxVal)
            {
                maxVal = lazyRoll[i][j];
            }
        }
    }

    printf("%d\n", maxVal);

    return 0;
}
