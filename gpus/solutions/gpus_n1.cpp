#include <iostream>
#include <algorithm>
#include <vector>

#include "gpus.h"

int n, m;

std::vector<int> gpuCosts;
std::vector<int> reqTimes;

__int128 ans;

void countSort(std::vector<int>& vals)
{
    std::vector<int> cnts(2 * n, 0);
    for (int val : vals)
    {
        cnts[val]++;
    }

    vals.clear();
    for (int val = 0; val < 2 * n; val++)
    {
        for (int i = 0; i < cnts[val]; i++)
        {
            vals.push_back(val);
        }
    }
}

std::vector<int> reqTimeCounts;

int maxTime;
int maxGpus;
std::vector<int> usedGpuCounts;
std::vector<int> donePerGpus;
std::vector<long long> sPerGpus;

void initBuild()
{
    long long s = 0;

    int f = maxTime + 1;

    usedGpuCounts.clear();
    donePerGpus.clear();
    sPerGpus.clear();
    usedGpuCounts.resize(m, 0);
    donePerGpus.reserve(m + 1);
    sPerGpus.reserve(m + 1);

    maxGpus = 0;
    donePerGpus.push_back(0);
    sPerGpus.push_back(0);

    std::vector<int> doneTimeCounts(f, 0);

    int done = 0;

    for (int i = 0; i < m && done < n; i++)
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

        maxGpus = i + 1;
        donePerGpus.push_back(done);
        sPerGpus.push_back(s);
    }
}

void testGpus(int gpus, long long cumCost)
{
    int done = donePerGpus[gpus];
    long long baseS = sPerGpus[gpus];

    int baseRem = n - done;

    int slack = gpus < maxGpus ? std::max(usedGpuCounts[gpus] - 1, 0) : 0;
    int lTime = std::max(0, (baseRem - slack + gpus) / (gpus + 1));
    int rTime = baseRem / gpus;

    for (int time = lTime; time <= rTime; time++)
    {
        int f = maxTime + time + 1;
        long long s = baseS;
        int rem = baseRem;

        rem -= time * gpus;
        s += time * cumCost;

        if (gpus == m && rem > 0) continue;
        if (rem > 0) s += rem * gpuCosts[gpus];

        __int128 currAns = (__int128) f * s;

        ans = std::min(ans, currAns);

        // std::clog << f << " * " << s << " = " << currAns << "\n";
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

    initBuild();

    long long cumCost = 0;
    for (int gpus = 1; gpus <= maxGpus; gpus++)
    {
        cumCost += gpuCosts[gpus - 1];
        testGpus(gpus, cumCost);
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
