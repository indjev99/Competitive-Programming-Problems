#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>

const int NUM_ELEMS = 3;

std::vector<std::vector<int>> trips;
std::vector<std::vector<int>> revs;

void genTrips(int m)
{
    trips.clear();

    for (int offset = 0; 2 * offset < m; offset = 2 * offset + 1)
    {
        for (int i = 0; i + 2 * offset < m; i++)
        {
            if (offset > 0 && i / offset % 3 != 0) continue;
            trips.push_back({i, i + offset, i + 2 * offset});
        }
    }
}

void genRevs(int m)
{
    for (auto& trip : trips)
    {
        for (int i = 0; i < NUM_ELEMS; i++)
        {
            assert(trip[i] >= 0 && trip[i] < m);
        }
    }

    revs.assign(m, std::vector<int>(m, -1));

    for (int k = 0; k < (int) trips.size(); k++)
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

    return trips.size();
}

std::vector<int> encode(int a)
{
    return trips[a];
}

int decode(int x, int y)
{
    return revs[x][y];
}
