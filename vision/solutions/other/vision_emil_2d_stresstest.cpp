#include "vision.h"

#include <algorithm>

std::vector<int> getVisionPattern1d()
{
    return {1};
}

int getMove1d(std::vector<int> v)
{
    return 0;
}

const int N = 60;
const int T1 = N - 1;
const int T2 = N - 2;

std::vector<std::vector<int>> getVisionPattern2d()
{
    std::vector<std::vector<int>> pat(N, std::vector<int>(N, 0));
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            pat[i][j] = N;
            if (i == 0 && j == 0) pat[i][j] = T1;
            else if (i == N - 1 && j == N - 1) pat[i][j] = T2;
        }
    }
    return pat;
}

std::pair<int, int> getMove2d(std::vector<std::vector<int>> v)
{
    int n = v.size();
    int curr = (n - 1) / 2;

    if (curr == T1)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if ((i >= curr - 1 && i <= curr + 1) ||
                    (j >= curr - 1 && j <= curr + 1)) continue;

                if (v[i][j] == T2) return {i, j};
            }
        }

        return {-1, -1};
    }

    if (curr == T2)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (v[i][j] == T1) return {i, j};
            }
        }

        return {-1, -1};
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (v[i][j] == T1 || v[i][j] == T2) return {i, j};
        }
    }

    return {-1, -1};
}
