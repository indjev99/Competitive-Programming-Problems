#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <random>
#include <cmath>

using uint = unsigned int;
using ull = unsigned long long;
constexpr uint NUM_TESTS = 10000;
constexpr uint MAX_STATE_SIZE = 8;
using State = std::array<uint, MAX_STATE_SIZE>;

constexpr uint stateSize(uint n);
constexpr void processOne(uint n, uint i, int curr, State& state);
constexpr uint getAnswer(uint n, const State& state);

#include "memory.h"

#undef NUM_TESTS
#undef MAX_STATE_SIZE
#undef constexpr
#undef for
#undef if
#undef return
#undef unsigned
#undef using
#undef include
#undef State
#undef int
#undef long
#undef double
#undef uint
#undef ull
#undef std
#undef cin
#undef cout
#undef cerr
#undef main
#undef sqrtl
#undef clear
#undef fill
#undef begin
#undef end
#undef fill_n
#undef back_inserter
#undef shuffle
#undef stateSize
#undef processOne
#undef getAnswer
#undef testConstexprArr
#undef testConstexprInit
#undef testConstexprSize
#undef testConstexprMain
#undef testConstexprAns
#undef n
#undef k
#undef seed
#undef parts
#undef generator
#undef trueAns
#undef sum
#undef sumSq1
#undef sumSq2
#undef nums
#undef state
#undef size
#undef num
#undef ans
#undef diffSq
#undef diffSq1
#undef diffSq2
#undef mean
#undef meanSq
#undef i
#undef t

constexpr uint testConstexprArr[100] = {
    456, 12, 0, 23, 43, 0, 0, 999, 505, 23,
    12, 0, 23, 77, 23, 0, 1, 603, 222, 18,
    12, 1, 17, 77, 505, 400, 0, 0, 0, 23,
    205, 12, 0, 43, 12, 1, 0, 999, 505, 0,
    0, 0, 23, 12, 854, 1, 0, 1, 12, 0,
    12, 12, 0, 732, 43, 23, 0, 34, 505, 555,
    12, 0, 23, 77, 23, 0, 1, 603, 222, 18,
    12, 1, 17, 77, 505, 400, 0, 0, 0, 23,
    205, 12, 640, 222, 12, 100, 220, 16, 180, 9,
    0, 150, 23, 12, 854, 1, 100, 1, 12, 20
};

constexpr State testConstexprInit = {0, 0, 0, 0, 0, 0, 0, 0};
constexpr uint testConstexprSize = stateSize(1000);

constexpr uint testConstexprMain(uint n, State state)
{
    for (uint i = 0; i < n; ++i)
    {
        processOne(n, i, testConstexprArr[i % 100], state);
        for (uint j = testConstexprSize; j < MAX_STATE_SIZE; ++j)
        {
            state[j] = 0;
        }
    }
    return getAnswer(n, state);
}

constexpr uint testConstexprAns = testConstexprMain(1000, testConstexprInit);

int main()
{
    uint k, seed;
    std::cin >> k >> seed;

    std::mt19937 generator(seed);

    uint n = 0;
    uint trueAns = 0;
    std::vector<uint> parts(k);

    for (uint i = 0; i < k; ++i)
    {
        std::cin >> parts[i];
        n += parts[i];
        trueAns += parts[i] * parts[i];
    }

    parts.resize(n, 0);

    uint size = stateSize(n);

    if (size == 0 || size > MAX_STATE_SIZE)
    {
        std::cout << "Invalid state size." << std::endl;
        return 0;
    }

    long long sum = 0;
    ull sumSq1 = 0;
    ull sumSq2 = 0;

    std::vector<uint> nums;
    State state;

    for (uint t = 0; t < NUM_TESTS; ++t)
    {
        nums.clear();
        std::shuffle(parts.begin(), parts.end(), generator);
        for (uint i = 0; i < n; ++i)
        {
            std::fill_n(std::back_inserter(nums), parts[i], i);
        }
        std::shuffle(nums.begin(), nums.end(), generator);

        std::fill(state.begin(), state.end(), 0);

        for (uint i = 0; i < n; ++i)
        {
            processOne(n, i, nums[i], state);
            std::fill(state.begin() + size, state.end(), 0);
        }

        uint ans = getAnswer(n, state);

        sum += (long long) ans - trueAns;
        ull diffSq = ((ull) ans - trueAns) * ((ull) ans - trueAns);
        ull diffSq1 = diffSq & (((ull) 1u << 32u) - 1u);
        ull diffSq2 = diffSq >> 32u;
        sumSq1 += diffSq1;
        sumSq2 += diffSq2 + (sumSq1 >> 32u);
        sumSq1 &= (((ull) 1u << 32u) - 1u);
    }

    long double meanError = (long double) sum / NUM_TESTS;
    long double rootMeanSquaredError = sqrtl((long double) sumSq1 / NUM_TESTS + (long double) sumSq2 * ((ull) 1u << 32u) / NUM_TESTS);

    long double rootSize = sqrtl(std::max(size, 1u));
    long double err1 = std::abs(logl((1 + trueAns + meanError) / (1 + trueAns))) * rootSize;
    long double err2 = logl(1 + rootMeanSquaredError * rootSize) / logl(trueAns);

    long double fact1 = pow(3, 0.005 - err1);
    long double fact2 = (1.15 - err2) / 0.3;

    fact1 = std::min<long double>(std::max<long double>(fact1, 0), 1);
    fact2 = std::min<long double>(std::max<long double>(fact2, 0), 1);

    long double score = fact1 * (0.35 + 0.65 * fact2);

    std::cout << "N: " << n << std::endl;
    std::cout << "H: " << trueAns << std::endl;
    std::cout << "S: " << size << std::endl;
    std::cout << "E: " << meanError << std::endl;
    std::cout << "R: " << rootMeanSquaredError << std::endl;
    std::cout << "U: " << err1 << std::endl;
    std::cout << "V: " << err2 << std::endl;
    std::cout << "Result: " << score << std::endl;

    return 0;
}
