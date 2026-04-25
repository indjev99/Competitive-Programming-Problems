#include "vision.h"

std::vector<int> getVisionPattern1d()
{
    return {4, 1, 1, 1, 2, 1, 2, 2, 1, 1};
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

    if (curr == 4)
    {
        if (v[1] == 1 && v[0] == 2) return 0;
        if (v[7] == 1 && v[8] == 2) return 8;
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
