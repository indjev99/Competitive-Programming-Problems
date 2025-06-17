#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cmath>

const std::string PASSWORD = "PASSWORD_93765017535";

const double TARGET = 2150000;
const double SCALE = 0.5;
const double POW = 2.25;

int main(int argc, char *argv[])
{
    std::ifstream in(argv[1]);
    std::ifstream out(argv[3]);

    if (!in)
    {
        std::cerr << "In-file " << argv[1] << " not found." << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    if (!out)
    {
        std::cerr << "Out-file " << argv[1] << " not found." << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    std::string password;
    out >> password;

    if (password != PASSWORD)
    {
        std::cerr << "Your program wrote to stdout." << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    int ops;
    out >> ops;

    if (ops < 0)
    {
        std::string msg;
        std::getline(out, msg);
        if (msg == "") std::getline(out, msg);
        std::cerr << msg << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    double score = 0;
    if (ops <= TARGET) score = 1;
    else if (ops >= 3 * TARGET) score = 0;
    else score = pow(1 - SCALE * (ops / TARGET - 1), POW);

    std::cerr << "Number of operations: " << ops << std::endl;
    std::cout << std::fixed << std::setprecision(4) << score << std::endl;

    return 0;
}
