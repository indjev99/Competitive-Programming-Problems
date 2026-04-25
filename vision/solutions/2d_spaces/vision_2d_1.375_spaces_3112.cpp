#include "vision.h"

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
    std::vector<int> pat1d = getVisionPattern1d();
    int m = pat1d.size();

    std::vector<int> spaces = {0, 1, 2};

    std::vector<std::vector<int>> pattern;
    for (int s : spaces)
    {
        pattern.push_back(pat1d);
        for (int j = 0; j < s; j++)
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

        return {-1, -1};
    }

    int newJ = getMove1d(v[curr]);

    if (curr != 3) return {curr, newJ};

    std::vector<int> trans(v.size());
    for (int i = 0; i < (int) v.size(); i++)
    {
        trans[i] = v[i][curr];
    }

    bool have0 = trans[2] == 3 || trans[4] == 3;
    bool have1 = trans[1] == 3 || trans[5] == 3;
    bool have2 = (trans[1] == 1 && trans[0] == 3) || (trans[5] == 1 && trans[6] == 3);

    if (have2 && have0)
    {
        if (trans[2] == 3) return {2, newJ};
        if (trans[4] == 3) return {4, newJ};
    }
    else if (have0 && have1)
    {
        if (trans[1] == 3) return {1, newJ};
        if (trans[5] == 3) return {5, newJ};
    }
    else if (have1 && have2)
    {
        if (trans[1] == 1 && trans[0] == 3) return {0, newJ};
        if (trans[5] == 1 && trans[6] == 3) return {6, newJ};
    }

    return {-1, -1};
}
