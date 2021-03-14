#include <numeric>
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

    for (int period = 1; period < n; ++period)
    {
        for (int i = 2 * period - 1; i < n; i += period)
        {
            if (period < periods[i] && hasPeriod(i + 1, period))
            {
                periods[i] = period;
            }
            else break;
        }
    }

    return periods;
}