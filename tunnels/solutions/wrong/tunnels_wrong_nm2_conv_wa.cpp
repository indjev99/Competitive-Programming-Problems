#include "tunnels.h"

void solve(int n, int m, int k, const std::vector<std::vector<bool>>& walls)
{
    std::vector<std::vector<int>> dpCost(
        n + 1, std::vector<int>(m, -1)
    );

    for (int y = 0; y < m; y++)
    {
        dpCost[n][y] = 0;
    }

    std::vector<std::vector<std::vector<std::pair<int, int>>>> dpMid(
        n, std::vector<std::vector<std::pair<int, int>>>(m)
    );

    for (int x = 0; x < n; x++)
    {
        for (int y = 0; y < m; y++)
        {
            dpMid[x][y].resize(m - y);
        }
    }

    auto solveRow = [&](int x, int minY, int maxY)
    {
        for (int l = maxY; l >= minY; l--)
        {
            dpMid[x][l][0] = {dpCost[x + 1][l], l};

            for (int r = l + 1; r <= maxY; r++)
            {
                if (walls[x][r]) break;

                if (dpCost[x + 1][l] == -1)
                {
                    dpMid[x][l][r - l] = dpMid[x][l + 1][r - (l + 1)];
                    continue;
                }

                if (dpCost[x + 1][r] == -1)
                {
                    dpMid[x][l][r - l] = dpMid[x][l][(r - 1) - l];
                    continue;
                }

                int bestMid = dpMid[x][l][(r - 1) - l].second;
                int bestCost = std::max(
                    dpMid[x][l][bestMid - l].first,
                    dpMid[x][bestMid + 1][r - (bestMid + 1)].first
                ) + 1;

                for (int mid = bestMid + 1; mid < r; mid++)
                {
                    int lc = dpMid[x][l][mid - l].first;
                    int rc = dpMid[x][mid + 1][r - (mid + 1)].first;

                    int cost = std::max(lc, rc) + 1;

                    if (cost >= bestCost) break;

                    bestCost = cost;
                    bestMid = mid;
                }

                dpMid[x][l][r - l] = {bestCost, bestMid};
            }
        }

        int cost = dpMid[x][minY][maxY - minY].first;
        for (int y = minY; y <= maxY; y++)
        {
            dpCost[x][y] = cost;
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
            while (dpCost[x + 1][min] == -1) min++;
            while (dpCost[x + 1][max] == -1) max--;

            if (min == max) break;

            int mid = dpMid[x][min][max - min].second;

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
