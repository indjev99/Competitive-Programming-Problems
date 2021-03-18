#include <numeric>
#include <assert.h>
#include <algorithm>
#include "periods.h"

std::vector<int> findPeriods(int n)
{
    std::vector<int> periods(n);
    std::iota(periods.begin(), periods.end(), 1);

    int mid = (n + 1) / 2;
    if (hasPeriod(mid, 1))
    {
        std::fill(periods.begin(), periods.begin() + mid, 1);
        for (int i = mid; i < n; ++i)
        {
            if (hasPeriod(i + 1, 1)) 
            {
                periods[i] = 1;
            }
        }

        return periods;
    }

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