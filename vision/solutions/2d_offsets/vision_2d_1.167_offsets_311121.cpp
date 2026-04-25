#include "vision.h"

#include <algorithm>

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
    int m = pat1d.size();

    int space = 2;
    int offsetStep = 2;
    std::vector<int> offsets;
    for (int i = 0; i < m; i += offsetStep)
    {
        offsets.push_back(i);
    }

    std::vector<std::vector<int>> pattern;
    for (int o : offsets)
    {
        pattern.push_back(std::vector<int>(m));
        for (int j = 0; j < m; j++)
        {
            pattern.back()[j] = pat1d[(j - o + m) % m];
        }
        for (int j = 0; j < space; j++)
        {
            pattern.push_back(std::vector<int>(m, 1));
        }
    }

    return pattern;
}

std::pair<int, int> getMove2d(std::vector<std::vector<int>> v)
{
    int curr = (v.size() - 1) / 2;

    if (curr == 1)
    {
        if (v[1][0] != 1) return {1, 0};
        if (v[1][2] != 1) return {1, 2};

        for (int i = 0; i <= 2; i++)
        {
            if (i == 1) continue;
            for (int j = 0; j <= 2; j++)
            {
                if (v[i][j] != 1) return {i, j};
            }
        }

        return {0, 0};
    }

    int newJ = getMove1d(v[curr]);

    if (curr != 3) return {curr, newJ};

    const int NONE = 100;

    std::vector<int> offs(v.size(), NONE);
    for (int i = 0; i < (int) v.size(); i++)
    {
        for (int j = 0; j < (int) v.size(); j++)
        {
            if (v[i][j] == 3)
            {
                int o = j - curr;
                if (std::abs(o) < std::abs(offs[i])) offs[i] = o;
            }
        }
    }

    if (newJ > curr)
    {
        if (offs[6] == 2) return {6, 5};
        if (offs[0] == 2) return {0, 5};
    }
    else
    {
        if (offs[6] == -2) return {6, 1};
        if (offs[0] == -2) return {0, 1};
    }

    return {-1, -1};
}
