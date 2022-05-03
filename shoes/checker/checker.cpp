#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>

int n, subtask, n_tests, seed;
long long hsh;
long double qs;

long double target1, target2;

const long long HASH_BASE = 853;
const long long HASH_MOD = 7246427;

long long hashWithInput()
{
    long long hsh = (n + HASH_MOD) % HASH_MOD;
    hsh = (hsh * HASH_BASE + subtask) % HASH_MOD;
    hsh = (hsh * HASH_BASE + n_tests) % HASH_MOD;
    hsh = (hsh * HASH_BASE + seed) % HASH_MOD;
    return hsh;
}

std::ifstream in, sol, out;
int main(int argc, char *argv[])
{
    n = 5000;

    in.open(argv[1]);
    out.open(argv[3]);

    if (!in)
    {
        std::cout << 0 << "\n";
        std::cerr << "In-file " << argv[1] << " not found.\n";
        return 0;
    }

    if (!out)
    {
        std::cout << 0 << "\n";
        std::cerr << "Out-file " << argv[3] << " not found.\n";
        return 0;
    }

    in >> n >> subtask >> n_tests >> seed >> target1 >> target2;
    out >> qs >> hsh;

    if (hsh != hashWithInput())
    {
        std::cout << 0 << "\n";
        std::cerr<<"Your program printed to stdout. It shouldn't.\n";
        return 0;
    }

    if (qs == -5){
        std::cout << 0 << "\n";
        std::cerr << "Your program didn't return the correct pairs.\n";
        return 0;
    }

    if (qs == -4)
    {
        std::cout << 0 << "\n";
        std::cerr << "Your program returned pairs that contain the same shoe.\n";
        return 0;
    }

    if (qs == -3)
    {
        std::cout << 0 << "\n";
        std::cerr << "Your program returned a pair that contains an out of bound number.\n";
        return 0;
    }

    if (qs == -2)
    {
        std::cout << 0 << "\n";
        std::cerr << "Your program returned a vector that contains too many or too few pairs.\n";
        return 0;
    }

    if (qs == -1)
    {
        std::cout << 0 << "\n";
        std::cerr << "Your program used pairInSet() on a set that contained duplicates/out of range numbers.\n";
        return 0;
    }

    long double score;

    if(qs <= target2){
        double part = (target2 - qs) / (target2 - target1);
        score = std::min(0.25l + 0.75l * powl(part, 1.5l), 1.0l);
    }
    else {
        score = std::max(0.25l * powl(target2 / qs, 0.75l), 0.05l);
    }

    std::cout << std::fixed << std::setprecision(6) << score << "\n";
    std::cerr << "Your program successfully returned the correct pairs and used " << std::fixed << std::setprecision(2) << qs << " queries on average.";

    return 0;
}
