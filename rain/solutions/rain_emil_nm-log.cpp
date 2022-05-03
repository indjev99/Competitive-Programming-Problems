#include <iostream>
#include <algorithm>
#include <vector>
#include <map>

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

int lbY[MAX_N * MAX_N], rbY[MAX_N * MAX_N];
std::vector<std::pair<int, int>> cellsInComp[MAX_N * MAX_N];

void findCellsInComps()
{
    for (int k = 0; k < numComps; ++k)
    {
        lbY[k] = m;
        rbY[k] = 0;
    }

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            int comp = comps[i][j];
            if (comp == NOCOMP) continue;
            lbY[comp] = std::min(lbY[comp], j);
            rbY[comp] = std::max(rbY[comp], j + 1);
            cellsInComp[comp].push_back({i, j});
        }
    }
}

std::map<int, int> depthMap;

int inside2DAdders[MAX_N + 1][MAX_N + 1];
int insideAdders[MAX_N][MAX_N];

void solve()
{
    findComps();
    findCellsInComps();

    for (int k = 0; k < numComps; ++k)
    {
        depthMap.clear();
        depthMap[std::max(lbY[k] - w + 1, 0)] = -1;
        depthMap[rbY[k]] = -1;

        int lastX = -1, lastY = -1;
        for (auto [i, j] : cellsInComp[k])
        {
            if (i > lastX)
            {
                lastX = i;
                lastY = std::max(lbY[k] - w, -1);
            }

            int initStartY = std::max(lastY + 1, j - w + 1);
            auto initNextIt = depthMap.upper_bound(initStartY);
            auto startIt = std::prev(initNextIt);
            if (startIt->first < initStartY) startIt = depthMap.insert(initNextIt, {initStartY, startIt->second});

            for (; startIt->first <= j;)
            {
                int startX = std::max(startIt->second + 1, i - h + 1);

                auto nextIt = std::next(startIt);
                if (nextIt->first > j + 1) nextIt = depthMap.insert(nextIt, {j + 1, startIt->second});

                ++inside2DAdders[startX][startIt->first];
                --inside2DAdders[startX][nextIt->first];
                --inside2DAdders[i + 1][startIt->first];
                ++inside2DAdders[i + 1][nextIt->first];

                if (startIt->first > initStartY) depthMap.erase(startIt);
                else startIt->second = i;
                startIt = nextIt;
            }

            lastY = j;
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

    for (int j = 0; j < m - w + 1; ++j)
    {
        int value = 0;
        for (int i = 0; i < n - h + 1; ++i)
        {
            value += insideAdders[i][j];
            if (value > cloudNum)
            {
                cloudNum = value;
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
