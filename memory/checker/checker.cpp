#include <iostream>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cmath>

using uint = unsigned int;
using ull = unsigned long long;
constexpr uint MAX_STATE_SIZE = 8;

int main(int argc, char *argv[])
{
    std::ifstream in(argv[1]);
    std::ifstream out(argv[3]);

    if (!in)
    {
        std::cout << 0 << std::endl;
        std::cerr << "In-file " << argv[1] << " not found." << std::endl;
        return 0;
    }

    if (!out)
    {
        std::cout << 0 << std::endl;
        std::cerr << "Out-file " << argv[3] << " not found." << std::endl;
        return 0;
    }

    uint k, seed;
    in >> k >> seed;

    uint n = 0;
    uint trueAns = 0;

    for (uint i = 0; i < k; ++i)
    {
        uint part;
        in >> part;
        n += part;
        trueAns += part * part;
    }

    uint numTests, size;
    uint ans;
    long long sum = 0;
    ull sumSq1 = 0;
    ull sumSq2 = 0;
    ull hsh;
    ull trueHsh = 0;

    out >> numTests >> size;
    trueHsh += numTests;
    trueHsh += size;

    if (size > 0 && size <= MAX_STATE_SIZE)
    {
        for (uint i = 0; i < numTests; ++i)
        {
            out >> ans;
            trueHsh += ans;
            sum += (long long) ans - trueAns;
            ull diffSq = ((ull) ans - trueAns) * ((ull) ans - trueAns);
            ull diffSq1 = diffSq & (((ull) 1u << 32u) - 1u);
            ull diffSq2 = diffSq >> 32u;
            sumSq1 += diffSq1;
            sumSq2 += diffSq2 + (sumSq1 >> 32u);
            sumSq1 &= (((ull) 1u << 32u) - 1u);
        }
    }

    out >> hsh;

    if (hsh != trueHsh)
    {
        std::cerr << "Your program somehow pruinted to stdout." << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    if (size == 0 || size > MAX_STATE_SIZE)
    {
        std::cerr << "Invalid state size." << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    long double meanError = (long double) sum / numTests;
    long double rootMeanSquaredError = sqrtl((long double) sumSq1 / numTests + (long double) sumSq2 * ((ull) 1u << 32u) / numTests);

    long double rootSize = sqrtl(std::max(size, 1u));
    long double err1 = std::abs(logl((1 + trueAns + meanError) / (1 + trueAns))) * rootSize;
    long double err2 = logl(1 + rootMeanSquaredError * rootSize) / logl(trueAns);

    long double fact1 = pow(3, 0.005 - err1);
    long double fact2 = (1.15 - err2) / 0.3;

    fact1 = std::min<long double>(std::max<long double>(fact1, 0), 1);
    fact2 = std::min<long double>(std::max<long double>(fact2, 0), 1);

    long double score = fact1 * (0.35 + 0.65 * fact2);
 
    score = std::min<long double>(std::max<long double>(score, 0), 1);

    std::cout << std::setprecision(6) << std::fixed << score << std::endl;

    return 0;
}
