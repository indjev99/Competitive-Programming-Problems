#include "vision.h"
#include <algorithm>
#include <iostream>
#include <cassert>

std::vector<int> getVisionPattern1d()
{
    return {3, 1, 1, 1, 2, 1};
}

int getMove1d(std::vector<int> v)
{
    if (v.size() == 3)
    {
        int max = 0;
        for (int i = 1 ; i < v.size() ; ++i)
        {
            if (v[max] < v[i])
            {
                max = i;
            }
        }

        return max;
    }

    if (v.size() == 5)
    {
        for (int i = 0 ; i < v.size() ; ++i)
        {
            if (v[i] == 3)
            {
                return i;
            }
        }

        assert(false);
    }

    if (v[0] == 1 && v[1] == 1 && v[2] == 1)
    {
        return 0;
    }

    return 6;
}

std::vector<std::vector<int>> getVisionPattern2d()
{
    return {
        {1, 1, 1, 1, 1, 1},
        {1, 2, 1, 3, 1, 4},
        {1, 1, 1, 1, 1, 1},
        {1, 3, 1, 4, 1, 2},
        {1, 1, 1, 1, 1, 1},
        {1, 4, 1, 2, 1, 3}
    };
}

std::pair<int, int> getMove2d(std::vector<std::vector<int>> v)
{
    int next[5];
    next[2] = 3;
    next[3] = 4;
    next[4] = 2;

    int vis = v.size() / 2;
    int n = v.size();

    if (vis == 1)
    {
        int maxRow = 0;
        int maxCol = 0;
        for (int i = 0 ; i < n ; ++i)
        {
            for (int j = 0 ; j < n ; ++j)
            {
                if (v[i][j] > v[maxRow][maxCol])
                {
                    maxRow = i;
                    maxCol = j;
                }
            }
        }

        return {maxRow, maxCol};
    }

    int myRow = v.size() / 2;
    int myCol = v.size() / 2;
    int nextRow = myRow + 2;
    int nextCol = myCol + 2;
    if (v[myRow + 2][myCol] != next[v[myRow][myCol]]) nextRow = myRow - 2;
    if (v[myRow][myCol + 2] != next[v[myRow][myCol]]) nextCol = myCol - 2;
    return {nextRow, nextCol};
}
