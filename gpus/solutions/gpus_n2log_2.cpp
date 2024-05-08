#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

#include "gpus.h"

int n, m;

std::vector<int> gpuCosts;
std::vector<int> reqTimes;

__int128 ans;

void testF(int f)
{
    long long s = 0;

    std::vector<int> doneTimeCounts(f, 0);

    std::priority_queue<std::pair<int, int>> pq;

    int lastTime = f;
    for (int time : reqTimes)
    {
        while (lastTime > time)
        {
            --lastTime;
            pq.push({-doneTimeCounts[lastTime], lastTime});
        }

        int minCount = m;
        int minT = -1;

        while (!pq.empty())
        {
            auto [newMinCount, newMinT] = pq.top();
            pq.pop();

            newMinCount = -newMinCount;

            if (newMinCount != doneTimeCounts[newMinT]) continue;

            minCount = newMinCount;
            minT = newMinT;

            break;
        }

        if (minCount == m || minT == -1) return;

        s += gpuCosts[minCount];
        doneTimeCounts[minT]++;

        if (doneTimeCounts[minT] < m)
        {
            pq.push({-doneTimeCounts[minT], minT});
        }
    }

    __int128 currAns = (__int128) f * s;

    ans = std::min(ans, currAns);

    // std::clog << f << " * " << s << " = " << currAns << "\n";
}

void solve()
{
    ans = (__int128) 4 * n * n * n;

    std::reverse(reqTimes.begin(), reqTimes.end());

    int maxTime = reqTimes.front();

    for (int f = maxTime + 1; f <= maxTime + n; f++)
    {
        testF(f);
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
