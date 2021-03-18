#include <numeric>
#include <assert.h>
#include "periods.h"

std::vector<int> findPeriods(int n)
{
    std::vector<int> periods(n);
    std::iota(periods.begin(), periods.end(), 1);

    for (int i = 0; i < n; ++i)
    {
        int next = i + periods[i];
        if (next < n)
        {
            assert(periods[next] == next + 1);
            if (hasPeriod(next + 1, periods[i])) periods[next] = periods[i];
        }
    }

    return periods;
}