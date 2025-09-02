#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>

const int NUM_ELEMS = 3;

const int MAX_M = 4500;
const int MAX_N = MAX_M + MAX_M * (MAX_M - 1) / 6;

int n = 0;
int trips[MAX_N][NUM_ELEMS];
int revs[MAX_M][MAX_M];

void genTrips(int m)
{
    for (int i = 0; i < m; i++)
    {
        trips[n][0] = i;
        trips[n][1] = i;
        trips[n][2] = i;

        n++;
    }

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < i; j++)
        {
            int k = ((i + 1) ^ (j + 1)) - 1;

            if (k >= j) continue;

            trips[n][0] = i;
            trips[n][1] = j;
            trips[n][2] = k;

            n++;
        }
    }
}

void genRevs(int m)
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < m; j++)
        {
            revs[i][j] = -1;
        }
    }

    for (int k = 0; k < n; k++)
    {
        auto& trip = trips[k];

        for (int i = 0; i < NUM_ELEMS; i++)
        {
            assert(trip[i] >= 0 && trip[i] < m);
        }
    }

    for (int k = 0; k < n; k++)
    {
        auto& trip = trips[k];

        for (int i = 0; i < NUM_ELEMS; i++)
        {
            for (int j = 0; j < NUM_ELEMS; j++)
            {
                if (i == j) continue;

                assert(revs[trip[i]][trip[j]] == -1);
            }
        }

        for (int i = 0; i < NUM_ELEMS; i++)
        {
            for (int j = 0; j < NUM_ELEMS; j++)
            {
                if (i == j) continue;

                revs[trip[i]][trip[j]] = k;
            }
        }
    }
}

int setup(int m)
{
    genTrips(m);
    genRevs(m);

    return n;
}

std::vector<int> encode(int a)
{
    return {trips[a][0], trips[a][1], trips[a][2]};
}

int decode(int x, int y)
{
    return revs[x][y];
}
