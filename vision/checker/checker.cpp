#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>

const std::string PASSWORD = "sh339fj348d-2hc2hcw";

const double TARGET1 = 1.5;
const double TARGET2 = 1.125;

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

    int d;
    in >> d;

    std::string pass;
    out >> pass;

    if (pass != PASSWORD)
    {
        std::cerr << "Your program wrote to stdout." << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    double avg;
    out >> avg;

    if (avg < 0)
    {
        std::string msg;
        std::getline(out, msg);
        if (msg == "") std::getline(out, msg);
        std::cerr << msg << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    double target = d == 1 ? TARGET1 : TARGET2;

    double score = (target - 1) / (std::max(avg, target) - 1);
    score = 1 - std::pow(1 - score, 0.8);

    std::cerr << "Average vision: " << avg << std::endl;
    std::cout << score << std::endl;

    return 0;
}
