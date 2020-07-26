#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>

const double POWER = 0.75;

int k, seed;
long long targetQs;
long long qs, hsh;

const long long HASH_BASE = 947;
const long long HASH_MOD = 1190518999;

long long hashWithInput(long long val)
{
    long long hsh = (val + HASH_MOD) % HASH_MOD;
    hsh = (hsh * HASH_BASE + k) % HASH_MOD;
    hsh = (hsh * HASH_BASE + seed) % HASH_MOD;
    return hsh;
}

std::ifstream in, sol, out;
int main(int argc, char *argv[])
{
    in.open(argv[1]);
    sol.open(argv[2]);
    out.open(argv[3]);

    if (!in)
    {
        std::cout << 0 << "\n";
        std::cerr << "In-file " << argv[1] << " not found.\n";
        return 0;
    }

    if (!sol)
    {
        std::cout << 0 << "\n";
        std::cerr << "Sol-file " << argv[2] << " not found.\n";
        return 0;
    }

    if (!out)
    {
        std::cout << 0 << "\n";
        std::cerr << "Out-file " << argv[3] << " not found.\n";
        return 0;
    }

    in >> k >> seed;
    sol >> targetQs;
    out >> qs >> hsh;

    if (hsh != hashWithInput(qs))
    {
        std::cout << 0 << "\n";
        std::cerr<<"Your program printed to stdout. It shouldn't.\n";
        return 0;
    }

    if (qs == -4)
    {
        std::cout << 0 << "\n";
        std::cerr << "Your program used invalid comparisons.\n";
        return 0;
    }

    if (qs == -3)
    {
        std::cout << 0 << "\n";
        std::cerr << "Your program returned a vector that contains too many or too few numbers.\n";
        return 0;
    }

    if (qs == -2)
    {
        std::cout << 0 << "\n";
        std::cerr << "Your program returned a vector that contains out of range numbers.\n";
        return 0;
    }

    if (qs == -1)
    {
        std::cout << 0 << "\n";
        std::cerr << "Your program returned a vector that is not sorted and/or contains duplicates.\n";
        return 0;
    }

    double score = (targetQs + 1.0) / (qs + 1.0);
    score = pow(score, POWER);
    score = std::min(score, 1.0);
    std::cout << std::setprecision(6) << score << "\n";
    std::cerr << "Your program successfully returned a correctly sorted vector.\n";

    return 0;
}
