#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>

typedef unsigned long long ull;

const double WRONG_GUESS = -1;
const double INVALID_AB = -2;

ull MAX_X;
ull SEED;

const ull BASE = 792753497ULL;
const ull MOD = 1185914819ULL;
ull hashWithInput(ull hsh)
{
    hsh = (hsh * BASE + MAX_X) % MOD;
    hsh = (hsh * BASE + SEED) % MOD;
    return hsh;
}

int main(int argc, char *argv[])
{
    std::ifstream in(argv[1]);
    std::ifstream sol(argv[2]);
    std::ifstream out(argv[3]);

    if (!in.is_open())
    {
        std::cerr << "In-file " << argv[1] << " not found." << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    if (!sol.is_open())
    {
        std::cerr << "Sol-file " << argv[2] << " not found." << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    if (!out.is_open())
    {
        std::cerr << "Out-file " << argv[3] << " not found." << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    double target1, target2, res;
    ull hsh;

    in >> MAX_X >> SEED;
    sol >> target1 >> target2;
    out >> res >> hsh;

    if (hsh != hashWithInput(std::round(res * 100)))
    {
        std::cerr << "Your program printed to stdout. It shouldn't." << std::endl;
        std::cout << 0 << "\n";
        return 0;
    }

    if (res == WRONG_GUESS)
    {
        std::cerr << "Wrong guess." << std::endl;
        std::cout << 0 << "\n";
        return 0;
    }

    if (res == INVALID_AB)
    {
        std::cerr << "Invalid A or B." << std::endl;
        std::cout << 0 << "\n";
        return 0;
    }

    double score = res < target2 ? std::min(target1 / res, 1.0) : std::max(0.35 * target2 / res, 0.1);

    std::cerr << "Average queries: " << res << std::endl;
    std::cout << std::fixed << std::setprecision(4) << score << "\n";

    return 0;
}
