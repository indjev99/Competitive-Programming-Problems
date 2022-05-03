#include <iostream>
#include <algorithm>
#include <vector>

const int MAX_N = 3e3;
const int MAX_COMPS = 20;

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

int counts[MAX_COMPS];
int allCounts[MAX_N][MAX_N][MAX_COMPS];

void solve()
{
    findComps();

    for (int i = 0; i < n; ++i)
    {
        std::fill(counts, counts + numComps, 0);
        for (int j = 0; j < m; ++j)
        {
            int comp = comps[i][j];
            if (comp != NOCOMP) ++counts[comp];
            if (j >= w)
            {
                int comp2 = comps[i][j - w];
                if (comp2 != NOCOMP) --counts[comp2];
            }
    
            if (j >= w - 1)
            {
                for (int k = 0; k < MAX_COMPS; ++k)
                {
                    allCounts[i][j - w + 1][k] = counts[k];
                }
            }
        }
    }

    for (int j = 0; j < m - w + 1; ++j)
    {
        std::fill(counts, counts + numComps, 0);
        for (int i = 0; i < n; ++i)
        {
            int numDiff = 0;
            for (int k = 0; k < numComps; ++k)
            {
                counts[k] += allCounts[i][j][k];
                if (i >= h) counts[k] -= allCounts[i - h][j][k];
                if (counts[k] > 0) ++numDiff;
            }

            if (i >= h - 1 && numDiff > cloudNum)
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
