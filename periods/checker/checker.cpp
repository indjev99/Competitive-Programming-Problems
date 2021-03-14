#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <math.h>

int n;
std::vector<int> array;

const long long BASE = 792753497ll;
const long long MOD = 1185914819ll;
long long hashWithInput(long long hsh)
{
    for (long long elem : array)
    {
        hsh = (hsh * BASE + elem) % MOD;
    }
    return hsh;
}

int main(int argc, char *argv[])
{
    std::ifstream in(argv[1]);
    std::ifstream out(argv[3]);

    if (!in.is_open())
    {
        std::cerr << "In-file " << argv[1] << " not found." << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    if (!out.is_open())
    {
        std::cerr << "Out-file " << argv[3] << " not found." << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    in >> n;
    array.resize(n);
    for (int& elem : array)
    {
        in >> elem;
    }

    int qs;
    long long hsh;
    out >> qs >> hsh;

    if (hsh != hashWithInput(qs))
    {
        std::cerr<<"Your program printed to stdout. It shouldn't.\n";
        std::cout << 0 << "\n";
        return 0;
    }

    if (qs == -2)
    {
        std::cerr << "Invalid query.\n";
        std::cout << 0 << "\n";
        return 0;
    }

    if (qs == -1)
    {
        std::cerr << "Wrong answer.\n";
        std::cout << 0 << "\n";
        return 0;
    }

    int targetQs = n / 2 + 1;
    double score;
    if (qs <= targetQs) score = 1.0;
    else score = 0.5 * pow(1.0 * targetQs / qs, 0.65);

    std::cerr << "Your program correctly found the minimal periods.\n";
    std::cout << std::setprecision(4) << score << "\n";

    return 0;
}
