#include <iostream>
#include <fstream>
#include <ctime>
#include <cassert>
#include <algorithm>
#include <numeric>
#include <vector>
#include <unordered_set>
#include <random>

std::mt19937 generator;

int randNum(int lb, int ub)
{
    std::uniform_int_distribution<int> distribution(lb, ub - 1);
    return distribution(generator);
}

struct PermSpec
{
    int type;
    int rngChunks = 0;
    int cycleShift = 0;
    bool rev = false;
};

struct TestSpec
{
    int t;
    int n;
    int k;
    std::vector<PermSpec> pSpecs;
};

#define PST_RNG 0
#define PST_INC 1
#define PST_TRG 2

std::vector<int> genPerm(int n, const PermSpec& permSpec)
{
    int type = permSpec.type;
    int rngChunks = permSpec.rngChunks;
    int cycleShift = permSpec.cycleShift;
    bool rev = permSpec.rev;

    std::vector<int> rngPerm(n);
    std::iota(rngPerm.begin(), rngPerm.end(), 0);
    std::shuffle(rngPerm.begin(), rngPerm.end(), generator);

    std::vector<int> perm(n);

    for (int i = 0; i < n; i++)
    {
        int& elem = perm[i];
        switch (type)
        {
        case PST_RNG:
            elem = rngPerm[i];
            break;
        case PST_INC:
            elem = i;
            break;
        case PST_TRG:
            elem = i % 2 == 0 ? n / 2 + i / 2 : n / 2 - 1 - i / 2;
            break;
        }
    }

    if (rngChunks > 0)
    {
        for (int i = 0; i < n; i += rngChunks)
        {
            int e = std::min(i + rngChunks, n);
            std::shuffle(perm.begin() + i, perm.begin() + e, generator);
        }
    }

    if (cycleShift > 0)
    {
        std::rotate(perm.begin(), perm.begin() + cycleShift, perm.end());
    }

    if (rev)
    {
        std::reverse(perm.begin(), perm.end());
    }

    return perm;
}

std::vector<std::vector<int>> genPoints(const TestSpec& testSpec)
{
    int n = testSpec.n;
    int k = testSpec.k;

    assert((int) testSpec.pSpecs.size() == k);

    std::vector<std::vector<int>> perms(k);

    for (int d = 0; d < k; d++)
    {
        perms[d] = genPerm(n, testSpec.pSpecs[d]);
    }

    std::shuffle(perms.begin(), perms.end(), generator);

    std::vector<std::vector<int>> points(n);

    for (int i = 0; i < n; i++)
    {
        points[i].resize(k);
        for (int d = 0; d < k; d++)
        {
            points[i][d] = perms[d][i];
        }
    }

    std::shuffle(points.begin(), points.end(), generator);

    return points;
}

void genTest(std::ofstream& outfile, const TestSpec& testSpec)
{
    int t = testSpec.t;
    int n = testSpec.n;
    int k = testSpec.k;

    outfile << t << "\n";
    outfile << n << " " << k << "\n";

    for (int s = 0; s < t; s++)
    {
        std::vector<std::vector<int>> points = genPoints(testSpec);

        for (int i = 0; i < n; i++)
        {
            for (int d = 0; d < k; d++)
            {
                if (d > 0) outfile << " ";
                outfile << points[i][d];
            }
            outfile << "\n";
        }
    }
}

const int T = 5;
const int N = 10000;

std::vector<TestSpec> testSpecs = {
    {T, N, 1, {{PST_RNG}}},
    {T, N, 1, {{PST_RNG}}},
    {T, N, 1, {{PST_RNG}}},
    {T, N, 1, {{PST_RNG}}},
    {T, N, 1, {{PST_RNG}}},

    {T, N, 2, {{PST_RNG}, {PST_RNG}}},
    {T, N, 2, {{PST_RNG}, {PST_RNG}}},
    {T, N, 2, {{PST_INC}, {PST_INC}}},
    {T, N, 2, {{PST_INC}, {PST_INC, 3, N / 2}}},
    {T, N, 2, {{PST_INC}, {PST_INC, 50, 0, true}}},
    {T, N, 2, {{PST_INC}, {PST_INC, 500, N / 4, true}}},
    {T, N, 2, {{PST_INC}, {PST_TRG}}},
    {T, N, 2, {{PST_INC}, {PST_TRG, 5, N / 10, true}}},
    {T, N, 2, {{PST_INC}, {PST_TRG, 35, N * 3 / 4, true}}},
    {T, N, 2, {{PST_INC}, {PST_TRG, 90, N / 2}}},

    {T, N, 3, {{PST_RNG}, {PST_RNG}, {PST_RNG}}},
    {T, N, 3, {{PST_RNG}, {PST_RNG}, {PST_RNG}}},
    {T, N, 3, {{PST_INC}, {PST_INC}, {PST_RNG}}},
    {T, N, 3, {{PST_INC}, {PST_INC, 15, N / 3, true}, {PST_RNG}}},
    {T, N, 3, {{PST_INC}, {PST_TRG, true}, {PST_RNG}}},
    {T, N, 3, {{PST_INC}, {PST_TRG, 8, N / 2}, {PST_RNG}}},
    {T, N, 3, {{PST_INC}, {PST_INC}, {PST_INC}}},
    {T, N, 3, {{PST_INC}, {PST_INC}, {PST_TRG}}},
    {T, N, 3, {{PST_INC}, {PST_TRG, 4, N / 5}, {PST_TRG, 10, N / 2, true}}},
    {T, N, 3, {{PST_INC}, {PST_INC, 1000, N / 2, true}, {PST_TRG, 50}}},

    {T, N, 4, {{PST_RNG}, {PST_RNG}, {PST_RNG}, {PST_RNG}}},
    {T, N, 4, {{PST_RNG}, {PST_RNG}, {PST_RNG}, {PST_RNG}}},
    {T, N, 4, {{PST_INC}, {PST_INC}, {PST_RNG}, {PST_RNG}}},
    {T, N, 4, {{PST_INC}, {PST_TRG, 2, N * 9 / 10, true}, {PST_RNG}, {PST_RNG}}},
    {T, N, 4, {{PST_INC}, {PST_INC, 10, N / 3, true}, {PST_INC, 5, N / 2, true}, {PST_RNG}}},
    {T, N, 4, {{PST_INC}, {PST_TRG, 10, N * 2 / 3}, {PST_TRG, 120, 0, true}, {PST_RNG}}},
    {T, N, 4, {{PST_INC}, {PST_INC}, {PST_INC}, {PST_INC}}},
    {T, N, 4, {{PST_INC}, {PST_INC}, {PST_TRG}, {PST_TRG}}},
    {T, N, 4, {{PST_INC}, {PST_INC, 5, N / 4}, {PST_TRG, 8, 0, true}, {PST_TRG, 8, 0, true}}},
    {T, N, 4, {{PST_INC}, {PST_INC, 50, N / 2, true}, {PST_TRG, 5000, N / 6, true}, {PST_TRG, 10}}},

    {T, N, 5, {{PST_RNG}, {PST_RNG}, {PST_RNG}, {PST_RNG}, {PST_RNG}}},
    {T, N, 5, {{PST_RNG}, {PST_RNG}, {PST_RNG}, {PST_RNG}, {PST_RNG}}},
    {T, N, 5, {{PST_INC}, {PST_INC, 2, N / 20}, {PST_RNG}, {PST_RNG}, {PST_RNG}}},
    {T, N, 5, {{PST_INC}, {PST_TRG, 25, N / 2}, {PST_RNG}, {PST_RNG}, {PST_RNG}}},
    {T, N, 5, {{PST_INC}, {PST_INC, 100, N / 3}, {PST_INC, 25, N / 2, true}, {PST_RNG}, {PST_RNG}}},
    {T, N, 5, {{PST_INC}, {PST_TRG, 55, N / 2}, {PST_TRG, 5, N / 5, true}, {PST_RNG}, {PST_RNG}}},
    {T, N, 5, {{PST_INC}, {PST_INC, 10, N * 5 / 6}, {PST_TRG, 5, N / 4, true}, {PST_TRG}, {PST_RNG}}},
    {T, N, 5, {{PST_INC}, {PST_INC, 25, N / 3, true}, {PST_INC, 3, N * 3 / 4}, {PST_TRG, 5}, {PST_RNG}}},
    {T, N, 5, {{PST_INC}, {PST_INC, 15, N / 2}, {PST_INC, 2, N / 3, true}, {PST_TRG, 10, N / 10}, {PST_TRG, 60, N * 7 / 9, true}}},
    {T, N, 5, {{PST_INC}, {PST_INC, 500, N / 6, true}, {PST_TRG, 300, N / 2}, {PST_TRG, 5, N * 4 / 5}, {PST_TRG, 2, N / 3, true}}},
};

int main()
{
    std::ios::sync_with_stdio(false);

    int num = 1;
    for (const TestSpec& ts : testSpecs)
    {
        std::string nums = "";
        nums += (num / 10 + '0');
        nums += (num % 10 + '0');

        std::cerr << "Test " << num << ":" << std::endl;

        std::ofstream outfile("pointsort." + nums + ".in");

        genTest(outfile, ts);

        std::ofstream dummySol("pointsort." + nums + ".sol");

        num++;
    }

    return 0;
}
