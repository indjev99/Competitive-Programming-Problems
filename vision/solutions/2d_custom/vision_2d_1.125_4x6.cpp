#include "vision.h"

std::vector<int> getVisionPattern1d()
{
    return {2, 2, 1, 1, 2, 1};
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
        bool adj2 = v[1] == 2 || v[3] == 2;
        if (adj2)
        {
            if (v[1] == 1 && v[0] == 1) return 0;
            if (v[3] == 1 && v[4] == 1) return 4;
            if (v[1] == 2) return 1;
            if (v[3] == 2) return 3;
            return -1;
        }
        else
        {
            if (v[0] == 2) return 0;
            if (v[4] == 2) return 4;
            return -1;
        }
    }

    return -1;
}

std::vector<std::vector<int>> getVisionPattern2d()
{
    return {
        {2, 1, 1, 1, 1, 1},
        {1, 2, 1, 1, 1, 1},
        {1, 1, 1, 2, 1, 1},
        {1, 1, 1, 1, 1, 1}
    };
}

std::pair<int, int> getMove2d(std::vector<std::vector<int>> v)
{
    int curr = (v.size() - 1) / 2;

    if (curr == 1)
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (v[i][j] != 1) return {i, j};
            }
        }
        return {0, 0};
    }

    if (curr == 2)
    {
        std::pair<int, int> NONE = {-1, -1};

        std::pair<int, int> close = NONE;
        if (v[1][1] == 2) close = {1, 1};
        if (v[1][3] == 2) close = {1, 3};
        if (v[3][1] == 2) close = {3, 1};
        if (v[3][3] == 2) close = {3, 3};

        std::pair<int, int> far = NONE;
        if (v[1][0] == 2) far = {1, 0};
        if (v[1][4] == 2) far = {1, 4};
        if (v[3][0] == 2) far = {3, 0};
        if (v[3][4] == 2) far = {3, 4};

        if (close != NONE && far != NONE) return far;
        if (close != NONE && far == NONE) return close;
        if (close == NONE && far != NONE) return {4 - far.first, 4 - far.second};

        return {-1, -1};
    }

    return {-1, -1};
}
