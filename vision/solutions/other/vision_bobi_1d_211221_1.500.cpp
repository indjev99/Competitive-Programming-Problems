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
    return {};
}

std::pair<int, int> getMove2d(std::vector<std::vector<int>> v)
{
    return {0, 0};
}