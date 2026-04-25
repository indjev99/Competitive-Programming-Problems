#include "vision.h"

std::vector<int> getVisionPattern1d()
{
    return {3, 1, 1, 1, 2, 1};
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
        if (v[0] == 3) return 0;
        if (v[4] == 3) return 4;
        return -1;
    }

    if (curr == 3)
    {
        if (v[1] == 2) return 6;
        if (v[5] == 2) return 0;
        return -1;
    }

    return -1;
}

std::vector<std::vector<int>> getVisionPattern2d()
{
    std::vector<int> pat1d = getVisionPattern1d();
    int n = pat1d.size();
    std::vector<std::vector<int>> pattern(n, std::vector<int>(n));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            pattern[i][j] = pat1d[(i + j) % n];
        }
    }
    return pattern;
}

std::pair<int, int> getMove2d(std::vector<std::vector<int>> v)
{
    int curr = (v.size() - 1) / 2;
    std::vector<int> trans(v.size());
    for (int i = 0; i < (int) v.size(); i++)
    {
        trans[i] = v[i][curr];
    }
    if (curr == 3) return {getMove1d(trans), curr};
    else return {curr, getMove1d(v[curr])};
}
