#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>

const int NUM_ELEMS = 3;

std::vector<std::vector<int>> trips;
std::vector<std::vector<int>> revs;

void genTripsUtil(int left, int right)
{
    int numVals = right - left;

    int buck = numVals / 2;

    if (buck == 0) return;

    for (int i = 0; i < buck; i++)
    {
        for (int j = 0; j < i; j++)
        {
            int k = (i + j) % buck;

            trips.push_back({i + left, j + left, k + buck + left});
        }
    }

    genTripsUtil(left + buck, right);
}

void genTrips(int m)
{
    trips.clear();

    for (int i = 0; i < m; i++)
    {
        trips.push_back({i, i, i});
    }

    genTripsUtil(0, m);
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
