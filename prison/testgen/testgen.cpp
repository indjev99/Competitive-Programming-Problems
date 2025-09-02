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

struct TestSpec
{
    int m;
};

void genTest(std::ofstream& outfile, const TestSpec& testSpec)
{
    outfile << testSpec.m << "\n";
    outfile << randNum(0, 1e9) << "\n";
}

std::vector<TestSpec> testSpecs = {
    {700},
    {700},
    {700},
    {1100},
    {1100},
    {1100},
    {1500},
    {1500},
    {1500},
    {1900},
    {1900},
    {1900},
    {2300},
    {2300},
    {2300},
    {2700},
    {2700},
    {2700},
    {3100},
    {3100},
    {3100},
    {3500},
    {3500},
    {3500},
    {3900},
    {3900},
    {3900},
    {4300},
    {4300},
    {4300},
};

int main()
{
    std::ios::sync_with_stdio(false);

    int num = 1;

    for (int t = 0; t < (int) testSpecs.size(); t++)
    {
        std::string nums = "";
        nums += (num / 10 % 10 + '0');
        nums += (num % 10 + '0');

        std::cout << std::endl;
        std::cout << "Test " << nums << ":" << std::endl;

        std::ofstream dummySol("prison." + nums + ".sol");
        std::ofstream outfile("prison." + nums + ".in");

        genTest(outfile, testSpecs[t]);

        num++;
    }

    return 0;
}
