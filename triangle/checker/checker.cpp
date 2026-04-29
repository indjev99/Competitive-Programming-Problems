#include <math.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

int main(int argc, char *argv[])
{
    std::ifstream in(argv[1]);
    std::ifstream out(argv[3]);

    if(!in.is_open()) {
        std::cerr << "In-file " << argv[1] << " not found." << std::endl;
        std::cout << 0.0 << std::endl;
        return 0;
    }

    if(!out.is_open()) {
        std::cerr << "Out-file " << argv[3] << " not found." << std::endl;
        std::cout << 0.0 << std::endl;
        return 0;
    }

    std::string s;
    out >> s;    

    if (s == "WA!")
    {
        std::cout << 0.0 << '\n';
        std::cerr << "Wrong Answer!\n";
        return 0;
    }

    long long seed;
    if (!(out >> seed) || seed != 4571891552734)
    {
        std::cout << 0.0 << '\n';
        std::cerr << "Security violation!\n";
        return 0;
    }
    
    double target = 8770;
    double contestant;
    out >> contestant;

    if (contestant > 2e6)
    {
        std::cout << 0.0 << '\n';
        std::cerr << "Wrong Answer!\n";
        return 0;
    }
    
    if (contestant <= target)
    {
        std::cout << 1.0 << '\n';
        std::cerr << "OK! " << contestant << "\n";
        return 0;
    }

    std::cout << std::max(0.15, 1.0 - std::sqrt(1.0 - pow(target / contestant, 0.55))) << '\n';
    std::cerr << "OK! " << contestant << "\n";
    return 0;
}
