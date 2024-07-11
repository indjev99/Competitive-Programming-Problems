#include <numeric>
#include "periods.h"

std::vector<int> findPeriods(int n)
{
    std::vector<int> periods(n);
    std::iota(periods.begin(), periods.end(), 1);

    for (int period = 1; period < n; ++period)
    {
        if (periods[period - 1] < period) continue;
        for (int i = 2 * period - 1; i < n; i += period)
        {
            if (hasPeriod(i + 1, period))
            {
                periods[i] = period;
            }
            else break;
        }
    }

    return periods;
}
