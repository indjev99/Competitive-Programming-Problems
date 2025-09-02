#include "manager.h"

#include <iostream>
#include <algorithm>
#include <random>

constexpr int NUM_ENCS = 5e3;
constexpr int NUM_ELEMS = 3;

constexpr int ENCODE = 0;
constexpr int DECODE = 1;

constexpr int NUM_PROCS = 2;

const std::string PASSWORD_CHECKER = "djk398fjk29skg05sjdfud";
const std::string PASSWORD_GRADER_AUTH = "A&0k4s+h";
const std::string PASSWORD_MANAGER_AUTH = "_d&f92Kx";

char** fds;

void finish()
{
    for (int i = 0; i < 2 * NUM_PROCS; i++)
    {
        close(atoi(fds[i]));
    }

    exit(0);
}

void myAssert(bool cond, const std::string& message)
{
    if (cond) return;

    std::cout << PASSWORD_CHECKER << std::endl;
    std::cout << -1 << std::endl;
    std::cout << message << std::endl;

    finish();
}

template <typename InType>
void checkGraderAuthAndAlive(InType& in)
{
    std::string pass;
    in >> pass;

    myAssert(pass == PASSWORD_GRADER_AUTH, "Your program used stdin or stdout or exited unexpectedly.");
}

std::vector<int> callEncode(int a)
{
    int proc = ENCODE;

    out(proc) << PASSWORD_MANAGER_AUTH << "\n";
    out(proc) << a << std::endl;

    checkGraderAuthAndAlive(in(proc));

    int tripSz;
    in(proc) >> tripSz;
    std::vector<int> trip(tripSz);
    for (int& elem : trip)
    {
        in(proc) >> elem;
    }

    return trip;
}

int callDecode(int x, int y)
{
    int proc = DECODE;

    out(proc) << PASSWORD_MANAGER_AUTH << "\n";
    out(proc) << x << " " << y << std::endl;

    checkGraderAuthAndAlive(in(proc));

    int b;
    in(proc) >> b;

    return b;
}

int main(int argc, char* argv[])
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    fds = argv + 1;
    fd_to_stream(fds, 2 * NUM_PROCS);

    int m;
    std::cin >> m;

    int seed;
    std::cin >> seed;

    std::mt19937 generator(seed);

    int n;

    for (int i = 0; i < NUM_PROCS; i++)
    {
        out(i) << PASSWORD_MANAGER_AUTH << "\n";
        out(i) << m << std::endl;
        out(i) << i << std::endl;

        checkGraderAuthAndAlive(in(i));

        int currN;
        in(i) >> currN;

        if (i == 0) n = currN;
        else myAssert(currN == n, "Players did not return the same N in setup.");
    }

    myAssert(n >= 1, "Invalid N.");

    std::vector<int> as;
    std::uniform_int_distribution<int> distr(0, n - 1);
    for (int i = 0; i < NUM_ENCS; i++)
    {
        as.push_back(distr(generator));
    }

    std::vector<std::vector<int>> trips;
    for (int i = 0; i < NUM_ENCS; i++)
    {
        trips.push_back(callEncode(as[i]));
    }

    for (int i = 0; i < NUM_ENCS; i++)
    {
        myAssert((int) trips[i].size() == NUM_ELEMS, "Invalid number of encoding elements.");
        for (int elem : trips[i])
        {
            myAssert(elem >= 0 && elem < m, "Invalid encoding element.");
        }
    }

    std::vector<int> bs;
    for (int i = 0; i < NUM_ENCS; i++)
    {
        std::shuffle(trips[i].begin(), trips[i].end(), generator);
        bs.push_back(callDecode(trips[i][0], trips[i][1]));
    }

    for (int i = 0; i < NUM_ENCS; i++)
    {
        myAssert(bs[i] == as[i], "Incorrect number decoded.");
    }

    std::cout << PASSWORD_CHECKER << std::endl;
    std::cout << n << std::endl;

    finish();

    return 0;
};
