#include "pointsort.h"
#include <algorithm>
#include <iostream>
#include <random>

static void myAssert(bool cond, const std::string& msg)
{
    if (cond) return;

    std::cerr << msg << std::endl;

    exit(0);
}

static std::mt19937 generator;

static int n, k;
static std::vector<std::vector<int>> points;

static void initPoints()
{
    points.clear();
    points.resize(n);
    std::vector<int> perm(n);
    std::iota(perm.begin(), perm.end(), 0);
    for (int d = 0; d < k; ++d)
    {
        std::shuffle(perm.begin(), perm.end(), generator);
        for (int i = 0; i < n; ++i)
        {
            points[i].push_back(perm[i]);
        }
    }
}

static void inputPoints()
{
    points.clear();
    points.resize(n);
    for (int i = 0; i < n; ++i)
    {
        points[i].resize(k);
        for (int d = 0; d < k; ++d)
        {
            std::cin >> points[i][d];
        }
    }
}

static long long numCmps = 0;

std::vector<bool> compare(int i, int j)
{
    myAssert(i >= 0 && i < n && j >= 0 && j < n && i != j, "Invalid comparison");

    numCmps++;

    std::vector<bool> ans(k);
    for (int d = 0; d < k; d++)
    {
        ans[d] = points[i][d] < points[j][d];
    }
    return ans;
}

void printPoints(const std::vector<std::vector<int>>& points)
{
    for (int i = 0; i < (int) points.size(); i++)
    {
        if (i > 0) std::cerr << " ";
        std::cerr << "(";
        for (int d = 0; d < k; d++)
        {
            if (d > 0) std::cerr << " ";
            std::cerr << points[i][d];
        }
        std::cerr << ")";
    }
    std::cerr << std::endl;
}

int main()
{
    int t;
    std::cin >> t;

    bool gen;
    std::cin >> n >> k >> gen;

    for (int i = 0; i < t; i++)
    {
        if (gen)
        {
            int seed;
            std::cin >> seed;
            generator.seed(seed);

            initPoints();
        }
        else
        {
            inputPoints();
        }

        std::vector<std::vector<int>> pointsGuess = pointSort(n, k);

        myAssert(pointsGuess == points, "Wrong answer");
    }

    std::cout << (double) numCmps / t << std::endl;

    return 0;
}
