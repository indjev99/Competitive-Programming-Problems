#include "vision.h"
#include <cassert>

std::vector<int> getVisionPattern1d()
{
    return {2, 1, 1, 2, 2, 1};
}

int getMove1d(std::vector<int> v)
{
    int n = v.size() / 2;
    int me = v[n];
    if (me == 1)
    {
        if (v[0] == 2) return 0;
        else return 2;
    }

    if (v[n - 1] == 1 && v[n + 1] == 1)
    {
        if (v[n + 2] == 1) return n + 2;
        else return n - 2;
    }

    if (v[n - 2] == 2)
    {
        return n - 2;
    }

    if (v[n + 2] == 2)
    {
        return n + 2;
    }

    if (v[n - 1] == 2)
    {
        return n - 1;
    }

    assert(v[n + 1] == 2);
    return n + 1;
}

std::vector<std::vector<int>> getVisionPattern2d()
{
    return {{2, 1, 1, 2, 1, 1},
            {1, 1, 1, 1, 2, 1},
            {1, 1, 1, 1, 1, 1},
            {2, 1, 1, 2, 1, 1},
            {1, 1, 1, 1, 2, 1},
            {1, 1, 1, 1, 1, 1}};
}

std::pair<int, int> getMove2d(std::vector<std::vector<int>> v)
{
    int n = v.size() / 2;
    int m = v[0].size() / 2;
    int me = v[n][m];

    if (me == 1)
    {
        for (int i = 0 ; i <= 2 * n ; ++i)
        {
            for (int j = 0 ; j <= 2 * m ; ++j)
            {
                if (v[i][j] != 1)
                {
                    return {i, j};
                }
            }
        }

        assert(false);
    }

    if (v[n - 1][m - 1] == 2 || v[n - 1][m + 1] == 2 || v[n + 1][m - 1] == 2 || v[n + 1][m + 1] == 2)
    {
        int cnt2 = 0;
        for (int i = 0 ; i <= 2 * n ; ++i)
        {
            for (int j = 0 ; j <= 2 * m ; ++j)
            {
                if (v[i][j] == 2)
                {
                    cnt2++;
                }
            }
        }

        if (cnt2 == 3) // upper two
        {
            for (int rotR = 0 ; rotR < 2 ; ++rotR)
            {
                for (int rotC = 0 ; rotC < 2 ; ++rotC)
                {
                    int dX = (rotR == 0 ? 1 : -1);
                    int dY = (rotC == 0 ? 1 : -1);
                    if (v[n + dX][m + dY] == 2)
                    {
                        return {n + 2 * dX, m + 2 * dY};
                    }
                }
            }

            assert(false);
        } else
        {
            assert(cnt2 == 5);
            for (int rotR = 0 ; rotR < 2 ; ++rotR)
            {
                for (int rotC = 0 ; rotC < 2 ; ++rotC)
                {
                    int dX = (rotR == 0 ? 1 : -1);
                    int dY = (rotC == 0 ? 1 : -1);
                    if (v[n - dX][m - dY] == 2)
                    {
                        return {n + 2 * dX, m + 2 * dY};
                    }
                }
            }

            assert(false);
        }
        
        assert(false);
    }

    for (int rotR = 0 ; rotR < 2 ; ++rotR)
    {
        for (int rotC = 0 ; rotC < 2 ; ++rotC)
        {
            int dX = (rotR == 0 ? 1 : -1);
            int dY = (rotC == 0 ? 1 : -1);
            if (v[n - 2 * dX][m - 2 * dY] == 2)
            {
                return {n + 2 * dX, m + 2 * dY};
            }
        }
    }

    assert(false);
}