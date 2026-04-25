#include "vision.h"

#include <algorithm>

std::vector<int> getVisionPattern1d()
{
    return {3, 1, 1, 2, 1};
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
        if (v[0] == 3) return 0;
        if (v[4] == 3) return 4;
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

const int space = 3;
const int offset = 1;

std::vector<std::vector<int>> getVisionPattern2d()
{
    std::vector<int> pat1d = getVisionPattern1d();
    int m = pat1d.size();

    int periods = 1;
    while (m * periods % (space + 1) != 0)
    {
        periods++;
    }

    int n = m * periods;
    std::vector<std::vector<int>> pattern(n, std::vector<int>(n, 1));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if ((j - i + n) % (space + 1) == 0)
            {
                int pIdx = (j - i + (j < i ? n : 0)) / (space + 1);
                pattern[i][j] = pat1d[(i + offset * pIdx) % m];
            }
        }
    }

    return pattern;
}

std::pair<int, int> getMove2d(std::vector<std::vector<int>> v)
{
    int curr = (v.size() - 1) / 2;

    std::vector<int> diag(v.size());
    for (int i = 0; i < (int) v.size(); i++)
    {
        diag[i] = v[i][curr];
    }

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

    int n = v.size();
    bool onDiag1 = false;
    bool onDiag2 = false;
    bool offDiag1 = false;
    bool offDiag2 = false;
    std::vector<int> diag1(n);
    std::vector<int> diag2(n);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            bool on1 = (j - i) % (space + 1) == 0;
            bool on2 = (j + i - n + 1) % (space + 1) == 0;

            if (v[i][j] != 1)
            {
                if (!on1) offDiag1 = true;
                if (!on2) offDiag2 = true;
            }

            if (j == i)
            {
                if (v[i][j] != 1 && !(i == curr && j == curr)) onDiag1 = true;
                diag1[i] = v[i][j];
            }
            if (j == n - i - 1)
            {
                if (v[i][j] != 1 && !(i == curr && j == curr)) onDiag2 = true;
                diag2[i] = v[i][j];
            }
        }
    }

    bool valid1 = onDiag1 && !offDiag1;
    bool valid2 = onDiag2 && !offDiag2;

    if (valid1)
    {
        int newI = getMove1d(diag1);
        return {newI, newI};
    }

    if (valid2)
    {
        int newI = getMove1d(diag2);
        return {newI, n - newI - 1};
    }

    return {-1, -1};
}
