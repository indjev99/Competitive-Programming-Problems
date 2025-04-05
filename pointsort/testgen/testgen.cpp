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

struct TestSpec
{
    int t;
    int n;
    int k;
};

std::vector<int> genPerm(int n)
{
    std::vector<int> perm(n);
    std::iota(perm.begin(), perm.end(), 0);
    std::shuffle(perm.begin(), perm.end(), generator);

    return perm;
}

std::vector<std::vector<int>> genPoints(const TestSpec& testSpec)
{
    int n = testSpec.n;
    int k = testSpec.k;

    std::vector<std::vector<int>> perms(k);

    for (int d = 0; d < k; d++)
    {
        perms[d] = genPerm(n);
    }

    std::vector<std::vector<int>> points(n);

    for (int i = 0; i < n; i++)
    {
        points[i].resize(k);
        for (int d = 0; d < k; d++)
        {
            points[i][d] = perms[d][i];
        }
    }

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

const int N = 10000;

std::vector<TestSpec> testSpecs = {
    {20, N, 2},
    {13, N, 3},
    {10, N, 4},
    {8, N, 5},
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
