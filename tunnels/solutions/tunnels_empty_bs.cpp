#include "tunnels.h"

void solve(int n, int m, int k, const std::vector<std::vector<bool>>& blocked)
{
    int x = 0;
    int y = k;

    while (x < n)
    {
        int min = y;
        while (min > 0 && !blocked[x][min - 1]) min--;

        int max = y;
        while (max < m - 1 && !blocked[x][max + 1]) max++;

        while (min < max)
        {
            int mid = (max + min) / 2;

            if (mid >= y)
            {
                if (!investigate(mid + 1)) max = mid;
                else min = mid + 1;
            }
            else
            {
                if (!investigate(mid)) min = mid + 1;
                else max = mid;
            }
        }

        x++;
        y = min;

        goDeeper(y);
    }
}
