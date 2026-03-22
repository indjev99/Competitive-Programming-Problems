#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>

static const std::string PASSWORD = "ch3e9v47slxyu54io2jud";

int main(int argc, char *argv[])
{
    std::ifstream in(argv[1]);
    std::ifstream sol(argv[2]);
    std::ifstream out(argv[3]);

    if (!in)
    {
        std::cerr << "In-file " << argv[1] << " not found." << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    if (!sol)
    {
        std::cerr << "Sol-file " << argv[1] << " not found." << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    if (!out)
    {
        std::cerr << "Out-file " << argv[1] << " not found." << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    std::string pass;
    out >> pass;

    if (pass != PASSWORD)
    {
        std::cerr << "Your program wrote to stdout." << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    double cost;
    out >> cost;

    if (cost < 0)
    {
        std::string msg;
        std::getline(out, msg);
        if (msg == "") std::getline(out, msg);
        std::cerr << msg << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    double target;
    sol >> target;
    target += 0.005;

    double rat = std::min(target / cost, 1.0);

    double result = 0.5 * (std::pow(rat, 10.0)) + 0.5 * (1.0 - std::pow(1.0 - rat, 0.15));

    std::cerr << "Q: " << std::fixed << std::setprecision(4) << rat << std::endl;
    std::cout << std::fixed << std::setprecision(4) << result << std::endl;

    return 0;
}
