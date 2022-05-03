#include <iostream>
#include <algorithm>
#include <bitset>
#include <vector>

const int MAX_N = 3e3;
const int MAX_COMPS = 400;

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

int ubX[MAX_COMPS], dbX[MAX_COMPS], lbY[MAX_COMPS], rbY[MAX_COMPS];

int inside2DAdders[MAX_N + 1][MAX_N + 1];
int insideAdders[MAX_N][MAX_N];
int inside[MAX_N][MAX_N];

void findInsides()
{
    for (int k = 0; k < numComps; ++k)
    {
        ubX[k] = n;
        dbX[k] = 0;
        lbY[k] = m;
        rbY[k] = 0;
    }

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            int comp = comps[i][j];
            if (comp == NOCOMP) continue;
            ubX[comp] = std::min(ubX[comp], i);
            dbX[comp] = std::max(dbX[comp], i + 1);
            lbY[comp] = std::min(lbY[comp], j);
            rbY[comp] = std::max(rbY[comp], j + 1);
        }
    }

    for (int k = 0; k < numComps; ++k)
    {
        int startX = std::max(dbX[k] - h + 1, 0);
        int startY = std::max(rbY[k] - w + 1, 0);
        if (startX < ubX[k] && startY < lbY[k])
        {
            ++inside2DAdders[startX][startY];
            --inside2DAdders[ubX[k]][startY];
            --inside2DAdders[startX][lbY[k]];
            ++inside2DAdders[ubX[k]][lbY[k]];
        }
    }

    for (int i = 0; i < n; ++i)
    {
        int adder = 0;
        for (int j = 0; j < m; ++j)
        {
            adder += inside2DAdders[i][j];
            insideAdders[i][j] = adder;
        }
    }

    for (int j = 0; j < m; ++j)
    {
        int value = 0;
        for (int i = 0; i < n; ++i)
        {
            value += insideAdders[i][j];
            inside[i][j] = value;
        }
    }
}

int counts[MAX_COMPS];
std::bitset<MAX_COMPS> set;
std::bitset<MAX_COMPS> horSets[MAX_N][MAX_N];
std::bitset<MAX_COMPS> vertSets[MAX_N][MAX_N];

void solve()
{
    findComps();
    findInsides();

    for (int i = 0; i < n; ++i)
    {
        std::fill(counts, counts + numComps, 0);
        set.reset();
        for (int j = 0; j < m; ++j)
        {
            int comp = comps[i][j];
            if (comp != NOCOMP && counts[comp]++ == 0) set[comp] = true;
            if (j >= w)
            {
                int comp2 = comps[i][j - w];
                if (comp2 != NOCOMP && --counts[comp2] == 0) set[comp2] = false;
            }
    
            if (j >= w - 1)
            {
                horSets[i][j - w + 1] = set;
            }
        }
    }

    for (int j = 0; j < m; ++j)
    {
        std::fill(counts, counts + numComps, 0);
        set.reset();
        for (int i = 0; i < n; ++i)
        {
            int comp = comps[i][j];
            if (comp != NOCOMP && counts[comp]++ == 0) set[comp] = true;
            if (i >= h)
            {
                int comp2 = comps[i - h][j];
                if (comp2 != NOCOMP && --counts[comp2] == 0) set[comp2] = false;
            }
    
            if (i >= h - 1)
            {
                vertSets[i - h + 1][j] = set;
            }
        }
    }

    for (int i = 0; i < n - h + 1; ++i)
    {
        for (int j = 0; j < m - w + 1; ++j)
        {
            set = horSets[i][j];
            set |= horSets[i + h - 1][j];
            set |= vertSets[i][j];
            set |= vertSets[i][j + w - 1];

            int numDiffs = set.count() + inside[i][j];
            if (numDiffs > cloudNum)
            {
                cloudNum = numDiffs;
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
