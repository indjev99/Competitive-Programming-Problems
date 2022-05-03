#include <iostream>
#include <algorithm>
#include <vector>

const int MAX_N = 3e3;

int n, m, h, w;
char table[MAX_N][MAX_N];

int cloudNum;

void input()
{
    std::cin >> n >> m >> h >> w;

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            std::cin >> table[i][j];
        }
    }
}

void output()
{
    std::cout << cloudNum << "\n";
}

#define NOCOMP -1

int numComps;
int comps[MAX_N][MAX_N];

void compsDFS(int i, int j, int comp)
{
    std::vector<std::pair<int, int>> stack;

    comps[i][j] = comp;
    stack.push_back({i, j});

    while (!stack.empty())
    {
        auto [x, y] = stack.back();
        stack.pop_back();

        if (x > 0 && comps[x - 1][y] == NOCOMP && table[x - 1][y] == 'x')
        {
            comps[x - 1][y] = comp;
            stack.push_back({x - 1, y});
        }
        if (x < n - 1 && comps[x + 1][y] == NOCOMP && table[x + 1][y] == 'x')
        {
            comps[x + 1][y] = comp;
            stack.push_back({x + 1, y});
        }
        if (y > 0 && comps[x][y - 1] == NOCOMP && table[x][y - 1] == 'x')
        {
            comps[x][y - 1] = comp;
            stack.push_back({x, y - 1});
        }
        if (y < m - 1 && comps[x][y + 1] == NOCOMP && table[x][y + 1] == 'x')
        {
            comps[x][y + 1] = comp;
            stack.push_back({x, y + 1});
        }
    }
}

void findComps()
{
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            comps[i][j] = NOCOMP;
        }
    }

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            if (comps[i][j] == NOCOMP && table[i][j] == 'x') compsDFS(i, j, numComps++);
        }
    }
}

int counts[MAX_N * MAX_N];

void solve()
{
    findComps();

    for (int i = 0; i < n - h + 1; ++i)
    {
        for (int j = 0; j < m - w + 1; ++j)
        {
            int numDiff = 0;
            std::fill(counts, counts + numComps, 0);

            for (int s = 0; s < h; ++s)
            {
                for (int t = 0; t < w; ++t)
                {
                    int comp = comps[i + s][j + t];
                    if (comp != NOCOMP && counts[comp]++ == 0) ++numDiff;
                }
            }

            if (numDiff > cloudNum)
            {
                cloudNum = numDiff;
            }
        }
    }
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    input();
    solve();
    output();

    return 0;
}
