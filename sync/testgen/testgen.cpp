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

unsigned long long randNum(unsigned long long lb, unsigned long long ub)
{
    std::uniform_int_distribution<unsigned long long> distribution(lb, ub);
    return distribution(generator);
}

void genTest(std::ofstream& outfile, int n, bool partialScoring)
{
    outfile << partialScoring << std::endl;
    outfile << n << std::endl;
}

std::vector<std::vector<int>> testSpecs = {
    {
        2,
        3,
        4,
        5,
    },
    {
        6,
        7,
        8,
        9,
        10,
        11,
        12,
        13,
        14,
        15,
    },
    {
        2,
        4,
        8,
        16,
        32,
        64,
        128,
        256,
        512,
        1024,
        2048,
    },
    {
        2 + 1,
        4 + 1,
        8 + 1,
        16 + 1,
        32 + 1,
        64 + 1,
        128 + 1,
        256 + 1,
        512 + 1,
        1024 + 1,
        2048 + 1,
    },
    {
        4 - 1,
        8 - 1,
        16 - 1,
        32 - 1,
        64 - 1,
        128 - 1,
        256 - 1,
        512 - 1,
        1024 - 1,
        2048 - 1,
    },
    {
        4 - 2,
        8 - 2,
        16 - 2,
        32 - 2,
        64 - 2,
        128 - 2,
        256 - 2,
        512 - 2,
        1024 - 2,
        2048 - 2,
    },
    {
        18,
        19,
        20,
        21,
        22,
        23,
        24,
        25,
        26,
        27,
        28,
        29,
        2500,
        2499,
        2498,
        2497,
        2496,
        2495,
        2494,
        2493,
        2492,
        2491,
        2490,
        1908,
        533,
        2414,
        1167,
        430,
        2114,
        184,
        403,
        2161,
        1211,
        728,
        2225,
        1823,
        978,
        636,
        84,
        817,
        1960,
        1549,
        1777,
    },
};

int main()
{
    std::ios::sync_with_stdio(false);

    int subtask = 1;
    int num = 1;
    for (const auto& specs : testSpecs)
    {
        bool partialScoring = subtask > 2;

        std::cerr << "Subtask " << subtask <<  ": " << num << std::endl;

        auto shuffledSpecs = specs;
        std::shuffle(shuffledSpecs.begin(), shuffledSpecs.end(), generator);

        for (int t = 0; t < (int) shuffledSpecs.size(); ++t)
        {
            std::string nums = "";
            nums += (num / 10 + '0');
            nums += (num % 10 + '0');

            std::cerr << "Test " << num << ":" << std::endl;

            std::ofstream outfile("sync." + nums + ".in");

            genTest(outfile, shuffledSpecs[t], partialScoring);

            std::ofstream dummySol("sync." + nums + ".sol");
            num++;
        }

        subtask++;
    }

    return 0;
}
