#include "vision.h"

#include <algorithm>

std::vector<int> getVisionPattern1d()
{
    return {3, 1, 1, 2};
}

int getMove1d(std::vector<int> v)
{
    int curr = (v.size() - 1) / 2;

    if (curr == 1)
    {
        if (v[0] != 1) return 0;
        if (v[2] != 1) return 2;
        return -1;
    }

    if (curr == 2)
    {
        if (v[1] == 3) return 1;
        if (v[3] == 3) return 3;
        return -1;
    }

    if (curr == 3)
    {
        if (v[0] == 2) return 0;
        if (v[6] == 2) return 6;
        return -1;
    }

    return -1;
}

std::vector<std::vector<int>> getVisionPattern2d()
{
    return {
        {3, 1, 1, 1},
        {1, 1, 1, 1},
        {1, 1, 1, 1},
        {1, 1, 1, 2},
    };
}

std::pair<int, int> getMove2d(std::vector<std::vector<int>> v)
{
    int curr = (v.size() - 1) / 2;

    if (curr == 1)
    {
        for (int i = 0; i <= 2; i++)
        {
            for (int j = 0; j <= 2; j++)
            {
                if (v[i][j] != 1) return {i, j};
            }
        }
        return {0, 1};
    }

    if (curr == 2)
    {
        if (v[1][1] == 3) return {1, 1};
        if (v[1][3] == 3) return {1, 3};
        if (v[3][1] == 3) return {3, 1};
        if (v[3][3] == 3) return {3, 3};
        return {-1, -1};
    }

    if (curr == 3)
    {
        if (v[0][0] == 2) return {0, 0};
        if (v[0][6] == 2) return {0, 6};
        if (v[6][0] == 2) return {6, 0};
        if (v[6][6] == 2) return {6, 6};
        return {-1, -1};
    }

    return {-1, -1};
}
