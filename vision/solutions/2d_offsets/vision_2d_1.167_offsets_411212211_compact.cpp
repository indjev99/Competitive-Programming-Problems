#include "vision.h"

#include <algorithm>

std::vector<int> getVisionPattern1d()
{
    return {4, 1, 1, 2, 1, 2, 2, 1, 1};
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

    if (curr == 4)
    {
        if (v[0] == 1 && v[1] == 2) return 1;
        if (v[8] == 1 && v[7] == 2) return 7;
        return -1;
    }

    return -1;
}

std::vector<std::vector<int>> getVisionPattern2d()
{
    std::vector<int> pat1d = getVisionPattern1d();
    int m = pat1d.size();

    int space = 3;
    std::vector<int> offsets = {0, 1, -1};

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

    if (curr != 4) return {curr, newJ};

    const int NONE = 100;

    std::vector<int> offs(v.size(), NONE);
    for (int i = 0; i < (int) v.size(); i++)
    {
        for (int j = 0; j < (int) v.size(); j++)
        {
            if (v[i][j] == 4)
            {
                int o = j - curr;
                if (std::abs(o) < std::abs(offs[i])) offs[i] = o;
            }
        }
    }

    if (newJ > curr)
    {
        if (offs[0] == -1 && offs[8] == 1) return {8, 8};
        if (offs[8] == -1 && offs[0] == 1) return {0, 8};

        if (offs[0] == -1 && offs[8] == -2) return {8, 6};
        if (offs[8] == -1 && offs[0] == -2) return {0, 6};

        if (offs[0] == 2 && offs[8] == 1) return {8, 8};
        if (offs[8] == 2 && offs[0] == 1) return {0, 8};
    }
    else
    {
        if (offs[0] == 1 && offs[8] == -1) return {8, 0};
        if (offs[8] == 1 && offs[0] == -1) return {0, 0};

        if (offs[0] == 1 && offs[8] == 2) return {8, 2};
        if (offs[8] == 1 && offs[0] == 2) return {0, 2};

        if (offs[0] == -2 && offs[8] == -1) return {8, 0};
        if (offs[8] == -2 && offs[0] == -1) return {0, 0};
    }

    return {-1, -1};
}
