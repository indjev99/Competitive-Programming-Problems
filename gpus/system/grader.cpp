#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <vector>
#include <chrono>
#include <string>

#include "gpus.h"

static std::vector<int> gpuCosts;
static std::vector<int> reqTimes;

static __int128 ans;

static int readNum()
{
    int pluses = 0;
    std::string str = "";

    while (true)
    {
        char c;

        std::cin >> c;

        if (c >= '0' && c <= '9') str += c;
        else if (c == '+') pluses++;

        if ((int) str.size() > pluses) break;
    }

    return std::stoi(str);
}

static void input()
{
    int n, m;
    std::cin >> n >> m;

    gpuCosts.resize(m);
    int last = 0;
    for (int& cost : gpuCosts)
    {
        cost = readNum();
        cost += last;
        cost = std::min(cost, 2 * n); // encho is dumb
        last = cost;
    }

    reqTimes.resize(n);
    last = 0;
    for (int& time : reqTimes)
    {
        time = readNum();
        time += last;
        last = time;
    }
}

static void output()
{
    std::cout << ans << std::endl;
}

int main(int argc, char** args)
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    double ioTimeSecs = 0;

    int numTests;
    std::cin >> numTests;

    for (int i = 0; i < numTests; i++)
    {
        auto ioStart = std::chrono::system_clock::now();

        input();

        auto ioEnd = std::chrono::system_clock::now();

        std::chrono::duration<double> ioTime = ioEnd - ioStart;

        ioTimeSecs += ioTime.count();

        ans = solveGpus(gpuCosts, reqTimes);

        output();
    }

    std::ofstream ioTimeFile(args[1]);

    ioTimeFile << "io-time:" << std::fixed << std::setprecision(3) << ioTimeSecs << std::endl;

    return 0;
}
