#include "prison.h"

#include <iostream>
#include <algorithm>
#include <cassert>
#include <random>

static constexpr int NUM_ENCS = 5e3;
static constexpr int NUM_ELEMS = 3;

static void myAssert(bool cond, const std::string& message)
{
    if (cond) return;

    std::cout << message << std::endl;
    exit(0);
}

int main()
{
    std::mt19937 generator(0);

    int m;
    std::cin >> m;

    int n = setup(m);
    std::cout << "N: " << n << std::endl;

    myAssert(n >= 1, "Invalid N: " + std::to_string(n));

    std::vector<int> as;
    std::uniform_int_distribution<int> distr(0, n - 1);
    for (int i = 0; i < NUM_ENCS; i++)
    {
        as.push_back(distr(generator));
    }

    std::vector<std::vector<int>> trips;
    for (int i = 0; i < NUM_ENCS; i++)
    {
        trips.push_back(encode(as[i]));
    }

    for (int i = 0; i < NUM_ENCS; i++)
    {
        std::string msg = "Encoded " + std::to_string(as[i]) + " as";
        for (int elem : trips[i])
        {
            msg += " " + std::to_string(elem);
        }

        myAssert((int) trips[i].size() == NUM_ELEMS, msg + ": Invalid number of encoding elements.");
        for (int elem : trips[i])
        {
            myAssert(elem >= 0 && elem < m, msg + ": Invalid encoding element.");
        }
    }

    std::vector<std::vector<int>> origTrips = trips;

    std::vector<int> bs;
    for (int i = 0; i < NUM_ENCS; i++)
    {
        std::shuffle(trips[i].begin(), trips[i].end(), generator);
        bs.push_back(decode(trips[i][0], trips[i][1]));
    }

    for (int i = 0; i < NUM_ENCS; i++)
    {
        std::string msg = "Encoded " + std::to_string(as[i]) + " as";
        for (int elem : origTrips[i])
        {
            msg += " " + std::to_string(elem);
        }
        msg += ", received " + std::to_string(trips[i][0]) + " " + std::to_string(trips[i][1]);
        msg += " and decoded as " + std::to_string(bs[i]);

        myAssert(bs[i] == as[i], msg + ": Incorrect number decoded.");
    }

    std::cout << "Success" << std::endl;

    return 0;
}
