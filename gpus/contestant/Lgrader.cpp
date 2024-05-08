#include <iostream>
#include <vector>

#include "gpus.h"

static std::vector<int> gpuCosts;
static std::vector<int> reqTimes;

static __int128 ans;

static void input()
{
    int n, m;
    std::cin >> n >> m;

    gpuCosts.resize(m);
    for (int& cost : gpuCosts)
    {
        std::cin >> cost;
    }

    reqTimes.resize(n);
    for (int& time : reqTimes)
    {
        std::cin >> time;
    }
}

static void output()
{
    std::cout << ans << std::endl;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int numTests;
    std::cin >> numTests;

    for (int i = 0; i < numTests; i++)
    {
        input();
        ans = solveGpus(gpuCosts, reqTimes);
        output();
    }

    return 0;
}
