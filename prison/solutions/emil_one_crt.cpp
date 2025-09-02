#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <cassert>

const int NUM_ELEMS = 3;

std::vector<std::vector<int>> trips;
std::vector<std::vector<int>> revs;

void genTripsUtil(int left, int right)
{
    int numVals = right - left;

    int buck = numVals / 3;

    if (buck == 0) return;

    bool found = false;
    int p1 = -1, p2 = -1, p3 = -1;
    for (int k = buck; k >= 0 && !found; k--)
    {
        for (int j = buck; j > k && !found; j--)
        {
            if (std::gcd(k, j) > 1) continue;
            for (int i = buck; i > j && !found; i--)
            {
                if (std::gcd(k, i) > 1) continue;
                if (std::gcd(j, i) > 1) continue;

                found = true;
                p1 = i;
                p2 = j;
                p3 = k;
            }
        }
    }

    if (!found) return;

    for (int i = 0; i < p2 * p3; i++)
    {
        int a = i % p1;
        int b = i % p2;
        int c = i % p3;

        trips.push_back({a + left, b + buck + left, c + 2 * buck + left});
    }
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
