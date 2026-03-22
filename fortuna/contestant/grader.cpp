#include "fortuna.h"

#include <iostream>
#include <random>

static void myAssert(bool cond, const char* msg)
{
    if (cond) return;

    std::cout << msg << std::endl;
    exit(0);
}

static const int MAX_N = 30;
static const int MAX_M = 30;
static const int MAX_CODES = 500;

static const int REPS = 1e6;
static const int MAX_COST = 100;

static int n;
static int m;

static bool vis[MAX_CODES];
static std::pair<bool, int> acts[MAX_CODES][MAX_N];

static void DFS(int curr)
{
    if (vis[curr]) return;
    vis[curr] = true;

    for (int r = 0; r < n; r++)
    {
        auto [done, next] = proc(curr, r);
        if (done) myAssert(next >= 0 && next < m, "Invalid output number");
        else myAssert(next >= 0 && next < MAX_CODES, "Invalid next value");
        acts[curr][r] = {done, next};

        if (!done) DFS(next);
    }
}

static std::mt19937 gen(0);

static int totalCost;

static int genNumber()
{
    std::uniform_int_distribution<int> distr(0, n - 1);

    int c = 0;
    while (true)
    {
        totalCost++;

        myAssert(totalCost <= REPS * MAX_COST, "Too many iterations");

        int r = distr(gen);

        auto [done, next] = acts[c][r];

        if (done) return next;

        c = next;
    }
}

int main()
{
    std::cin >> n >> m;

    setup(n, m);

    DFS(0);

    std::vector<double> cnts(m, 0);

    for (int t = 0; t < REPS; t++)
    {
        cnts[genNumber()]++;
    }

    std::cout << "Average iterations:" << std::endl;
    std::cout << (double) totalCost / REPS << std::endl;

    std::cout << "Counts:" << std::endl;
    for (int i = 0; i < m; i++)
    {
        std::cout << i << ": " << cnts[i] << std::endl;
    }

    return 0;
}
