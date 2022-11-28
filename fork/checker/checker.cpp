#include <iostream>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cmath>

// allowed guess error
const double EPS = 1e-8;

// ans error
const double EPS2 = 1e-12;

int main(int argc, char *argv[])
{
    std::ifstream sol(argv[2]);
    std::ifstream out(argv[3]);

    if (!sol)
    {
        std::cout << 0 << std::endl;
        std::cerr << "Sol-file " << argv[2] << " not found." << std::endl;
        return 0;
    }

    if (!out)
    {
        std::cout << 0 << std::endl;
        std::cerr << "Out-file " << argv[3] << " not found." << std::endl;
        return 0;
    }

    double ans;
    sol >> ans;

    double guess;
    out >> guess;

    double absErr = std::abs(ans - guess);
    double relErr = absErr == 0 && ans == 0 ? 0 : absErr / ans;

    if (relErr <= EPS + EPS2) std::cout << 1 << std::endl;
    else std::cout << 0 << std::endl;

    return 0;
}
