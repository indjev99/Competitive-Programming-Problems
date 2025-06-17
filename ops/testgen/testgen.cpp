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

constexpr int MODE_RNG = 0;
constexpr int MODE_SRT = 1;
constexpr int MODE_REV = 2;
constexpr int MODE_SRT_ALMOST = 3;
constexpr int MODE_REV_ALMOST = 4;
constexpr int MODE_REV_ALT = 5;
constexpr int MODE_REV_ALT_ALMOST = 6;

struct TestSpec
{
    int minN;
    int maxN;
    unsigned long long minV;
    unsigned long long maxV;
    int mode = MODE_RNG;
    int alt = 1;
};

void genTest(std::ofstream& outfile, const TestSpec& testSpec)
{
    int minN = testSpec.minN;
    int maxN = testSpec.maxN;
    unsigned long long minV = testSpec.minV;
    unsigned long long maxV = testSpec.maxV;
    int mode = testSpec.mode;

    int n = randNum(minN, maxN);

    std::vector<unsigned long long> vals(n);

    for (int i = 0; i < n; i++)
    {
        vals[i] = randNum(minV, maxV);
        if (n == 2 && i == 1 && minV < maxV && vals[i] == vals[0])
        {
            i--;
        }
    }

    if (mode == MODE_SRT || mode == MODE_SRT_ALMOST)
    {
        std::sort(vals.begin(), vals.end());
    }
    else if (mode == MODE_REV || mode == MODE_REV_ALMOST)
    {
        std::sort(vals.begin(), vals.end());
        std::reverse(vals.begin(), vals.end());
    }
    else if (mode == MODE_REV_ALT || mode == MODE_REV_ALT_ALMOST)
    {
        std::sort(vals.begin(), vals.end());
        std::reverse(vals.begin(), vals.end());
        std::vector<unsigned long long> oldVals = vals;

        std::vector<int> offsets(testSpec.alt);
        std::iota(offsets.begin(), offsets.end(), 0);
        std::shuffle(offsets.begin(), offsets.end(), generator);

        int i = 0;
        for (int offset : offsets)
        {
            for (int j = offset; j < n; j += testSpec.alt)
            {
                vals[j] = oldVals[i];
                i++;
            }
        }
    }

    if (mode == MODE_SRT_ALMOST || mode == MODE_REV_ALMOST || mode == MODE_REV_ALT_ALMOST)
    {
        int swaps = randNum(0, n / 4);
        for (int i = 0; i < swaps; i++)
        {
            int p1 = randNum(0, n - 1);
            int p2 = randNum(0, n - 1);
            std::swap(vals[p1], vals[p2]);
        }
    }

    outfile << n << std::endl;
    for (int i = 0; i < n; i++)
    {
        if (i > 0) outfile << " ";
        outfile << vals[i];
    }
    outfile << std::endl;
}

const unsigned long long MAX_V = 1000000000000000000ULL;
const unsigned long long MAX_SV = 200;

const int MAX_SN = 150;
const int MAX_N = 350;

std::vector<std::pair<int, std::vector<TestSpec>>> testSpecs = {
    {2, {
        {2, 2, 0, MAX_SV},
        {2, 2, 0, MAX_SV},
        {2, 2, 0, MAX_SV},
        {2, 2, 0, MAX_SV},
        {2, 2, 0, MAX_SV},
        {2, 2, 0, MAX_SV},
        {2, 2, 0, MAX_SV},
        {2, 2, 0, MAX_SV},
        {2, 2, 0, MAX_SV},
        {2, 2, 0, MAX_SV},
        {2, 2, 0, MAX_SV},
        {2, 2, 0, MAX_SV},

        {2, 2, 0, 0},
        {2, 2, MAX_SV, MAX_SV},
        {2, 2, MAX_SV - 1, MAX_SV - 1},

        {2, 2, MAX_SV - 1, MAX_SV},
        {2, 2, 0, 1},
        {2, 2, 1, 2},
        {2, 2, 3, 4},
        {2, 2, 7, 8},
        {2, 2, 15, 16},
        {2, 2, 31, 32},
        {2, 2, 63, 64},
        {2, 2, 127, 128},

        {2, 2, (1ull << 7), (1ull << 7) + (1ull << 3)},
        {2, 2, (1ull << 6), (1ull << 6) + (1ull << 3)},
    }},

    {2, {
        {2, 2, 0, MAX_V},
        {2, 2, 0, MAX_V},
        {2, 2, 0, MAX_V},
        {2, 2, 0, MAX_V},
        {2, 2, 0, MAX_V},
        {2, 2, 0, MAX_V},
        {2, 2, 0, MAX_V},
        {2, 2, 0, MAX_V},
        {2, 2, 0, MAX_V},
        {2, 2, 0, MAX_V},
        {2, 2, 0, MAX_V},
        {2, 2, 0, MAX_V},

        {2, 2, 0, 0},
        {2, 2, MAX_V, MAX_V},
        {2, 2, (1ull << 59) - 1, (1ull << 59) - 1},

        {2, 2, MAX_V - 1, MAX_V},
        {2, 2, 0, 1},
        {2, 2, 1, 2},
        {2, 2, 3, 4},
        {2, 2, 7, 8},
        {2, 2, 15, 16},
        {2, 2, 31, 32},
        {2, 2, 63, 64},
        {2, 2, 127, 128},
        {2, 2, 254, 256},
        {2, 2, 511, 512},
        {2, 2, 1023, 1024},
        {2, 2, 2047, 2048},
        {2, 2, 4095, 4096},

        {2, 2, (1ull << 15) - 1, (1ull << 15)},
        {2, 2, (1ull << 26) - 1, (1ull << 26)},
        {2, 2, (1ull << 37) - 1, (1ull << 37)},
        {2, 2, (1ull << 48) - 1, (1ull << 48)},
        {2, 2, (1ull << 59) - 1, (1ull << 59)},

        {2, 2, (1ull << 7), (1ull << 7) + (1ull << 3)},
        {2, 2, (1ull << 15), (1ull << 15) + (1ull << 7)},
        {2, 2, (1ull << 26), (1ull << 26) + (1ull << 13)},
        {2, 2, (1ull << 37), (1ull << 37) + (1ull << 18)},
        {2, 2, (1ull << 48), (1ull << 48) + (1ull << 24)},
        {2, 2, (1ull << 59), (1ull << 59) + (1ull << 30)},

        {2, 2, (1ull << 6), (1ull << 6) + (1ull << 3)},
        {2, 2, (1ull << 14), (1ull << 14) + (1ull << 7)},
        {2, 2, (1ull << 25), (1ull << 25) + (1ull << 13)},
        {2, 2, (1ull << 36), (1ull << 36) + (1ull << 18)},
        {2, 2, (1ull << 47), (1ull << 47) + (1ull << 24)},
        {2, 2, (1ull << 58), (1ull << 58) + (1ull << 30)},
    }},

    {MAX_SN, {
        {MAX_SN, MAX_SN, 0, MAX_V},
        {MAX_SN, MAX_SN, 0, MAX_V},
        {MAX_SN, MAX_SN, 0, MAX_V},
        {MAX_SN, MAX_SN, 0, MAX_V},
        {MAX_SN, MAX_SN, 0, MAX_V},
        {MAX_SN, MAX_SN, 0, MAX_V},
        {MAX_SN, MAX_SN, 0, MAX_V},
        {MAX_SN, MAX_SN, 0, MAX_V},
        {MAX_SN, MAX_SN, 0, MAX_V},
        {MAX_SN, MAX_SN, 0, MAX_V},
        {MAX_SN, MAX_SN, 0, MAX_V},
        {MAX_SN, MAX_SN, 0, MAX_V},
        {MAX_SN, MAX_SN, 0, MAX_V},
        {MAX_SN, MAX_SN, 0, MAX_V},
        {MAX_SN, MAX_SN, 0, MAX_V},
        {MAX_SN, MAX_SN, 0, MAX_V},
        {MAX_SN, MAX_SN, 0, MAX_V},
        {MAX_SN, MAX_SN, 0, MAX_V},
        {MAX_SN, MAX_SN, 0, MAX_V},
        {MAX_SN, MAX_SN, 0, MAX_V},

        {MAX_SN, MAX_SN, 0, MAX_V, MODE_SRT},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT, 2},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT, 3},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT, 4},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT, 5},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT, 6},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT, 7},

        {MAX_SN, MAX_SN, 0, MAX_V, MODE_SRT_ALMOST},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_SRT_ALMOST},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALMOST},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALMOST},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALMOST},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALMOST},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},

        {MAX_SN, MAX_SN, 0, MAX_V, MODE_SRT_ALMOST},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_SRT_ALMOST},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALMOST},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALMOST},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALMOST},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALMOST},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},

        {MAX_SN, MAX_SN, 0, MAX_V, MODE_SRT_ALMOST},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_SRT_ALMOST},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALMOST},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALMOST},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALMOST},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALMOST},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},

        {MAX_SN, MAX_SN, 0, 512},
        {MAX_SN, MAX_SN, 0, 512},
        {MAX_SN, MAX_SN, 0, 512},
        {MAX_SN, MAX_SN, 0, 512},
        {MAX_SN, MAX_SN, 0, 512},
        {MAX_SN, MAX_SN, 0, 512},
        {MAX_SN, MAX_SN, 0, 512},
        {MAX_SN, MAX_SN, 0, 512},

        {MAX_SN, MAX_SN, 0, 512, MODE_SRT},
        {MAX_SN, MAX_SN, 0, 512, MODE_REV},
        {MAX_SN, MAX_SN, 0, 512, MODE_REV_ALT, 2},
        {MAX_SN, MAX_SN, 0, 512, MODE_REV_ALT, 3},
        {MAX_SN, MAX_SN, 0, 512, MODE_REV_ALT, 4},
        {MAX_SN, MAX_SN, 0, 512, MODE_REV_ALT, 5},
        {MAX_SN, MAX_SN, 0, 512, MODE_REV_ALT, 6},
        {MAX_SN, MAX_SN, 0, 512, MODE_REV_ALT, 7},
        {MAX_SN, MAX_SN, 0, 512, MODE_SRT_ALMOST},
        {MAX_SN, MAX_SN, 0, 512, MODE_REV_ALMOST},
        {MAX_SN, MAX_SN, 0, 512, MODE_REV_ALMOST},
        {MAX_SN, MAX_SN, 0, 512, MODE_REV_ALT_ALMOST, 2},
        {MAX_SN, MAX_SN, 0, 512, MODE_REV_ALT_ALMOST, 2},
        {MAX_SN, MAX_SN, 0, 512, MODE_REV_ALT_ALMOST, 3},
        {MAX_SN, MAX_SN, 0, 512, MODE_REV_ALT_ALMOST, 3},
        {MAX_SN, MAX_SN, 0, 512, MODE_REV_ALT_ALMOST, 4},
        {MAX_SN, MAX_SN, 0, 512, MODE_REV_ALT_ALMOST, 4},
        {MAX_SN, MAX_SN, 0, 512, MODE_REV_ALT_ALMOST, 5},
        {MAX_SN, MAX_SN, 0, 512, MODE_REV_ALT_ALMOST, 5},
        {MAX_SN, MAX_SN, 0, 512, MODE_REV_ALT_ALMOST, 6},
        {MAX_SN, MAX_SN, 0, 512, MODE_REV_ALT_ALMOST, 6},
        {MAX_SN, MAX_SN, 0, 512, MODE_REV_ALT_ALMOST, 7},
        {MAX_SN, MAX_SN, 0, 512, MODE_REV_ALT_ALMOST, 7},

        {MAX_SN, MAX_SN, 0, 0},
        {MAX_SN, MAX_SN, MAX_V, MAX_V},
        {MAX_SN, MAX_SN, (1ull << 59) - 1, (1ull << 59) - 1},
        {MAX_SN, MAX_SN, 0, 1},
        {MAX_SN, MAX_SN, MAX_V - 1, MAX_V},

        {MAX_SN, MAX_SN, (1ull << 3) - 1, (1ull << 3)},
        {MAX_SN, MAX_SN, (1ull << 7) - 1, (1ull << 7)},
        {MAX_SN, MAX_SN, (1ull << 15) - 1, (1ull << 15)},
        {MAX_SN, MAX_SN, (1ull << 26) - 1, (1ull << 26)},
        {MAX_SN, MAX_SN, (1ull << 37) - 1, (1ull << 37)},
        {MAX_SN, MAX_SN, (1ull << 48) - 1, (1ull << 48)},
        {MAX_SN, MAX_SN, (1ull << 59) - 1, (1ull << 59)},

        {MAX_SN, MAX_SN, (1ull << 3), (1ull << 3) + (1ull << 2)},
        {MAX_SN, MAX_SN, (1ull << 7), (1ull << 7) + (1ull << 3)},
        {MAX_SN, MAX_SN, (1ull << 15), (1ull << 15) + (1ull << 7)},
        {MAX_SN, MAX_SN, (1ull << 26), (1ull << 26) + (1ull << 13)},
        {MAX_SN, MAX_SN, (1ull << 37), (1ull << 37) + (1ull << 18)},
        {MAX_SN, MAX_SN, (1ull << 48), (1ull << 48) + (1ull << 24)},
        {MAX_SN, MAX_SN, (1ull << 59), (1ull << 59) + (1ull << 30)},

        {MAX_SN, MAX_SN, (1ull << 6), (1ull << 6) + (1ull << 3)},
        {MAX_SN, MAX_SN, (1ull << 14), (1ull << 14) + (1ull << 7)},
        {MAX_SN, MAX_SN, (1ull << 25), (1ull << 25) + (1ull << 13)},
        {MAX_SN, MAX_SN, (1ull << 36), (1ull << 36) + (1ull << 18)},
        {MAX_SN, MAX_SN, (1ull << 47), (1ull << 47) + (1ull << 24)},
        {MAX_SN, MAX_SN, (1ull << 58), (1ull << 58) + (1ull << 30)},
    }},

    {MAX_SN, {
        {4, MAX_SN, 0, MAX_V},
        {4, MAX_SN, 0, MAX_V},
        {4, MAX_SN, 0, MAX_V},
        {4, MAX_SN, 0, MAX_V},
        {4, MAX_SN, 0, MAX_V},
        {4, MAX_SN, 0, MAX_V},
        {4, MAX_SN, 0, MAX_V},
        {4, MAX_SN, 0, MAX_V},
        {4, MAX_SN, 0, MAX_V},
        {4, MAX_SN, 0, MAX_V},
        {4, MAX_SN, 0, MAX_V},
        {4, MAX_SN, 0, MAX_V},
        {4, MAX_SN, 0, MAX_V},
        {4, MAX_SN, 0, MAX_V},
        {4, MAX_SN, 0, MAX_V},
        {4, MAX_SN, 0, MAX_V},
        {4, MAX_SN, 0, MAX_V},
        {4, MAX_SN, 0, MAX_V},
        {4, MAX_SN, 0, MAX_V},
        {4, MAX_SN, 0, MAX_V},

        {MAX_SN, MAX_SN, 0, MAX_V},
        {MAX_SN, MAX_SN, 0, MAX_V},
        {MAX_SN, MAX_SN, 0, MAX_V},
        {MAX_SN, MAX_SN, 0, MAX_V},
        {MAX_SN, MAX_SN, 0, MAX_V},
        {MAX_SN, MAX_SN, 0, MAX_V},
        {MAX_SN, MAX_SN, 0, MAX_V},
        {MAX_SN, MAX_SN, 0, MAX_V},
        {MAX_SN, MAX_SN, 0, MAX_V},
        {MAX_SN, MAX_SN, 0, MAX_V},
        {MAX_SN, MAX_SN, 0, MAX_V},

        {1, 1, 0, MAX_V},
        {2, 2, 0, MAX_V},
        {3, 3, 0, MAX_V},
        {4, 4, 0, MAX_V},
        {MAX_SN - 2, MAX_SN - 2, 0, MAX_V},
        {MAX_SN - 1, MAX_SN - 1, 0, MAX_V},

        {4, MAX_SN, 0, MAX_V, MODE_SRT},
        {4, MAX_SN, 0, MAX_V, MODE_REV},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT, 2},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT, 3},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT, 4},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT, 5},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT, 6},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT, 7},
        {4, MAX_SN, 0, MAX_V, MODE_SRT_ALMOST},
        {4, MAX_SN, 0, MAX_V, MODE_SRT_ALMOST},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALMOST},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALMOST},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALMOST},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALMOST},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},

        {4, MAX_SN, 0, MAX_V, MODE_SRT},
        {4, MAX_SN, 0, MAX_V, MODE_REV},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT, 2},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT, 3},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT, 4},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT, 5},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT, 6},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT, 7},
        {4, MAX_SN, 0, MAX_V, MODE_SRT_ALMOST},
        {4, MAX_SN, 0, MAX_V, MODE_SRT_ALMOST},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALMOST},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALMOST},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALMOST},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALMOST},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},
        {4, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},

        {MAX_SN, MAX_SN, 0, MAX_V, MODE_SRT},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT, 2},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT, 3},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT, 4},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT, 5},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT, 6},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT, 7},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_SRT_ALMOST},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_SRT_ALMOST},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALMOST},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALMOST},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALMOST},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALMOST},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},
        {MAX_SN, MAX_SN, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},

        {4, MAX_SN, 0, 512},
        {4, MAX_SN, 0, 512},
        {4, MAX_SN, 0, 512},
        {4, MAX_SN, 0, 512},
        {4, MAX_SN, 0, 512},
        {4, MAX_SN, 0, 512},
        {4, MAX_SN, 0, 512},
        {4, MAX_SN, 0, 512},

        {MAX_SN, MAX_SN, 0, 512},
        {MAX_SN, MAX_SN, 0, 512},
        {MAX_SN, MAX_SN, 0, 512},

        {4, MAX_SN, 0, 512, MODE_SRT},
        {4, MAX_SN, 0, 512, MODE_REV},
        {4, MAX_SN, 0, 512, MODE_REV_ALT, 2},
        {4, MAX_SN, 0, 512, MODE_REV_ALT, 3},
        {4, MAX_SN, 0, 512, MODE_REV_ALT, 4},
        {4, MAX_SN, 0, 512, MODE_REV_ALT, 5},
        {4, MAX_SN, 0, 512, MODE_REV_ALT, 6},
        {4, MAX_SN, 0, 512, MODE_REV_ALT, 7},
        {4, MAX_SN, 0, 512, MODE_SRT_ALMOST},
        {4, MAX_SN, 0, 512, MODE_REV_ALMOST},
        {4, MAX_SN, 0, 512, MODE_REV_ALMOST},
        {4, MAX_SN, 0, 512, MODE_REV_ALT_ALMOST, 2},
        {4, MAX_SN, 0, 512, MODE_REV_ALT_ALMOST, 2},
        {4, MAX_SN, 0, 512, MODE_REV_ALT_ALMOST, 3},
        {4, MAX_SN, 0, 512, MODE_REV_ALT_ALMOST, 3},
        {4, MAX_SN, 0, 512, MODE_REV_ALT_ALMOST, 4},
        {4, MAX_SN, 0, 512, MODE_REV_ALT_ALMOST, 4},
        {4, MAX_SN, 0, 512, MODE_REV_ALT_ALMOST, 5},
        {4, MAX_SN, 0, 512, MODE_REV_ALT_ALMOST, 5},
        {4, MAX_SN, 0, 512, MODE_REV_ALT_ALMOST, 6},
        {4, MAX_SN, 0, 512, MODE_REV_ALT_ALMOST, 6},
        {4, MAX_SN, 0, 512, MODE_REV_ALT_ALMOST, 7},
        {4, MAX_SN, 0, 512, MODE_REV_ALT_ALMOST, 7},

        {4, MAX_SN, 0, 0},
        {4, MAX_SN, MAX_V, MAX_V},
        {4, MAX_SN, (1ull << 59) - 1, (1ull << 59) - 1},
        {4, MAX_SN, 0, 1},
        {4, MAX_SN, MAX_V - 1, MAX_V},

        {4, MAX_SN, (1ull << 3) - 1, (1ull << 3)},
        {4, MAX_SN, (1ull << 7) - 1, (1ull << 7)},
        {4, MAX_SN, (1ull << 15) - 1, (1ull << 15)},
        {4, MAX_SN, (1ull << 26) - 1, (1ull << 26)},
        {4, MAX_SN, (1ull << 37) - 1, (1ull << 37)},
        {4, MAX_SN, (1ull << 48) - 1, (1ull << 48)},
        {4, MAX_SN, (1ull << 59) - 1, (1ull << 59)},

        {4, MAX_SN, (1ull << 3), (1ull << 3) + (1ull << 2)},
        {4, MAX_SN, (1ull << 7), (1ull << 7) + (1ull << 3)},
        {4, MAX_SN, (1ull << 15), (1ull << 15) + (1ull << 7)},
        {4, MAX_SN, (1ull << 26), (1ull << 26) + (1ull << 13)},
        {4, MAX_SN, (1ull << 37), (1ull << 37) + (1ull << 18)},
        {4, MAX_SN, (1ull << 48), (1ull << 48) + (1ull << 24)},
        {4, MAX_SN, (1ull << 59), (1ull << 59) + (1ull << 30)},

        {4, MAX_SN, (1ull << 6), (1ull << 6) + (1ull << 3)},
        {4, MAX_SN, (1ull << 14), (1ull << 14) + (1ull << 7)},
        {4, MAX_SN, (1ull << 25), (1ull << 25) + (1ull << 13)},
        {4, MAX_SN, (1ull << 36), (1ull << 36) + (1ull << 18)},
        {4, MAX_SN, (1ull << 47), (1ull << 47) + (1ull << 24)},
        {4, MAX_SN, (1ull << 58), (1ull << 58) + (1ull << 30)},
    }},

    {MAX_N, {
        {MAX_N, MAX_N, 0, MAX_V},
        {MAX_N, MAX_N, 0, MAX_V},
        {MAX_N, MAX_N, 0, MAX_V},
        {MAX_N, MAX_N, 0, MAX_V},
        {MAX_N, MAX_N, 0, MAX_V},
        {MAX_N, MAX_N, 0, MAX_V},
        {MAX_N, MAX_N, 0, MAX_V},
        {MAX_N, MAX_N, 0, MAX_V},
        {MAX_N, MAX_N, 0, MAX_V},
        {MAX_N, MAX_N, 0, MAX_V},
        {MAX_N, MAX_N, 0, MAX_V},
        {MAX_N, MAX_N, 0, MAX_V},
        {MAX_N, MAX_N, 0, MAX_V},
        {MAX_N, MAX_N, 0, MAX_V},
        {MAX_N, MAX_N, 0, MAX_V},
        {MAX_N, MAX_N, 0, MAX_V},
        {MAX_N, MAX_N, 0, MAX_V},
        {MAX_N, MAX_N, 0, MAX_V},
        {MAX_N, MAX_N, 0, MAX_V},
        {MAX_N, MAX_N, 0, MAX_V},

        {MAX_N, MAX_N, 0, MAX_V, MODE_SRT},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT, 2},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT, 3},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT, 4},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT, 5},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT, 6},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT, 7},

        {MAX_N, MAX_N, 0, MAX_V, MODE_SRT_ALMOST},
        {MAX_N, MAX_N, 0, MAX_V, MODE_SRT_ALMOST},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALMOST},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALMOST},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALMOST},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALMOST},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},

        {MAX_N, MAX_N, 0, MAX_V, MODE_SRT_ALMOST},
        {MAX_N, MAX_N, 0, MAX_V, MODE_SRT_ALMOST},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALMOST},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALMOST},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALMOST},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALMOST},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},

        {MAX_N, MAX_N, 0, MAX_V, MODE_SRT_ALMOST},
        {MAX_N, MAX_N, 0, MAX_V, MODE_SRT_ALMOST},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALMOST},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALMOST},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALMOST},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALMOST},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},

        {MAX_N, MAX_N, 0, 512},
        {MAX_N, MAX_N, 0, 512},
        {MAX_N, MAX_N, 0, 512},
        {MAX_N, MAX_N, 0, 512},
        {MAX_N, MAX_N, 0, 512},
        {MAX_N, MAX_N, 0, 512},
        {MAX_N, MAX_N, 0, 512},
        {MAX_N, MAX_N, 0, 512},

        {MAX_N, MAX_N, 0, 512, MODE_SRT},
        {MAX_N, MAX_N, 0, 512, MODE_REV},
        {MAX_N, MAX_N, 0, 512, MODE_REV_ALT, 2},
        {MAX_N, MAX_N, 0, 512, MODE_REV_ALT, 3},
        {MAX_N, MAX_N, 0, 512, MODE_REV_ALT, 4},
        {MAX_N, MAX_N, 0, 512, MODE_REV_ALT, 5},
        {MAX_N, MAX_N, 0, 512, MODE_REV_ALT, 6},
        {MAX_N, MAX_N, 0, 512, MODE_REV_ALT, 7},
        {MAX_N, MAX_N, 0, 512, MODE_SRT_ALMOST},
        {MAX_N, MAX_N, 0, 512, MODE_REV_ALMOST},
        {MAX_N, MAX_N, 0, 512, MODE_REV_ALMOST},
        {MAX_N, MAX_N, 0, 512, MODE_REV_ALT_ALMOST, 2},
        {MAX_N, MAX_N, 0, 512, MODE_REV_ALT_ALMOST, 2},
        {MAX_N, MAX_N, 0, 512, MODE_REV_ALT_ALMOST, 3},
        {MAX_N, MAX_N, 0, 512, MODE_REV_ALT_ALMOST, 3},
        {MAX_N, MAX_N, 0, 512, MODE_REV_ALT_ALMOST, 4},
        {MAX_N, MAX_N, 0, 512, MODE_REV_ALT_ALMOST, 4},
        {MAX_N, MAX_N, 0, 512, MODE_REV_ALT_ALMOST, 5},
        {MAX_N, MAX_N, 0, 512, MODE_REV_ALT_ALMOST, 5},
        {MAX_N, MAX_N, 0, 512, MODE_REV_ALT_ALMOST, 6},
        {MAX_N, MAX_N, 0, 512, MODE_REV_ALT_ALMOST, 6},
        {MAX_N, MAX_N, 0, 512, MODE_REV_ALT_ALMOST, 7},
        {MAX_N, MAX_N, 0, 512, MODE_REV_ALT_ALMOST, 7},

        {MAX_N, MAX_N, 0, 0},
        {MAX_N, MAX_N, MAX_V, MAX_V},
        {MAX_N, MAX_N, (1ull << 59) - 1, (1ull << 59) - 1},
        {MAX_N, MAX_N, 0, 1},
        {MAX_N, MAX_N, MAX_V - 1, MAX_V},

        {MAX_N, MAX_N, (1ull << 3) - 1, (1ull << 3)},
        {MAX_N, MAX_N, (1ull << 7) - 1, (1ull << 7)},
        {MAX_N, MAX_N, (1ull << 15) - 1, (1ull << 15)},
        {MAX_N, MAX_N, (1ull << 26) - 1, (1ull << 26)},
        {MAX_N, MAX_N, (1ull << 37) - 1, (1ull << 37)},
        {MAX_N, MAX_N, (1ull << 48) - 1, (1ull << 48)},
        {MAX_N, MAX_N, (1ull << 59) - 1, (1ull << 59)},

        {MAX_N, MAX_N, (1ull << 3), (1ull << 3) + (1ull << 2)},
        {MAX_N, MAX_N, (1ull << 7), (1ull << 7) + (1ull << 3)},
        {MAX_N, MAX_N, (1ull << 15), (1ull << 15) + (1ull << 7)},
        {MAX_N, MAX_N, (1ull << 26), (1ull << 26) + (1ull << 13)},
        {MAX_N, MAX_N, (1ull << 37), (1ull << 37) + (1ull << 18)},
        {MAX_N, MAX_N, (1ull << 48), (1ull << 48) + (1ull << 24)},
        {MAX_N, MAX_N, (1ull << 59), (1ull << 59) + (1ull << 30)},

        {MAX_N, MAX_N, (1ull << 6), (1ull << 6) + (1ull << 3)},
        {MAX_N, MAX_N, (1ull << 14), (1ull << 14) + (1ull << 7)},
        {MAX_N, MAX_N, (1ull << 25), (1ull << 25) + (1ull << 13)},
        {MAX_N, MAX_N, (1ull << 36), (1ull << 36) + (1ull << 18)},
        {MAX_N, MAX_N, (1ull << 47), (1ull << 47) + (1ull << 24)},
        {MAX_N, MAX_N, (1ull << 58), (1ull << 58) + (1ull << 30)},
    }},

    {MAX_N, {
        {4, MAX_N, 0, MAX_V},
        {4, MAX_N, 0, MAX_V},
        {4, MAX_N, 0, MAX_V},
        {4, MAX_N, 0, MAX_V},
        {4, MAX_N, 0, MAX_V},
        {4, MAX_N, 0, MAX_V},
        {4, MAX_N, 0, MAX_V},
        {4, MAX_N, 0, MAX_V},
        {4, MAX_N, 0, MAX_V},
        {4, MAX_N, 0, MAX_V},
        {4, MAX_N, 0, MAX_V},
        {4, MAX_N, 0, MAX_V},
        {4, MAX_N, 0, MAX_V},
        {4, MAX_N, 0, MAX_V},
        {4, MAX_N, 0, MAX_V},
        {4, MAX_N, 0, MAX_V},
        {4, MAX_N, 0, MAX_V},
        {4, MAX_N, 0, MAX_V},
        {4, MAX_N, 0, MAX_V},
        {4, MAX_N, 0, MAX_V},

        {MAX_N, MAX_N, 0, MAX_V},
        {MAX_N, MAX_N, 0, MAX_V},
        {MAX_N, MAX_N, 0, MAX_V},
        {MAX_N, MAX_N, 0, MAX_V},
        {MAX_N, MAX_N, 0, MAX_V},
        {MAX_N, MAX_N, 0, MAX_V},
        {MAX_N, MAX_N, 0, MAX_V},
        {MAX_N, MAX_N, 0, MAX_V},
        {MAX_N, MAX_N, 0, MAX_V},
        {MAX_N, MAX_N, 0, MAX_V},
        {MAX_N, MAX_N, 0, MAX_V},

        {1, 1, 0, MAX_V},
        {2, 2, 0, MAX_V},
        {3, 3, 0, MAX_V},
        {4, 4, 0, MAX_V},
        {MAX_N - 2, MAX_N - 2, 0, MAX_V},
        {MAX_N - 1, MAX_N - 1, 0, MAX_V},

        {4, MAX_N, 0, MAX_V, MODE_SRT},
        {4, MAX_N, 0, MAX_V, MODE_REV},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT, 2},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT, 3},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT, 4},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT, 5},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT, 6},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT, 7},
        {4, MAX_N, 0, MAX_V, MODE_SRT_ALMOST},
        {4, MAX_N, 0, MAX_V, MODE_SRT_ALMOST},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALMOST},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALMOST},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALMOST},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALMOST},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},

        {4, MAX_N, 0, MAX_V, MODE_SRT},
        {4, MAX_N, 0, MAX_V, MODE_REV},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT, 2},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT, 3},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT, 4},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT, 5},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT, 6},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT, 7},
        {4, MAX_N, 0, MAX_V, MODE_SRT_ALMOST},
        {4, MAX_N, 0, MAX_V, MODE_SRT_ALMOST},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALMOST},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALMOST},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALMOST},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALMOST},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},
        {4, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},

        {MAX_N, MAX_N, 0, MAX_V, MODE_SRT},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT, 2},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT, 3},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT, 4},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT, 5},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT, 6},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT, 7},
        {MAX_N, MAX_N, 0, MAX_V, MODE_SRT_ALMOST},
        {MAX_N, MAX_N, 0, MAX_V, MODE_SRT_ALMOST},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALMOST},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALMOST},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALMOST},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALMOST},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 2},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 3},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 4},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 5},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 6},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},
        {MAX_N, MAX_N, 0, MAX_V, MODE_REV_ALT_ALMOST, 7},

        {4, MAX_N, 0, 512},
        {4, MAX_N, 0, 512},
        {4, MAX_N, 0, 512},
        {4, MAX_N, 0, 512},
        {4, MAX_N, 0, 512},
        {4, MAX_N, 0, 512},
        {4, MAX_N, 0, 512},
        {4, MAX_N, 0, 512},

        {MAX_N, MAX_N, 0, 512},
        {MAX_N, MAX_N, 0, 512},
        {MAX_N, MAX_N, 0, 512},

        {4, MAX_N, 0, 512, MODE_SRT},
        {4, MAX_N, 0, 512, MODE_REV},
        {4, MAX_N, 0, 512, MODE_REV_ALT, 2},
        {4, MAX_N, 0, 512, MODE_REV_ALT, 3},
        {4, MAX_N, 0, 512, MODE_REV_ALT, 4},
        {4, MAX_N, 0, 512, MODE_REV_ALT, 5},
        {4, MAX_N, 0, 512, MODE_REV_ALT, 6},
        {4, MAX_N, 0, 512, MODE_REV_ALT, 7},
        {4, MAX_N, 0, 512, MODE_SRT_ALMOST},
        {4, MAX_N, 0, 512, MODE_REV_ALMOST},
        {4, MAX_N, 0, 512, MODE_REV_ALMOST},
        {4, MAX_N, 0, 512, MODE_REV_ALT_ALMOST, 2},
        {4, MAX_N, 0, 512, MODE_REV_ALT_ALMOST, 2},
        {4, MAX_N, 0, 512, MODE_REV_ALT_ALMOST, 3},
        {4, MAX_N, 0, 512, MODE_REV_ALT_ALMOST, 3},
        {4, MAX_N, 0, 512, MODE_REV_ALT_ALMOST, 4},
        {4, MAX_N, 0, 512, MODE_REV_ALT_ALMOST, 4},
        {4, MAX_N, 0, 512, MODE_REV_ALT_ALMOST, 5},
        {4, MAX_N, 0, 512, MODE_REV_ALT_ALMOST, 5},
        {4, MAX_N, 0, 512, MODE_REV_ALT_ALMOST, 6},
        {4, MAX_N, 0, 512, MODE_REV_ALT_ALMOST, 6},
        {4, MAX_N, 0, 512, MODE_REV_ALT_ALMOST, 7},
        {4, MAX_N, 0, 512, MODE_REV_ALT_ALMOST, 7},

        {4, MAX_N, 0, 0},
        {4, MAX_N, MAX_V, MAX_V},
        {4, MAX_N, (1ull << 59) - 1, (1ull << 59) - 1},
        {4, MAX_N, 0, 1},
        {4, MAX_N, MAX_V - 1, MAX_V},

        {4, MAX_N, (1ull << 3) - 1, (1ull << 3)},
        {4, MAX_N, (1ull << 7) - 1, (1ull << 7)},
        {4, MAX_N, (1ull << 15) - 1, (1ull << 15)},
        {4, MAX_N, (1ull << 26) - 1, (1ull << 26)},
        {4, MAX_N, (1ull << 37) - 1, (1ull << 37)},
        {4, MAX_N, (1ull << 48) - 1, (1ull << 48)},
        {4, MAX_N, (1ull << 59) - 1, (1ull << 59)},

        {4, MAX_N, (1ull << 3), (1ull << 3) + (1ull << 2)},
        {4, MAX_N, (1ull << 7), (1ull << 7) + (1ull << 3)},
        {4, MAX_N, (1ull << 15), (1ull << 15) + (1ull << 7)},
        {4, MAX_N, (1ull << 26), (1ull << 26) + (1ull << 13)},
        {4, MAX_N, (1ull << 37), (1ull << 37) + (1ull << 18)},
        {4, MAX_N, (1ull << 48), (1ull << 48) + (1ull << 24)},
        {4, MAX_N, (1ull << 59), (1ull << 59) + (1ull << 30)},

        {4, MAX_N, (1ull << 6), (1ull << 6) + (1ull << 3)},
        {4, MAX_N, (1ull << 14), (1ull << 14) + (1ull << 7)},
        {4, MAX_N, (1ull << 25), (1ull << 25) + (1ull << 13)},
        {4, MAX_N, (1ull << 36), (1ull << 36) + (1ull << 18)},
        {4, MAX_N, (1ull << 47), (1ull << 47) + (1ull << 24)},
        {4, MAX_N, (1ull << 58), (1ull << 58) + (1ull << 30)},
    }}
};

const int NUM_TESTS = 30;

int main()
{
    std::ios::sync_with_stdio(false);

    int num = 1;
    for (const auto& [maxN, specs] : testSpecs)
    {
        std::cerr << "Subtask " << maxN << ": " << num << std::endl;

        auto shuffledSpecs = specs;
        std::shuffle(shuffledSpecs.begin(), shuffledSpecs.end(), generator);

        for (int t = 0; t < (int) shuffledSpecs.size(); t += NUM_TESTS)
        {
            std::string nums = "";
            nums += (num / 10 + '0');
            nums += (num % 10 + '0');

            std::cerr << "Test " << num << ":" << std::endl;

            std::ofstream outfile("ops." + nums + ".in");

            int numTests = std::min(NUM_TESTS, (int) shuffledSpecs.size() - t);

            outfile << maxN << std::endl << numTests << std::endl;

            for (int t2 = 0; t2 < numTests; t2++)
            {
                genTest(outfile, shuffledSpecs[t + t2]);
            }

            std::ofstream dummySol("ops." + nums + ".sol");
            num++;
        }
    }

    return 0;
}
