#include "vision.h"

std::vector<int> getVisionPattern1d()
{
    return {4, 1, 1, 1, 2, 1, 2, 1, 3, 1, 1, 1, 2, 1, 2, 1};
}

int getMove1d(std::vector<int> v)
{
    int curr = (v.size() - 1) / 2;

    if (curr == 1)
    {
        if (v[0] != 1) return 0;
        if (v[2] != 1) return 2;
        return 0;
    }

    if (curr == 2)
    {
        if (v[0] == 4) return 0;
        if (v[4] == 4) return 4;
        if (v[0] == 3) return 0;
        if (v[4] == 3) return 4;
        if (v[0] == 2) return 0;
        if (v[4] == 2) return 4;
        return -1;
    }

    if (curr == 3)
    {
        if (v[1] == 2) return 6;
        if (v[5] == 2) return 0;
        return -1;
    }

    if (curr == 4)
    {
        if (v[2] == 1 && v[0] == 2) return 0;
        if (v[6] == 1 && v[8] == 2) return 8;
        return -1;
    }

    return -1;
}

std::vector<std::vector<int>> getVisionPattern2d()
{
    return {{1}};
}

std::pair<int, int> getMove2d(std::vector<std::vector<int>> v)
{
    return {0, 0};
}
