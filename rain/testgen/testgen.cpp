#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <random>
#include <map>
#include <time.h>

std::mt19937 generator;

int randNum(int lb, int ub)
{
    std::uniform_int_distribution<int> distribution(lb, ub - 1);
    return distribution(generator);
}

double randReal(double lb, double ub)
{
    std::uniform_real_distribution<double> distribution(lb, ub);
    return distribution(generator);
}

const int MAX_N = 5e3;

int n, m, h, w;
bool table[MAX_N][MAX_N];

// SOLVER

#define NOCOMP -1

int numComps;
int comps[MAX_N][MAX_N];
int cloudX, cloudY, cloudNum;

void compsDFS(int i, int j, int comp)
{
    std::vector<std::pair<int, int>> stack;

    comps[i][j] = comp;
    stack.push_back({i, j});

    while (!stack.empty())
    {
        auto [x, y] = stack.back();
        stack.pop_back();

        if (x > 0 && comps[x - 1][y] == NOCOMP && table[x - 1][y])
        {
            comps[x - 1][y] = comp;
            stack.push_back({x - 1, y});
        }
        if (x < n - 1 && comps[x + 1][y] == NOCOMP && table[x + 1][y])
        {
            comps[x + 1][y] = comp;
            stack.push_back({x + 1, y});
        }
        if (y > 0 && comps[x][y - 1] == NOCOMP && table[x][y - 1])
        {
            comps[x][y - 1] = comp;
            stack.push_back({x, y - 1});
        }
        if (y < m - 1 && comps[x][y + 1] == NOCOMP && table[x][y + 1])
        {
            comps[x][y + 1] = comp;
            stack.push_back({x, y + 1});
        }
    }
}

void findComps()
{
    numComps = 0;
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
            if (comps[i][j] == NOCOMP && table[i][j]) compsDFS(i, j, numComps++);
        }
    }
}

int lbY[MAX_N * MAX_N], rbY[MAX_N * MAX_N];
std::vector<std::pair<int, int>> cellsInComp[MAX_N * MAX_N];
int maxCellsInComp;

void findCellsInComps()
{
    maxCellsInComp = 0;
    for (int k = 0; k < numComps; ++k)
    {
        lbY[k] = n;
        rbY[k] = 0;
        cellsInComp[k].clear();
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
            maxCellsInComp = std::max<int>(maxCellsInComp, cellsInComp[comp].size());
        }
    }
}

std::map<int, int> depthMap;

int inside2DAdders[MAX_N + 1][MAX_N + 1];
int insideAdders[MAX_N][MAX_N];
int inside[MAX_N][MAX_N];

void solve()
{
    cloudX = 0;
    cloudY = 0;
    cloudNum = 0;

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            inside2DAdders[i][j] = 0;
            insideAdders[i][j] = 0;
            inside[i][j] = 0;
        }
    }

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
            inside[i][j] = value;
            if (value > cloudNum)
            {
                cloudNum = value;
                cloudX = i;
                cloudY = j;
            }
        }
    }
}

// END SOLVER

#define T_RANDOM 0
#define T_ROWS 1
#define T_COLS 2
#define T_DIAGS1 3
#define T_DIAGS2 4
#define T_CIRCLES 5
#define T_PLUSES 6
#define T_RHOMBS 7
#define T_XS 8
#define T_US1 9
#define T_US2 10
#define T_US3 11
#define T_US4 12
#define T_PATHS 13
#define T_FILLED 14

int type;
int spacing;
bool noise;
int maxComps;
int maxSize;
bool makeUniqueSol;

bool checkValid(int i, int j)
{
    if (i < 0 || i >= n || j < 0|| j >= m) return false;
    return true;
}

bool checkCell(int i, int j)
{
    if (i < 0 || i >= n || j < 0 || j >= m) return false;
    return table[i][j];
}

int checkAdj(int i, int j)
{
    return checkCell(i, j) + checkCell(i - 1, j) + checkCell(i + 1, j) + checkCell(i, j - 1) + checkCell(i, j + 1);
}

bool taken2[MAX_N];

bool genTest(std::ostream& out)
{
    w = randNum(2, n);
    h = randNum(2, m);

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            table[i][j] = false;
        }
        taken2[i] = false;
    }

    std::cerr << " starting " << type << " " << maxComps << " " << maxSize << std::endl;

    if (maxComps > 0 && makeUniqueSol) maxComps -= 2;

    if (maxComps == 0 && maxSize == 0 && type == T_RANDOM)
    {
        std::cerr << " 1 " << std::endl;
        double density = randReal(0.1, 0.4);
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
            {
                table[i][j] = randReal(0, 1) < density;
            }
        }
    }
    else if (maxComps == 0 && maxSize == 0 && type == T_ROWS)
    {
        std::cerr << " 2 " << std::endl;
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
            {
                table[i][j] = i % (2 + spacing) < 1;
            }
        }
    }
    else if (maxComps == 0 && maxSize == 0 && type == T_COLS)
    {
        std::cerr << " 3 " << std::endl;
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
            {
                table[i][j] = j % (2 + spacing) < 1;
            }
        }
    }
    else if (maxComps == 0 && maxSize == 0 && type == T_DIAGS1)
    {
        std::cerr << " 4 " << std::endl;
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
            {
                table[i][j] = (i + j) % (3 + spacing) < 2;
            }
        }
    }
    else if (maxComps == 0 && maxSize == 0 && type == T_DIAGS2)
    {
        std::cerr << " 5 " << std::endl;
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
            {
                table[i][j] = ((i - j) % (3 + spacing) + (3 + spacing)) % (3 + spacing) < 2;
            }
        }
    }
    else if (maxComps == 0 && maxSize == 0 && type == T_CIRCLES)
    {
        std::cerr << " 6 " << std::endl;
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
            {
                table[i][j] = std::max(std::abs(i - n / 2), std::abs(j - m / 2)) % (2 + spacing) < 1;
            }
        }
    }
    else if (maxComps == 0 && maxSize == 0 && type == T_PLUSES)
    {
        std::cerr << " 7 " << std::endl;
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
            {
                table[i][j] = std::min(std::abs(i - n / 2), std::abs(j - m / 2)) % (2 + spacing) < 1;
            }
        }
    }
    else if (maxComps == 0 && maxSize == 0 && type == T_RHOMBS)
    {
        std::cerr << " 8 " << std::endl;
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
            {
                table[i][j] = std::max(std::abs(i - j), std::abs(i + j - m + 1)) % (2 + spacing) < 1;
            }
        }
    }
    else if (maxComps == 0 && maxSize == 0 && type == T_XS)
    {
        std::cerr << " 9 " << std::endl;
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
            {
                table[i][j] = std::min(std::abs(i - j), std::abs(i + j - m + 1)) % (2 + spacing) < 1;
            }
        }
    }
    else if (maxComps == 0 && maxSize == 0 && type == T_US1)
    {
        std::cerr << " 10 " << std::endl;
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
            {
                table[i][j] = std::max(std::abs(i - n / 2), std::abs(j - m / 2)) % (2 + spacing) < 1;
            }
        }
    }
    else if (maxComps == 0 && maxSize == 0 && type == T_US2)
    {
        std::cerr << " 11 " << std::endl;
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
            {
                table[i][j] = std::max(i - n / 2, std::abs(j - m / 2)) % (2 + spacing) < 1;
            }
        }
    }
    else if (maxComps == 0 && maxSize == 0 && type == T_US3)
    {
        std::cerr << " 12 " << std::endl;
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
            {
                table[i][j] = std::max(n / 2 - i, m / 2 - j) % (2 + spacing) < 1;
            }
        }
    }
    else if (maxComps == 0 && maxSize == 0 && type == T_US4)
    {
        std::cerr << " 13 " << std::endl;
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
            {
                table[i][j] = std::max(std::abs(i - n / 2), j - m / 2) % (2 + spacing) < 1;
            }
        }
    }
    else if (maxComps == 0 && maxSize == 0 && type == T_PATHS)
    {
        std::cerr << " 14 " << std::endl;
        double density = randReal(0.05, 0.25);
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
            {
                if (!table[i][j] && randReal(0, 1) < density)
                {
                    int x = i;
                    int y = j;
                    table[x][y] = true;
                    std::vector<std::pair<int, int>> opts;
                    while (randNum(0, 2 * n) != 0)
                    {
                        opts.clear();

                        if (checkValid(x - 1, y) && checkAdj(x - 1, y) == 1) opts.emplace_back(x - 1, y);
                        if (checkValid(x + 1, y) && checkAdj(x + 1, y) == 1) opts.emplace_back(x + 1, y);
                        if (checkValid(x, y - 1) && checkAdj(x, y - 1) == 1) opts.emplace_back(x, y - 1);
                        if (checkValid(x, y + 1) && checkAdj(x, y + 1) == 1) opts.emplace_back(x, y + 1);

                        if (opts.empty()) break;

                        int next = randNum(0, opts.size());

                        x = opts[next].first;
                        y = opts[next].second;

                        table[x][y] = true;
                    }
                }
            }
        }
    }
    else if (maxComps == 0 && maxSize == 0 && type == T_FILLED)
    {
        std::cerr << " 15 " << std::endl;
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
            {
                int x = i % (20 + spacing);
                int y = j % (20 + spacing);
                int d = std::min(std::abs(x - 9), std::abs(y - 9));
                table[i][j] = d <= 9;
            }
        }
    }
    else if (maxComps == 0 && maxSize > 0 && type == T_ROWS)
    {
        std::cerr << " 16 " << std::endl;
        double density = randReal(0.1, 0.45);
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
            {
                if (checkAdj(i, j) || randReal(0, 1) > density) continue;
                int x = i;
                int y = j;
                for (int k = 0; k < maxSize && checkValid(x, y) && checkAdj(x, y) <= 1; ++k)
                {
                    table[x][y] = true;
                    ++x;
                }
            }
        }
    }
    else if (maxComps == 0 && maxSize > 0 && type == T_COLS)
    {
        std::cerr << " 17 " << std::endl;
        double density = randReal(0.1, 0.45);
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
            {
                if (checkAdj(i, j) || randReal(0, 1) > density) continue;
                int x = i;
                int y = j;
                for (int k = 0; k < maxSize && checkValid(x, y) && checkAdj(x, y) <= 1; ++k)
                {
                    table[x][y] = true;
                    ++y;
                }
            }
        }
    }
    else if (maxComps == 0 && maxSize > 0 && type == T_DIAGS1)
    {
        std::cerr << " 18 " << std::endl;
        double density = randReal(0.1, 0.45);
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
            {
                if (checkAdj(i, j) || randReal(0, 1) > density) continue;
                int x = i;
                int y = j;
                for (int k = 0; k < maxSize && checkValid(x, y) && checkAdj(x, y) <= 1; ++k)
                {
                    table[x][y] = true;
                    if (k % 2 == 0) ++x;
                    else ++y;
                }
            }
        }
    }
    else if (maxComps == 0 && maxSize > 0 && type == T_DIAGS2)
    {
        std::cerr << " 19 " << std::endl;
        double density = randReal(0.1, 0.45);
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
            {
                if (checkAdj(i, j) || randReal(0, 1) > density) continue;
                int x = i;
                int y = j;
                for (int k = 0; k < maxSize && checkValid(x, y) && checkAdj(x, y) <= 1; ++k)
                {
                    table[x][y] = true;
                    if (k % 2 == 0) ++x;
                    else --y;
                }
            }
        }
    }
    else if (maxComps == 0 && maxSize > 0 && (type == T_PATHS || (maxSize < 30 && type == T_FILLED)))
    {
        std::cerr << " 20 " << std::endl;
        double density = randReal(0.1, 0.45);
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
            {
                if (checkAdj(i, j) || randReal(0, 1) > density) continue;
                int x = i;
                int y = j;
                std::vector<std::pair<int, int>> opts;
                for (int k = 0; k < maxSize && checkValid(x, y) && checkAdj(x, y) <= 1; ++k)
                {
                    table[x][y] = true;

                    if (checkValid(x - 1, y) && checkAdj(x - 1, y) == 1) opts.emplace_back(x - 1, y);
                    if (checkValid(x + 1, y) && checkAdj(x + 1, y) == 1) opts.emplace_back(x + 1, y);
                    if (checkValid(x, y - 1) && checkAdj(x, y - 1) == 1) opts.emplace_back(x, y - 1);
                    if (checkValid(x, y + 1) && checkAdj(x, y + 1) == 1) opts.emplace_back(x, y + 1);

                    if (opts.empty()) break;

                    int next = randNum(0, opts.size());

                    x = opts[next].first;
                    y = opts[next].second;
                }
            }
        }
    }
    else if (maxComps == 0 && maxSize >= 30 && type == T_FILLED)
    {
        std::cerr << " 21 " << std::endl;
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
            {
                int x = i % (8 + spacing);
                int y = j % (8 + spacing);
                int d = std::min(std::abs(x - 3), std::abs(y - 3));
                table[i][j] = d <= 3;
            }
        }
    }
    else if (maxComps > 0 && maxSize == 0 && type == T_ROWS)
    {
        std::cerr << " 22 " << std::endl;
        for (int k = 0; k < maxComps; ++k)
        {
            // std::cerr << k << " ";
            int i = randNum(0, n);
            if (taken2[i])
            {
                --k;
                continue;
            }

            if (i > 0) taken2[i - 1] = true;
            taken2[i] = true;
            if (i < n - 1) taken2[i + 1] = true;

            int left = randNum(0, n);
            int right = randNum(0, n);
            if (right >= left) ++right;
            else std::swap(left, right);
            
            for (int j = left; j < right; ++j)
            {
                table[i][j] = true;
            }
        }
    }
    else if (maxComps > 0 && maxSize == 0 && type == T_COLS)
    {
        std::cerr << " 24 " << std::endl;
        for (int k = 0; k < maxComps; ++k)
        {
            int i = randNum(0, n);
            if (taken2[i])
            {
                --k;
                continue;
            }

            if (i > 0) taken2[i - 1] = true;
            taken2[i] = true;
            if (i < n - 1) taken2[i + 1] = true;

            int left = randNum(0, n);
            int right = randNum(0, n);
            if (right >= left) ++right;
            else std::swap(left, right);
            
            for (int j = left; j < right; ++j)
            {
                table[i][j] = true;
            }
        }
    }
    else if (maxComps > 0 && maxSize == 0 && type == T_DIAGS1)
    {
        std::cerr << " 25 " << std::endl;
        for (int k = 0; k < maxComps; ++k)
        {
            int i = randNum(0, n);

            int left = randNum(0, m);
            int right = randNum(0, m);
            if (right >= left) ++right;
            else std::swap(left, right);

            int x = i;
            int y = left;

            if (!checkValid(x, y) || checkAdj(x, y))
            {
                --k;
                continue;
            }

            int t = 0;
            while (y < right && checkValid(x, y) && checkAdj(x, y) <= 1)
            {
                table[x][y] = true;
                if (t % 2 == 0) ++x;
                else ++y;
                ++t;
            }
        }
    }
    else if (maxComps > 0 && maxSize == 0 && type == T_DIAGS2)
    {
        std::cerr << " 26 " << std::endl;
        for (int k = 0; k < maxComps; ++k)
        {
            int i = randNum(0, n);

            int left = randNum(0, m);
            int right = randNum(0, m);
            if (right >= left) ++right;
            else std::swap(left, right);

            int x = i;
            int y = right - 1;

            if (!checkValid(x, y) || checkAdj(x, y))
            {
                --k;
                continue;
            }

            int t = 0;
            while (y >= left && checkValid(x, y) && checkAdj(x, y) <= 1)
            {
                table[x][y] = true;
                if (t % 2 == 0) ++x;
                else --y;
                ++t;
            }
        }
    }
    else if (maxComps > 0 && maxSize == 0 && type == T_PATHS)
    {
        std::cerr << " 27 " << std::endl;
        for (int k = 0; k < maxComps; ++k)
        {
            int x = randNum(0, n);
            int y = randNum(0, m);

            if (!checkValid(x, y) || checkAdj(x, y))
            {
                --k;
                continue;
            }

            int t = 0;
            while (randNum(0, 5 * n) != 0 && checkValid(x, y) && checkAdj(x, y) <= 1)
            {
                table[x][y] = true;
                ++t;
            }
        }
    }
    else
    {
        std::cerr << "No known generation method: " << type << " " << maxComps << " " << maxSize << std::endl;
        exit(1);
    }

    std::cerr << " generated" << std::endl;

    if (noise)
    {
        double density = randReal(0.02, 0.05);
        if (maxComps > 0) density = randReal(0.025, 0.05);
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
            {
                if (maxSize > 0 && !checkCell(i, j) && checkAdj(i, j)) continue;
                if (maxComps > 0 && (checkCell(i, j) || !checkAdj(i, j))) continue;
                if (randReal(0, 1) < density) table[i][j] ^= 1;
            }
        }
    }

    if (maxComps > 0 && makeUniqueSol) maxComps += 2;

    std::cerr << " noised" << std::endl;

    solve();
    int numSols = 0;
    for (int i = 0; i < n - h + 1; ++i)
    {
        for (int j = 0; j < m - w + 1; ++j)
        {
            if (inside[i][j] != cloudNum) continue;
            ++numSols;
        }
    }

    std::cerr << " solved" << std::endl;

    if (makeUniqueSol && numSols > 1)
    {
        for (int i = 0; i < n - h + 1; ++i)
        {
            for (int j = 0; j < m - w + 1; ++j)
            {
                if (inside[i][j] != cloudNum) continue;
                int farI = i + h - 1;
                int farJ = j + w - 1;
                if (
                    !checkAdj(i, j) &&
                    !checkAdj(farI, farJ))
                {
                    table[i][j] = true;
                    table[farI][farJ] = true;
                    goto foundUnique;
                }
                if (
                    !checkAdj(farI, j) &&
                    !checkAdj(i, farJ))
                {
                    table[farI][j] = true;
                    table[i][farJ] = true;
                    goto foundUnique;
                }
            }
        }

        std::cerr << "Can't make unique" << std::endl;
        return false;

        foundUnique:

        solve();
        numSols = 0;
        for (int i = 0; i < n - h + 1; ++i)
        {
            for (int j = 0; j < m - w + 1; ++j)
            {
                if (inside[i][j] != cloudNum) continue;
                ++numSols;
            }
        }

        std::cerr << " uniqued" << std::endl;
    }

    std::cerr << "Sol: " << cloudNum << "\nNum sols: " << numSols << "\nNum comps: " << numComps << "\nMax comp: " << maxCellsInComp << std::endl;

    out << n << " " << m << " " << w << " " << h << "\n";

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            if (j > 0) out << " ";
            out << (table[i][j] ? 'x' : '.');
        }
        out << "\n";
    }

    out << std::flush;

    if ((makeUniqueSol && numSols > 1))
    {
        std::cerr << "Didn't meet specs 1" << std::endl;
        exit(-1);
    }

    if ((maxComps > 0 && numComps > maxComps))
    {
        std::cerr << "Didn't meet specs 2: " << maxComps << std::endl;
        exit(-1);
    }

    if ((maxSize > 0 && maxCellsInComp > maxSize))
    {
        std::cerr << "Didn't meet specs 3: " << maxSize << std::endl;
        exit(-1);
    }

    return true;
}

#define T_RANDOM 0
#define T_ROWS 1
#define T_COLS 2
#define T_DIAGS1 3
#define T_DIAGS2 4
#define T_CIRCLES 5
#define T_PLUSES 6
#define T_RHOMBS 7
#define T_XS 8
#define T_US1 9
#define T_US2 10
#define T_US3 11
#define T_US4 12
#define T_PATHS 13
#define T_FILLED 14

struct Params
{
    int type;
    int spacing;
    bool noise;
    bool makeUniqueSol;
};

// 0
std::vector<Params> easy = {
    {T_RANDOM, 0, false, false},
    {T_RANDOM, 0, false, true},
    {T_ROWS, 0, false, false},
    {T_DIAGS2, 5, true, true},
    {T_PATHS, 0, true, true}
};

// 1 -- no
std::vector<Params> medium = {
};

// 2
std::vector<Params> hard = {
    {T_RANDOM, 0, false, false},
    {T_RANDOM, 0, false, true},
    {T_ROWS, 1, true, false},
    {T_DIAGS1, 0, false, false},
    {T_DIAGS2, 3, true, false},
    {T_CIRCLES, 6, true, true},
    {T_PLUSES, 3, true, true},
    {T_XS, 2, true, false},
    {T_RHOMBS, 0, false, false},
    {T_US2, 1, false, false},
    {T_US1, 0, true, false},
    {T_FILLED, 0, true, false},
    {T_PATHS, 0, true, true},
    {T_RANDOM, 0, false, true},
    {T_RANDOM, 0, false, false}
};

// 3
std::vector<Params> fewComps = {
    {T_ROWS, 0, true, true},
    {T_COLS, 0, true, false},
    {T_DIAGS1, 0, true, true},
    {T_DIAGS2, 0, true, false},
    {T_PATHS, 0, true, false},
    {T_PATHS, 0, false, true},
};

// 4
std::vector<Params> smallComps = {
    {T_ROWS, 0, true, true},
    {T_COLS, 0, true, false},
    {T_DIAGS1, 0, false, true},
    {T_DIAGS2, 0, true, false},
    {T_PATHS, 0, false, false},
    {T_PATHS, 0, true, true}
};

std::vector<Params>* pVecs[] = {
    &easy,
    &medium,
    &hard,
    &fewComps,
    &smallComps
};

int main()
{
    std::ios::sync_with_stdio(false);

    generator.seed(time(nullptr));

    int num, str, start;
    while (true)
    {
        num = -1;
        std::cerr << std::endl;
        std::cout << "Start Test_num? ";
        std::cin >> start >> num;
        if (num < 0 || num >= 100) break;
        std::cout << "n m strenth maxComps maxSize? ";
        std::cin >> n >> m >> str >> maxComps >> maxSize;
        std::vector<Params>& tests = *pVecs[str];
        for (int i = num - start; i < (int) tests.size(); ++i)
        {
            type = tests[i].type;
            spacing = tests[i].spacing;
            noise = tests[i].noise;
            makeUniqueSol = tests[i].makeUniqueSol;

            std::string nums = "";
            nums += (num / 10 + '0');
            nums += (num % 10 + '0');

            std::string name = "rain." + nums + ".in";

            std::cerr << "\n" << name << " -- " << i << std::endl;

            std::ofstream outfile(name);

            int cnt = 0;
            while (!genTest(outfile) && ++cnt <= 20) {}
            if (cnt > 20) exit(-1);

            outfile.close();

            ++num;
        }
    }

    return 0;
}
