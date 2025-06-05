#include <iostream>
#include <fstream>
#include <ctime>
#include <cassert>
#include <algorithm>
#include <numeric>
#include <vector>
#include <unordered_set>
#include <random>

std::mt19937 generator(0);

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

const int BITS = 6;

#define TT_RAND 0
#define TT_VALVE 1
#define TT_SIEVE 2
#define TT_STAIRS 3
#define TT_LOGSIEVE 4
#define TT_CUP 5
#define TT_VALVE2 6

struct TestSpec
{
    int n;
    int m;

    int testType = TT_RAND;

    // rand
    std::vector<double> meanWallsPerRow = {};
    double neiWallProb = 0.65;

    // valve
    int nonValveLen = 0;

    // sieve
    double fracSieve = 0.5;

    // stairs
    bool endPipe = false;
    int numThickPipes = 0;
    bool doubleGap = false;

    // logsieve
    bool logSieveWall = false;

    double flipProb = 0.5;

    double leftProb = 0.5;
    int maxCostDecr = 0;
    bool useDiff = true;
    bool useLen = false;

    void printDescr() const
    {
        std::cout << n << " " << m << " / " << testType << ":";
        if (testType == TT_RAND)
        {
            for (double mwpr : meanWallsPerRow) std::cout << " " << mwpr;
        }
        if (testType == TT_VALVE || testType == TT_VALVE2)
        {
            std::cout << " " << nonValveLen;
        }
        if (testType == TT_SIEVE)
        {
            std::cout << " " << fracSieve;
        }
        if (testType == TT_STAIRS)
        {
            std::cout << " " << endPipe << " " << numThickPipes << " " << doubleGap;
        }
        if (testType == TT_LOGSIEVE)
        {
            std::cout << " " << logSieveWall;
        }
        std::cout << " / " << flipProb << " " << useDiff << " " << useLen << std::endl;
    }
};

void genRand(std::vector<std::vector<bool>>& walls, int& k, const TestSpec& testSpec)
{
    int n = testSpec.n;
    int m = testSpec.m;

    std::vector<std::vector<bool>> reach(
        n + 1, std::vector<bool>(m, false)
    );

    std::fill(reach[n].begin(), reach[n].end(), true);

    for (int i = n - 1; i >= 0; i--)
    {
        double meanWalls = testSpec.meanWallsPerRow[randNum(0, testSpec.meanWallsPerRow.size())];
        double wallProb = meanWalls / m;
    
        for (int j = 0; j < m; j++)
        {
            if ((walls[i + 1][j] || (j > 0 && walls[i][j - 1])) && randReal(0, 1) < testSpec.neiWallProb)
            {
                walls[i][j] = true;
            }
            else if (randReal(0, 1) < wallProb)
            {
                walls[i][j] = true;
            }

            if (!walls[i][j] && (reach[i + 1][j] || (j > 0 && reach[i][j - 1])))
            {
                reach[i][j] = true;
            }
        }

        bool anyReach = false;

        for (int j = m - 1; j >= 0; j--)
        {
            if (!walls[i][j] && j < m - 1 && reach[i][j + 1])
            {
                reach[i][j] = true;
            }

            anyReach = anyReach || reach[i][j];
        }

        if (!anyReach)
        {
            std::fill(walls[i].begin(), walls[i].end(), false);
            i++;
        }
    }

    k = 0;
    do
    {
        k = randNum(0, m);
    }
    while (!reach[0][k]);
}

void genValve(std::vector<std::vector<bool>>& walls, int& k, const TestSpec& testSpec)
{
    int n = testSpec.n;
    int m = testSpec.m;

    k = randNum(2, m);

    int vLip = m - testSpec.nonValveLen - 1;

    for (int i = 0; i < n; i++)
    {
        if (i % 3 == 0)
        {
            walls[i][1] = true;
        }
        else if (i % 3 == 1)
        {
            walls[i][vLip] = true;
        }
        else
        {
            for (int j = 1; j <= vLip; j++)
            {
                walls[i][j] = true;
            }
        }
    }

    walls[n - 1][1] = true;
}

void genSieve(std::vector<std::vector<bool>>& walls, int& k, const TestSpec& testSpec)
{
    int n = testSpec.n;
    int m = testSpec.m;

    k = randNum(0, m);

    int sieveLen = m * testSpec.fracSieve;

    for (int i = 0; i < n; i++)
    {
        if (i % 2 == 0)
        {
            continue;
        }

        int l = randNum(0, m - sieveLen + 1);
        int r = l + sieveLen;

        for (int j = l; j < r; j += 2)
        {
            walls[i][j] = true;
        }
    }
}

void genStairs(std::vector<std::vector<bool>>& walls, int& k, const TestSpec& testSpec)
{
    int n = testSpec.n;
    int m = testSpec.m;

    k = randNum(0, m);

    for (int i = 0; i < n; i++)
    {
        if (i % 3 == 0)
        {
            continue;
        }
        else if (i % 3 == 1)
        {
            walls[i][1] = true;
            walls[i][2] = true;
        }
        else
        {
            walls[i][2] = true;
        }
    }

    for (int x = n - 1; x >= 0; x--)
    {
        if (walls[x][1]) break;
        walls[x][1] = true;
    }

    int s = 1;
    for (; 1 + 3 * s < n; s++)
    {
        int x = 1;
        int y = 2 + 2 * s;

        assert(y < m);

        walls[x][y] = true;

        for (int i = 0; i < 1 + s; i++)
        {
            x++;
            walls[x][y] = true;
        }

        for (int i = 0; i < 2 * s - 1; i++)
        {
            x++;
            y--;
            walls[x][y] = true;
        }
    }

    s--;
    int ey = 2 + 2 * s;
    int cey = ey;

    for (int y = ey; y < m; y++)
    {
        walls[1][y] = true;
    }

    auto makePipe = [&](int thickness)
    {
        assert(cey + 3 < m);

        walls[1][cey + 1] = false;
        for (int x = 2; x < n; x++)
        {
            walls[x][cey] = true;
            walls[x][cey + 3] = true;
            walls[x][cey + 2] = x - 2 > thickness;
        }
        cey += 3;
    };

    auto closeStair = [&](int thickness)
    {
        walls[1][ey - 2 * thickness - 1] = true;
    };

    if (testSpec.endPipe)
    {
        makePipe(0);
    }

    int numThicks = testSpec.numThickPipes;

    if (testSpec.doubleGap)
    {
        numThicks = 2;
    }

    std::vector<int> thicks;
    for (int i = 0; i < numThicks; i++)
    {
        int thickness = randNum(4, m / 3 - 4);
        if (std::count(thicks.begin(), thicks.end(), thickness))
        {
            i--;
            continue;
        }

        thicks.push_back(thickness);
    }

    std::sort(thicks.begin(), thicks.end());

    if (!testSpec.doubleGap)
    {
        for (int thickness : thicks)
        {
            makePipe(thickness);
            closeStair(thickness);
        }
    }
    else
    {
        while (thicks[1] < thicks[0] + 2) thicks[1]++;

        makePipe(thicks[0]);
        closeStair(thicks[0]);

        int th = 0;
        while (cey + 6 < m && th < thicks[0] - 1)
        {
            makePipe(th);
            th++;
        }

        makePipe(thicks[1] - 1);
        closeStair(thicks[1]);
    }
}

void genLogsieve(std::vector<std::vector<bool>>& walls, int& k, const TestSpec& testSpec)
{
    int n = testSpec.n;
    int m = testSpec.m;

    bool wall = testSpec.logSieveWall;

    k = randNum(0, m);

    for (int i = 0; i < n; i++)
    {
        if (!wall)
        {
            if (i % 2 == 0)
            {
                continue;
            }
        }
        else
        {
            if (i % 3 <= 1)
            {
                continue;
            }

        }

        bool flip = randNum(0, 2);

        int pos = 0;
        for (int gap = 1; ; gap *= 2)
        {
            int prevPos = pos;
            pos += gap;

            if (pos >= m)
            {
                for (int p = prevPos; p < m; p++)
                {
                    if (wall) walls[i - 1][!flip ? p : (m - p - 1)] = true;
                    walls[i][!flip ? p : (m - p - 1)] = true;
                }
                break;
            }

            if (wall) walls[i - 1][!flip ? pos : (m - pos - 1)] = true;
            walls[i][!flip ? pos : (m - pos - 1)] = true;

            if (wall)
            {
                for (int p = prevPos + 1; p < pos; p++)
                {
                    if (wall) walls[i - 1][!flip ? p : (m - p - 1)] = true;
                }
            }

            pos++;
        }
    }
}

void genCup(std::vector<std::vector<bool>>& walls, int& k, const TestSpec& testSpec)
{
    int n = testSpec.n;
    int m = testSpec.m;

    k = randNum(0, m);

    for (int i = 1; i < n; i++)
    {
        walls[i][1] = true;
        walls[i][m - 2] = true;
    }

    for (int i = 1; i < m - 1; i++)
    {
        walls[n - 1][i] = true;
    }
}

void genValve2(std::vector<std::vector<bool>>& walls, int& k, const TestSpec& testSpec)
{
    int n = testSpec.n;
    int m = testSpec.m;

    k = randNum(0, m);

    int vLip = m - testSpec.nonValveLen - 1;

    for (int j = 1; j <= vLip; j++)
    {
        walls[1][j] = true;
    }
    walls[2][1] = true;

    for (int i = 3; i < n; i += 2)
    {
        int row = i;
        bool skipRow = false;
        for (int j = vLip; j > 0; j--)
        {
            if (walls[row - 1][j - 1])
            {
                row++;
                if (j > vLip / 10)
                {
                    skipRow = true;
                }
            }
            if (row < n) walls[row][j] = true;
        }
        if (skipRow)
        {
            if (i + 2 < n) walls[i + 2][vLip] = true;
            i++;
            if (i + 3 < n) walls[i + 3][1] = true;
        }
    }
}

void genTest(std::ofstream& outfile, const TestSpec& testSpec)
{
    int n = testSpec.n;
    int m = testSpec.m;

    std::vector<std::vector<bool>> walls(
        n + 1, std::vector<bool>(m, false)
    );

    int k;

    if (testSpec.testType == TT_RAND) genRand(walls, k, testSpec);
    else if (testSpec.testType == TT_VALVE) genValve(walls, k, testSpec);
    else if (testSpec.testType == TT_SIEVE) genSieve(walls, k, testSpec);
    else if (testSpec.testType == TT_STAIRS) genStairs(walls, k, testSpec);
    else if (testSpec.testType == TT_LOGSIEVE) genLogsieve(walls, k, testSpec);
    else if (testSpec.testType == TT_CUP) genCup(walls, k, testSpec);
    else if (testSpec.testType == TT_VALVE2) genValve2(walls, k, testSpec);

    int seed = randNum(0, 100000);

    double leftProb = testSpec.leftProb;
    int maxCostDecr = testSpec.maxCostDecr;
    bool useDiff = testSpec.useDiff;
    bool useLen = testSpec.useLen;

    if (maxCostDecr > 0) useDiff = false;
    if (maxCostDecr > 0) useLen = false;
    if (useLen) useDiff = false;

    if (randReal(0, 1) < testSpec.flipProb)
    {
        k = m - 1 - k;
        for (int i = 0; i < n; i++)
        {
            std::reverse(walls[i].begin(), walls[i].end());
        }
    }

    outfile << seed << " " << leftProb << " " << maxCostDecr << " " << useDiff << " " << useLen << "\n";

    outfile << n << " " << m << " " << k << "\n";

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j += BITS)
        {
            int code = 0;
            for (int j2 = 0; j2 < BITS && j + j2 < m; j2++)
            {
                code |= ((int) walls[i][j + j2]) << j2;
            }
            outfile << (char) ('0' + code);
        }
        outfile << "\n";
    }
}

const int MAX_NM = 5000;
const int SMALL_NM = 100;
const int MID_NM = 400;
const int BIG_NM = 1000;

std::vector<std::vector<TestSpec>> testSpecs = {
    {
        {
            .n = MAX_NM,
            .m = 2,
            .meanWallsPerRow = {0},
        },
        {
            .n = MAX_NM,
            .m = 2,
            .meanWallsPerRow = {0.75},
            .neiWallProb = 0,
        },
        {
            .n = MAX_NM,
            .m = 2,
            .meanWallsPerRow = {1.25},
            .neiWallProb = 0,
        },
    },

    {
        {
            .n = MAX_NM / 2,
            .m = MAX_NM,
            .meanWallsPerRow = {0},
        },
        {
            .n = MAX_NM,
            .m = MAX_NM / 2,
            .meanWallsPerRow = {0},
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {0},
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {0},
            .leftProb = 1,
            .useDiff = false,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {0},
            .leftProb = 0,
            .useDiff = false,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {0},
            .maxCostDecr = 2,
        },
    },

    {
        {
            .n = SMALL_NM / 2,
            .m = SMALL_NM,
            .meanWallsPerRow = {6},
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM / 2,
            .meanWallsPerRow = {6},
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .testType = TT_CUP
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .testType = TT_LOGSIEVE,
            .logSieveWall = true,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .testType = TT_LOGSIEVE,
            .logSieveWall = false,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .testType = TT_STAIRS,
            .endPipe = false,
            .doubleGap = true,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .testType = TT_STAIRS,
            .endPipe = true,
            .doubleGap = true,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .testType = TT_STAIRS,
            .endPipe = false,
            .doubleGap = true,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .testType = TT_STAIRS,
            .endPipe = true,
            .doubleGap = true,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .testType = TT_STAIRS,
            .endPipe = false,
            .numThickPipes = 0,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .testType = TT_STAIRS,
            .endPipe = true,
            .numThickPipes = 0,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .testType = TT_STAIRS,
            .endPipe = false,
            .numThickPipes = 3,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .testType = TT_STAIRS,
            .endPipe = true,
            .numThickPipes = 3,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .testType = TT_STAIRS,
            .endPipe = false,
            .numThickPipes = SMALL_NM / 9,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .testType = TT_STAIRS,
            .endPipe = true,
            .numThickPipes = SMALL_NM / 9 - 1,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .testType = TT_STAIRS,
            .endPipe = false,
            .numThickPipes = 0,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .testType = TT_STAIRS,
            .endPipe = true,
            .numThickPipes = 0,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .testType = TT_STAIRS,
            .endPipe = false,
            .numThickPipes = 3,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .testType = TT_STAIRS,
            .endPipe = true,
            .numThickPipes = 3,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .testType = TT_STAIRS,
            .endPipe = false,
            .numThickPipes = SMALL_NM / 9,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .testType = TT_STAIRS,
            .endPipe = true,
            .numThickPipes = SMALL_NM / 9 - 1,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .testType = TT_SIEVE,
            .fracSieve = 0.4,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .testType = TT_SIEVE,
            .fracSieve = 0.4,
            .useLen = true,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .testType = TT_SIEVE,
            .fracSieve = 0.5,
            .leftProb = 0.5,
            .useDiff = false,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .testType = TT_SIEVE,
            .fracSieve = 0.6,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .testType = TT_SIEVE,
            .fracSieve = 0.6,
            .useLen = true,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .testType = TT_SIEVE,
            .fracSieve = 0.7,
            .leftProb = 0.5,
            .useDiff = false,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .testType = TT_SIEVE,
            .fracSieve = 0.8,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .testType = TT_SIEVE,
            .fracSieve = 0.8,
            .useLen = true,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .testType = TT_VALVE,
            .nonValveLen = 14,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .testType = TT_VALVE,
            .nonValveLen = 14,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .testType = TT_VALVE,
            .nonValveLen = 30,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .testType = TT_VALVE,
            .nonValveLen = 30,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .testType = TT_VALVE,
            .nonValveLen = 62,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .testType = TT_VALVE,
            .nonValveLen = 62,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .testType = TT_VALVE2,
            .nonValveLen = 14,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .testType = TT_VALVE2,
            .nonValveLen = 14,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .testType = TT_VALVE2,
            .nonValveLen = 30,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .testType = TT_VALVE2,
            .nonValveLen = 30,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .testType = TT_VALVE2,
            .nonValveLen = 62,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .testType = TT_VALVE2,
            .nonValveLen = 62,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .meanWallsPerRow = {0},
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .meanWallsPerRow = {0},
            .leftProb = 1,
            .useDiff = false,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .meanWallsPerRow = {0},
            .leftProb = 0,
            .useDiff = false,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .meanWallsPerRow = {0},
            .maxCostDecr = 2,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .meanWallsPerRow = {0, 0, 1},
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .meanWallsPerRow = {3},
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .meanWallsPerRow = {6},
            .leftProb = 1,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .meanWallsPerRow = {12},
            .neiWallProb = 0.25,
            .leftProb = 0,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .meanWallsPerRow = {24},
            .neiWallProb = 0,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .meanWallsPerRow = {1, 5, 30},
            .neiWallProb = 0.1,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .meanWallsPerRow = {0, 7, 50},
            .neiWallProb = 0,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .meanWallsPerRow = {5},
            .useDiff = false,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .meanWallsPerRow = {16},
            .neiWallProb = 0.1,
            .useDiff = false,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .meanWallsPerRow = {0, 4, 9, 22},
            .neiWallProb = 0.1,
            .useDiff = false,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .meanWallsPerRow = {1, 12, 29},
            .neiWallProb = 0,
            .useDiff = false,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .meanWallsPerRow = {2},
            .maxCostDecr = 2,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .meanWallsPerRow = {7},
            .maxCostDecr = 1,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .meanWallsPerRow = {13},
            .neiWallProb = 0.15,
            .maxCostDecr = 3,
        },
        {
            .n = SMALL_NM,
            .m = SMALL_NM,
            .meanWallsPerRow = {1, 40},
            .neiWallProb = 0,
            .maxCostDecr = 4,
        },
    },

    {
        {
            .n = MID_NM / 2,
            .m = MID_NM,
            .meanWallsPerRow = {6},
        },
        {
            .n = MID_NM,
            .m = MID_NM / 2,
            .meanWallsPerRow = {6},
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_CUP
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_LOGSIEVE,
            .logSieveWall = true,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_LOGSIEVE,
            .logSieveWall = false,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_STAIRS,
            .endPipe = false,
            .doubleGap = true,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_STAIRS,
            .endPipe = true,
            .doubleGap = true,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_STAIRS,
            .endPipe = false,
            .doubleGap = true,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_STAIRS,
            .endPipe = true,
            .doubleGap = true,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_STAIRS,
            .endPipe = false,
            .numThickPipes = 0,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_STAIRS,
            .endPipe = true,
            .numThickPipes = 0,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_STAIRS,
            .endPipe = false,
            .numThickPipes = 3,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_STAIRS,
            .endPipe = true,
            .numThickPipes = 3,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_STAIRS,
            .endPipe = false,
            .numThickPipes = MID_NM / 9,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_STAIRS,
            .endPipe = true,
            .numThickPipes = MID_NM / 9 - 1,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_STAIRS,
            .endPipe = false,
            .numThickPipes = 0,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_STAIRS,
            .endPipe = true,
            .numThickPipes = 0,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_STAIRS,
            .endPipe = false,
            .numThickPipes = 3,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_STAIRS,
            .endPipe = true,
            .numThickPipes = 3,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_STAIRS,
            .endPipe = false,
            .numThickPipes = MID_NM / 9,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_STAIRS,
            .endPipe = true,
            .numThickPipes = MID_NM / 9 - 1,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_SIEVE,
            .fracSieve = 0.4,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_SIEVE,
            .fracSieve = 0.4,
            .useLen = true,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_SIEVE,
            .fracSieve = 0.5,
            .leftProb = 0.5,
            .useDiff = false,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_SIEVE,
            .fracSieve = 0.6,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_SIEVE,
            .fracSieve = 0.6,
            .useLen = true,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_SIEVE,
            .fracSieve = 0.7,
            .leftProb = 0.5,
            .useDiff = false,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_SIEVE,
            .fracSieve = 0.8,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_SIEVE,
            .fracSieve = 0.8,
            .useLen = true,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_VALVE,
            .nonValveLen = 14,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_VALVE,
            .nonValveLen = 14,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_VALVE,
            .nonValveLen = 30,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_VALVE,
            .nonValveLen = 30,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_VALVE,
            .nonValveLen = 62,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_VALVE,
            .nonValveLen = 62,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_VALVE,
            .nonValveLen = 126,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_VALVE,
            .nonValveLen = 126,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_VALVE,
            .nonValveLen = 254,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_VALVE,
            .nonValveLen = 254,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_VALVE2,
            .nonValveLen = 14,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_VALVE2,
            .nonValveLen = 14,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_VALVE2,
            .nonValveLen = 30,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_VALVE2,
            .nonValveLen = 30,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_VALVE2,
            .nonValveLen = 62,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_VALVE2,
            .nonValveLen = 62,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_VALVE2,
            .nonValveLen = 126,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_VALVE2,
            .nonValveLen = 126,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_VALVE2,
            .nonValveLen = 254,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .testType = TT_VALVE2,
            .nonValveLen = 254,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .meanWallsPerRow = {0},
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .meanWallsPerRow = {0},
            .leftProb = 1,
            .useDiff = false,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .meanWallsPerRow = {0},
            .leftProb = 0,
            .useDiff = false,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .meanWallsPerRow = {0},
            .maxCostDecr = 2,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .meanWallsPerRow = {0, 0, 1},
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .meanWallsPerRow = {3},
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .meanWallsPerRow = {6},
            .leftProb = 1,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .meanWallsPerRow = {12},
            .neiWallProb = 0.4,
            .leftProb = 0,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .meanWallsPerRow = {24},
            .neiWallProb = 0.25,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .meanWallsPerRow = {50},
            .neiWallProb = 0.1,
            .leftProb = 0.8,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .meanWallsPerRow = {100},
            .neiWallProb = 0.05,
            .leftProb = 0.2,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .meanWallsPerRow = {200},
            .neiWallProb = 0,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .meanWallsPerRow = {300},
            .neiWallProb = 0,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .meanWallsPerRow = {1, 5, 30},
            .neiWallProb = 0.35,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .meanWallsPerRow = {0, 7, 50},
            .neiWallProb = 0.3,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .meanWallsPerRow = {3, 15, 90},
            .neiWallProb = 0.15,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .meanWallsPerRow = {0, 35, 250},
            .neiWallProb = 0,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .meanWallsPerRow = {5},
            .useDiff = false,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .meanWallsPerRow = {16},
            .neiWallProb = 0.2,
            .useDiff = false,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .meanWallsPerRow = {75},
            .neiWallProb = 0.1,
            .useDiff = false,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .meanWallsPerRow = {180},
            .neiWallProb = 00,
            .useDiff = false,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .meanWallsPerRow = {0, 4, 9, 22},
            .neiWallProb = 0.1,
            .useDiff = false,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .meanWallsPerRow = {0, 16, 36, 88},
            .neiWallProb = 0.05,
            .useDiff = false,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .meanWallsPerRow = {5, 53, 199},
            .neiWallProb = 0,
            .useDiff = false,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .meanWallsPerRow = {2},
            .maxCostDecr = 2,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .meanWallsPerRow = {7},
            .maxCostDecr = 1,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .meanWallsPerRow = {13},
            .maxCostDecr = 3,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .meanWallsPerRow = {105},
            .neiWallProb = 0.1,
            .maxCostDecr = 1,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .meanWallsPerRow = {230},
            .neiWallProb = 0,
            .maxCostDecr = 2,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .meanWallsPerRow = {1, 35},
            .neiWallProb = 0.4,
            .maxCostDecr = 4,
        },
        {
            .n = MID_NM,
            .m = MID_NM,
            .meanWallsPerRow = {2, 200},
            .neiWallProb = 0,
            .maxCostDecr = 1,
        },
    },

    {
        {
            .n = BIG_NM / 2,
            .m = BIG_NM,
            .meanWallsPerRow = {6},
        },
        {
            .n = BIG_NM,
            .m = BIG_NM / 2,
            .meanWallsPerRow = {6},
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_CUP
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_LOGSIEVE,
            .logSieveWall = true,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_LOGSIEVE,
            .logSieveWall = false,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_STAIRS,
            .endPipe = false,
            .doubleGap = true,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_STAIRS,
            .endPipe = true,
            .doubleGap = true,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_STAIRS,
            .endPipe = false,
            .doubleGap = true,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_STAIRS,
            .endPipe = true,
            .doubleGap = true,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_STAIRS,
            .endPipe = false,
            .numThickPipes = 0,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_STAIRS,
            .endPipe = true,
            .numThickPipes = 0,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_STAIRS,
            .endPipe = false,
            .numThickPipes = 3,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_STAIRS,
            .endPipe = true,
            .numThickPipes = 3,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_STAIRS,
            .endPipe = false,
            .numThickPipes = BIG_NM / 9,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_STAIRS,
            .endPipe = true,
            .numThickPipes = BIG_NM / 9 - 1,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_STAIRS,
            .endPipe = false,
            .numThickPipes = 0,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_STAIRS,
            .endPipe = true,
            .numThickPipes = 0,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_STAIRS,
            .endPipe = false,
            .numThickPipes = 3,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_STAIRS,
            .endPipe = true,
            .numThickPipes = 3,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_STAIRS,
            .endPipe = false,
            .numThickPipes = BIG_NM / 9,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_STAIRS,
            .endPipe = true,
            .numThickPipes = BIG_NM / 9 - 1,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_SIEVE,
            .fracSieve = 0.4,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_SIEVE,
            .fracSieve = 0.4,
            .useLen = true,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_SIEVE,
            .fracSieve = 0.5,
            .leftProb = 0.5,
            .useDiff = false,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_SIEVE,
            .fracSieve = 0.6,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_SIEVE,
            .fracSieve = 0.6,
            .useLen = true,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_SIEVE,
            .fracSieve = 0.7,
            .leftProb = 0.5,
            .useDiff = false,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_SIEVE,
            .fracSieve = 0.8,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_SIEVE,
            .fracSieve = 0.8,
            .useLen = true,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_VALVE,
            .nonValveLen = 30,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_VALVE,
            .nonValveLen = 30,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_VALVE,
            .nonValveLen = 62,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_VALVE,
            .nonValveLen = 62,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_VALVE,
            .nonValveLen = 126,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_VALVE,
            .nonValveLen = 126,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_VALVE,
            .nonValveLen = 254,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_VALVE,
            .nonValveLen = 254,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_VALVE,
            .nonValveLen = 510,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_VALVE,
            .nonValveLen = 510,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_VALVE2,
            .nonValveLen = 30,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_VALVE2,
            .nonValveLen = 30,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_VALVE2,
            .nonValveLen = 62,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_VALVE2,
            .nonValveLen = 62,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_VALVE2,
            .nonValveLen = 126,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_VALVE2,
            .nonValveLen = 126,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_VALVE2,
            .nonValveLen = 254,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_VALVE2,
            .nonValveLen = 254,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_VALVE2,
            .nonValveLen = 510,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .testType = TT_VALVE2,
            .nonValveLen = 510,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .meanWallsPerRow = {0},
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .meanWallsPerRow = {0},
            .leftProb = 1,
            .useDiff = false,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .meanWallsPerRow = {0},
            .leftProb = 0,
            .useDiff = false,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .meanWallsPerRow = {0},
            .maxCostDecr = 2,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .meanWallsPerRow = {0, 0, 1},
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .meanWallsPerRow = {3},
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .meanWallsPerRow = {6},
            .leftProb = 1,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .meanWallsPerRow = {12},
            .neiWallProb = 0.4,
            .leftProb = 0,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .meanWallsPerRow = {24},
            .neiWallProb = 0.25,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .meanWallsPerRow = {50},
            .neiWallProb = 0.1,
            .leftProb = 0.8,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .meanWallsPerRow = {100},
            .neiWallProb = 0.05,
            .leftProb = 0.2,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .meanWallsPerRow = {250},
            .neiWallProb = 0,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .meanWallsPerRow = {500},
            .neiWallProb = 0,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .meanWallsPerRow = {1, 6, 50},
            .neiWallProb = 0.35,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .meanWallsPerRow = {0, 10, 80},
            .neiWallProb = 0.3,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .meanWallsPerRow = {4, 20, 150},
            .neiWallProb = 0.15,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .meanWallsPerRow = {0, 60, 650},
            .neiWallProb = 0,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .meanWallsPerRow = {5},
            .useDiff = false,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .meanWallsPerRow = {19},
            .neiWallProb = 0.2,
            .useDiff = false,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .meanWallsPerRow = {95},
            .neiWallProb = 0.1,
            .useDiff = false,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .meanWallsPerRow = {240},
            .neiWallProb = 00,
            .useDiff = false,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .meanWallsPerRow = {0, 6, 12, 29},
            .neiWallProb = 0.1,
            .useDiff = false,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .meanWallsPerRow = {0, 23, 55, 166},
            .neiWallProb = 0.05,
            .useDiff = false,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .meanWallsPerRow = {9, 111, 555},
            .neiWallProb = 0,
            .useDiff = false,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .meanWallsPerRow = {2},
            .maxCostDecr = 2,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .meanWallsPerRow = {10},
            .maxCostDecr = 1,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .meanWallsPerRow = {25},
            .maxCostDecr = 3,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .meanWallsPerRow = {133},
            .neiWallProb = 0.1,
            .maxCostDecr = 1,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .meanWallsPerRow = {299},
            .neiWallProb = 0,
            .maxCostDecr = 2,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .meanWallsPerRow = {1, 50},
            .neiWallProb = 0.4,
            .maxCostDecr = 4,
        },
        {
            .n = BIG_NM,
            .m = BIG_NM,
            .meanWallsPerRow = {2, 440},
            .neiWallProb = 0,
            .maxCostDecr = 1,
        },
    },

    {
        {
            .n = MAX_NM / 2,
            .m = MAX_NM,
            .meanWallsPerRow = {6},
        },
        {
            .n = MAX_NM,
            .m = MAX_NM / 2,
            .meanWallsPerRow = {6},
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_CUP
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_LOGSIEVE,
            .logSieveWall = true,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_LOGSIEVE,
            .logSieveWall = false,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_STAIRS,
            .endPipe = false,
            .doubleGap = true,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_STAIRS,
            .endPipe = true,
            .doubleGap = true,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_STAIRS,
            .endPipe = false,
            .doubleGap = true,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_STAIRS,
            .endPipe = true,
            .doubleGap = true,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_STAIRS,
            .endPipe = false,
            .numThickPipes = 0,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_STAIRS,
            .endPipe = true,
            .numThickPipes = 0,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_STAIRS,
            .endPipe = false,
            .numThickPipes = 3,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_STAIRS,
            .endPipe = true,
            .numThickPipes = 3,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_STAIRS,
            .endPipe = false,
            .numThickPipes = MAX_NM / 9,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_STAIRS,
            .endPipe = true,
            .numThickPipes = MAX_NM / 9 - 1,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_STAIRS,
            .endPipe = false,
            .numThickPipes = 0,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_STAIRS,
            .endPipe = true,
            .numThickPipes = 0,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_STAIRS,
            .endPipe = false,
            .numThickPipes = 3,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_STAIRS,
            .endPipe = true,
            .numThickPipes = 3,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_STAIRS,
            .endPipe = false,
            .numThickPipes = MAX_NM / 9,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_STAIRS,
            .endPipe = true,
            .numThickPipes = MAX_NM / 9 - 1,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_SIEVE,
            .fracSieve = 0.4,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_SIEVE,
            .fracSieve = 0.4,
            .useLen = true,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_SIEVE,
            .fracSieve = 0.5,
            .leftProb = 0.5,
            .useDiff = false,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_SIEVE,
            .fracSieve = 0.6,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_SIEVE,
            .fracSieve = 0.6,
            .useLen = true,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_SIEVE,
            .fracSieve = 0.7,
            .leftProb = 0.5,
            .useDiff = false,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_SIEVE,
            .fracSieve = 0.8,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_SIEVE,
            .fracSieve = 0.8,
            .useLen = true,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_VALVE,
            .nonValveLen = 126,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_VALVE,
            .nonValveLen = 126,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_VALVE,
            .nonValveLen = 254,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_VALVE,
            .nonValveLen = 254,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_VALVE,
            .nonValveLen = 510,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_VALVE,
            .nonValveLen = 510,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_VALVE,
            .nonValveLen = 1022,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_VALVE,
            .nonValveLen = 1022,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_VALVE,
            .nonValveLen = 2046,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_VALVE,
            .nonValveLen = 2046,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_VALVE2,
            .nonValveLen = 126,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_VALVE2,
            .nonValveLen = 126,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_VALVE2,
            .nonValveLen = 254,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_VALVE2,
            .nonValveLen = 254,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_VALVE2,
            .nonValveLen = 510,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_VALVE2,
            .nonValveLen = 510,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_VALVE2,
            .nonValveLen = 1022,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_VALVE2,
            .nonValveLen = 1022,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_VALVE2,
            .nonValveLen = 2046,
            .flipProb = 0,
            .useLen = true,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .testType = TT_VALVE2,
            .nonValveLen = 2046,
            .flipProb = 1,
            .useLen = true,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {0, 0, 1},
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {3},
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {6},
            .leftProb = 1,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {12},
            .neiWallProb = 0.4,
            .leftProb = 0,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {24},
            .neiWallProb = 0.25,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {50},
            .neiWallProb = 0.1,
            .leftProb = 0.8,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {100},
            .leftProb = 0.2,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {200},
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {600},
            .neiWallProb = 0,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {1200},
            .neiWallProb = 0.1,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {2400},
            .neiWallProb = 0,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {1, 5, 30},
            .neiWallProb = 0.35,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {0, 7, 50},
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {3, 15, 90},
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {0, 35, 250},
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {33, 169, 999},
            .neiWallProb = 0.1,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {1, 350, 2000},
            .neiWallProb = 0,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {5, 150, 3000},
            .neiWallProb = 0,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {5},
            .useDiff = false,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {16},
            .neiWallProb = 0.2,
            .useDiff = false,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {75},
            .neiWallProb = 0.1,
            .useDiff = false,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {180},
            .useDiff = false,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {500},
            .neiWallProb = 0.2,
            .useDiff = false,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {1800},
            .useDiff = false,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {0, 4, 9, 22},
            .useDiff = false,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {0, 16, 36, 88},
            .useDiff = false,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {5, 53, 199},
            .neiWallProb = 0.3,
            .useDiff = false,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {0, 10, 600},
            .neiWallProb = 0.1,
            .useDiff = false,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {50, 530, 1990},
            .neiWallProb = 0,
            .useDiff = false,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {2},
            .maxCostDecr = 2,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {7},
            .maxCostDecr = 1,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {13},
            .maxCostDecr = 3,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {105},
            .neiWallProb = 0.1,
            .maxCostDecr = 1,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {230},
            .neiWallProb = 0,
            .maxCostDecr = 2,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {999},
            .neiWallProb = 0,
            .maxCostDecr = 4,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {2500},
            .neiWallProb = 0,
            .maxCostDecr = 1,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {1, 35},
            .maxCostDecr = 4,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {5, 101},
            .maxCostDecr = 2,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {15, 399},
            .neiWallProb = 0.3,
            .maxCostDecr = 4,
        },
        {
            .n = MAX_NM,
            .m = MAX_NM,
            .meanWallsPerRow = {1, 33, 999},
            .neiWallProb = 0.1,
            .maxCostDecr = 1,
        },
    },
};

static const std::string PASSWORD = "4a89cneic0qkd0gd";
static const std::string CORRECT = "CORRECT";

int main()
{
    std::ios::sync_with_stdio(false);

    int num = 1;

    for (int st = 0; st < (int) testSpecs.size(); st++)
    {
        std::vector<int> perm(testSpecs[st].size());
        std::iota(perm.begin(), perm.end(), 0);
        std::shuffle(perm.begin(), perm.end(), generator);

        std::cout << std::endl;
        std::cout << "Subtask " << st + 1 << ":" << std::endl;
        std::cout << num << " - " << num + testSpecs[st].size() - 1 << std::endl;

        for (int i : perm)
        {
            std::string nums = "";
            nums += (num / 100 + '0');
            nums += (num / 10 % 10 + '0');
            nums += (num % 10 + '0');

            std::ofstream dummySol("tunnels." + nums + ".sol");

            dummySol << PASSWORD << std::endl;
            dummySol << CORRECT << std::endl;

            std::ofstream outfile("tunnels." + nums + ".in");

            const TestSpec& ts = testSpecs[st][i];

            std::cout << std::endl;
            std::cout << "Test " << num << ":" << std::endl;
            ts.printDescr();

            genTest(outfile, ts);

            num++;
        }
    }

    return 0;
}
