#include <vector>
#include <stdio.h>
#include <assert.h>
#include "prison.h"
using namespace std;

int baseM;
int mapping[4501][4501];
pair<int, int> inverseMapping[3500000];

void enumerate(int m, int& ctr)
{
    if (m < 2)
        return;

    if (m % 2 == 1)
    {
        enumerate(m - 1, ctr);
        return;
    }

    for (int i = m / 2; i < m; i++)
    {
        for (int j = i; j < m; j++)
        {
            inverseMapping[ctr] = {i, j};
            mapping[i][j] = ctr++;
        }
    }

    enumerate(m / 2, ctr);
}

int setup(int m)
{
    baseM = m;
    int ctr = 0;

    enumerate(m, ctr);

    return ctr;
}

vector<int> encode(int a)
{
    auto [x, y] = inverseMapping[a];

    int activeM = baseM;

    while( !(x >= activeM / 2 && y < activeM) )
    {
        if (activeM == 0)
            while(1);

        activeM /= 2;
        if (activeM % 2 == 1)
            activeM--;
    }

    int halfM = activeM / 2;
    return {x, y, (x - halfM + y - halfM) % halfM };
}

int decode(int x, int y)
{
    int activeM = baseM;

    while(x < activeM / 2 && y < activeM / 2)
    {
        activeM /= 2;
        if (activeM % 2 == 1)
            activeM--;
    }

    int halfM = activeM / 2;

    if (x < halfM)
    {
        x = (x - y + 5 * halfM) % halfM + halfM;
    }
    else if (y < halfM)
    {
        y = (y - x + 5 * halfM) % halfM + halfM;
    }

    if (x > y)
        swap(x, y);

    return mapping[x][y];
}

