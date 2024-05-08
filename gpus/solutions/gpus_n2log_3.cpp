#include <iostream>
#include <algorithm>
#include <vector>

#include "gpus.h"

int n, m;

std::vector<int> gpuCosts;
std::vector<int> reqTimes;

__int128 ans;

void testF(int f)
{
    long long s = 0;

    std::vector<int> doneTimeCounts(f, 0);

    for (int time : reqTimes)
    {
        int minCount = doneTimeCounts[time];

        if (minCount == m) return;

        int left = time;
        int right = f;

        while (right - left > 1)
        {
            int mid = (left + right) / 2;
            if (doneTimeCounts[mid] == minCount) left = mid;
            else right = mid;
        }

        int minT = left;

        s += gpuCosts[minCount];
        doneTimeCounts[minT]++;
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
