#include <iostream>
#include <algorithm>
#include <vector>

#include "gpus.h"

int n, m;

std::vector<int> gpuCosts;
std::vector<int> reqTimes;

__int128 ans;

std::vector<int> reqTimeCounts;

__int128 testF(int f)
{
    long long s = 0;

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
            s += gpuCosts[i];
            constraint--;
        }
    }

    if (done < n) return -1;

    __int128 currAns = (__int128) f * s;

    ans = std::min(ans, currAns);

    // std::clog << f << " * " << s << " = " << currAns << "\n";

    return currAns;
}

void solve()
{
    ans = (__int128) 4 * n * n * n;

    int maxTime = reqTimes.back();

    reqTimeCounts.clear();
    reqTimeCounts.resize(maxTime + n, 0);
    for (int time : reqTimes)
    {
        reqTimeCounts[time]++;
    }

    int l = maxTime + 1;
    int r = maxTime + n + 11;

    testF(l);

    while (r - l > 1)
    {
        int ml = (2 * l + r + 1) / 3;
        int mr = (l + 2 * r) / 3;

        __int128 aml = testF(ml);

        if (aml == -1)
        {
            l = ml;
            continue;
        }

        __int128 amr = testF(mr);

        if (aml < amr) r = mr;
        else l = ml;
    }

    int range = 100;

    for (int i = std::max(maxTime + 1, l - range); i <= std::min(maxTime + n, l + range); ++i)
    {
        testF(i);
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
