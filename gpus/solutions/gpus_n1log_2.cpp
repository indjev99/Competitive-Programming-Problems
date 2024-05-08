#include <iostream>
#include <algorithm>
#include <vector>

#include "gpus.h"

int n, m;

std::vector<int> gpuCosts;
std::vector<int> reqTimes;

__int128 ans;

std::vector<int> reqTimeCounts;

int maxTime;
int f;
long long s;
std::vector<int> usedGpuCounts;

void initBuild()
{
    f = maxTime + 1;
    s = 0;

    int f = maxTime + 1;

    usedGpuCounts.clear();
    usedGpuCounts.resize(n, 0);

    std::vector<int> doneTimeCounts(f, 0);

    int done = 0;

    for (int i = 0; done < n; i++)
    {
        int constraint = n;
        int freeBefore = n - done;
        for (int t = f - 1; t >= 0 && done < n; t--)
        {
            constraint = std::min(constraint, freeBefore);
            if (constraint == 0) break;

            freeBefore += doneTimeCounts[t] - reqTimeCounts[t];
            done++;
            doneTimeCounts[t]++;
            usedGpuCounts[i]++;
            s += gpuCosts[i];
            constraint--;
        }
    }

    while (usedGpuCounts.back() == 0)
    {
        usedGpuCounts.pop_back();
    }
}

void advanceF()
{
    if ((int) usedGpuCounts.size() <= m)
    {
        __int128 currAns = (__int128) f * s;

        ans = std::min(ans, currAns);

        // std::clog << f << " * " << s << " = " << currAns << "\n";
    }

    f++;

    for (int i = 0; i < (int) usedGpuCounts.size(); i++)
    {
        s += gpuCosts[i];
        usedGpuCounts[i]++;
        s -= gpuCosts[usedGpuCounts.size() - 1];
        usedGpuCounts.back()--;
        if (usedGpuCounts.back() == 0)
        {
            usedGpuCounts.pop_back();
        }
    }
}

void solve()
{
    ans = (__int128) 4 * n * n * n;

    maxTime = reqTimes.back();

    reqTimeCounts.clear();
    reqTimeCounts.resize(maxTime + 1, 0);
    for (int time : reqTimes)
    {
        reqTimeCounts[time]++;
    }

    gpuCosts.resize(n, 0);

    initBuild();

    while (f <= maxTime + n)
    {
        advanceF();
    }
}

__int128 solveGpus(std::vector<int>& _gpuCosts, std::vector<int>& _reqTimes)
{
    m = _gpuCosts.size();
    n = _reqTimes.size();

    gpuCosts = std::move(_gpuCosts);
    reqTimes = std::move(_reqTimes);

    solve();

    return ans;
}
