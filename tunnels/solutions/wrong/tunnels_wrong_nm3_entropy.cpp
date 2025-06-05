#include "tunnels.h"

#include <cmath>

void solve(int n, int m, int k, const std::vector<std::vector<bool>>& walls)
{
    std::vector<std::vector<double>> dpEntropy(
        n + 1, std::vector<double>(m, -1)
    );

    for (int y = 0; y < m; y++)
    {
        dpEntropy[n][y] = 0;
    }

    std::vector<std::vector<std::vector<std::pair<double, int>>>> dpMid(
        n, std::vector<std::vector<std::pair<double, int>>>(
            m, std::vector<std::pair<double, int>>(m)
        )
    );

    auto solveRow = [&](int x, int minY, int maxY)
    {
        for (int l = maxY; l >= minY; l--)
        {
            dpMid[x][l][l] = {dpEntropy[x + 1][l], l};

            for (int r = l + 1; r <= maxY; r++)
            {
                if (walls[x][r]) break;

                if (dpEntropy[x + 1][l] < 0)
                {
                    dpMid[x][l][r] = dpMid[x][l + 1][r];
                    continue;
                }

                if (dpEntropy[x + 1][r] < 0)
                {
                    dpMid[x][l][r] = dpMid[x][l][r - 1];
                    continue;
                }

                int bestMid = -1;
                double bestSplit = n * m + 1;

                for (int mid = l; mid < r; mid++)
                {
                    double le = dpMid[x][l][mid].first;
                    double re = dpMid[x][mid + 1][r].first;

                    double split = std::max(le, re);

                    if (split < bestSplit)
                    {
                        bestSplit = split;
                        bestMid = mid;
                    }
                }

                double le = dpMid[x][l][bestMid].first;
                double re = dpMid[x][bestMid + 1][r].first;

                double minE = std::min(le, re);
                double maxE = std::max(le, re);

                double entropy = maxE + std::log(1.0 + std::exp(minE - maxE));

                dpMid[x][l][r] = {entropy, bestMid};
            }
        }

        double entropy = dpMid[x][minY][maxY].first;
        for (int y = minY; y <= maxY; y++)
        {
            dpEntropy[x][y] = entropy;
        }
    };

    for (int x = n - 1; x >= 0; x--)
    {
        int startY = -1;
        for (int endY = 0; endY <= m; endY++)
        {
            if (endY == m || walls[x][endY])
            {
                if (startY + 1 < endY)
                {
                    solveRow(x, startY + 1, endY - 1);
                }
                startY = endY;
            }
        }
    }

    int x = 0;
    int y = k;

    while (x < n)
    {
        int min = y;
        while (min > 0 && !walls[x][min - 1]) min--;

        int max = y;
        while (max < m - 1 && !walls[x][max + 1]) max++;

        while (min < max)
        {
            while (dpEntropy[x + 1][min] < 0) min++;
            while (dpEntropy[x + 1][max] < 0) max--;

            if (min == max) break;

            int mid = dpMid[x][min][max].second;

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
