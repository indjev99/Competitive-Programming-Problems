#include "pointsort.h"
#include <algorithm>
#include <iostream>

static void myAssert(bool cond, const std::string& msg)
{
    if (cond) return;

    std::cout << "Signed" << std::endl;
    std::cout << -1 << std::endl;

    exit(0);
}

static int n, k;
static std::vector<std::vector<int>> points;

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

int main()
{
    int t;
    std::cin >> t;
    std::cin >> n >> k;

    for (int i = 0; i < t; i++)
    {
        inputPoints();

        std::vector<std::vector<int>> pointsGuess = pointSort(n, k);

        myAssert(pointsGuess == points, "Wrong answer");
    }

    std::cout << "Signed" << std::endl;
    std::cout << numCmps << std::endl;

    return 0;
}
