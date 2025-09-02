#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>

const std::string PASSWORD = "fdi84fj29slfh20fd5jkd";

int main(int argc, char *argv[])
{
    std::ifstream out(argv[3]);

    if (!out)
    {
        std::cerr << "Out-file " << argv[3] << " not found." << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    std::string password;
    out >> password;

    if (password != PASSWORD)
    {
        std::cerr << "IO issues." << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    int numColors;
    out >> numColors;

    if (numColors < 0)
    {
        std::string msg;
        std::getline(out, msg);
        if (msg == "") std::getline(out, msg);
        std::cerr << msg << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    int maxIters;
    out >> maxIters;

    double score = 0.0;
    if (numColors <= 4) score = 1.0;
    else if (numColors <= 8) score = 1.0 - 0.6 * (numColors - 4) / 4.0;
    else if (numColors <= 21) score = 0.4 * 8 / numColors;
    else score = 0.15;

    std::cerr << "Num colors: " << numColors << std::endl;
    std::cerr << "Num iters: " << maxIters << std::endl;

    std::cout << std::setprecision(6) << std::fixed << score << std::endl;

    return 0;
}
