#include <iostream>
#include <fstream>
#include <time.h>
#include <assert.h>
#include <algorithm>
#include <numeric>
#include <vector>
#include <unordered_set>
#include <random>

const int MAX_N = 1e6;

std::mt19937 generator;
int randNum(int lb, int ub)
{
    std::uniform_int_distribution<int> distribution(lb, ub);
    return distribution(generator);
}
bool oneIn(int chances)
{
    std::uniform_int_distribution<int> distribution(0, chances - 1);
    return distribution(generator) == 0;
}

int n;
int MAX_M;
int MAX_AB;

void genTest(std::ofstream& outfile, bool allStayLong)
{
    int m;
    if (oneIn(2)) m = randNum(1, MAX_M);
    else m = randNum(1, (MAX_M + 1) / 2);

    outfile << n << " " << m << "\n";
    for (int i = 0 ; i < n; ++i)
    {
        int a = randNum(1, MAX_AB);
        int b = randNum(1, MAX_AB);
        if (a > b) std::swap(a, b);
        if (a == b || (allStayLong && b - a < m))
        {
            --i;
            continue;
        }
        outfile << a << " " << b << "\n";
    }
}

int main()
{
    std::ios::sync_with_stdio(false);

    int num;
    while (true)
    {
        std::cout << "Test num? ";
        std::cin >> num;
        if (num < 0) return 0;
        std::cout << "MAX_N MAX_M MAX_AB? ";
        std::cin >> n >> MAX_M >> MAX_AB;
        for (int i = 0; i < 10; ++i)
        {
            std::string nums = "";
            nums += (num / 10 + '0');
            nums += (num % 10 + '0');
            std::ofstream outfile("ads." + nums + ".in");
            genTest(outfile, i % 2);
            outfile.close();

            ++num;
        }
    }
    return 0;
}
