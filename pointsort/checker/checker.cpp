#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>

const int MAX_K = 5;
double TARGETS[MAX_K + 1] = {
    0,
    121000,
    187500,
    267000,
    350800,
    434500
};

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

    int t, n, k;

    in >> t >> n >> k;

    std::string sign;
    out >> sign;

    if (sign != "Signed")
    {
        std::cerr << "Your program wrote to stdout." << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    int comps;
    out >> comps;

    if (comps < 0)
    {
        std::string msg;
        std::getline(out, msg);
        if (msg == "") std::getline(out, msg);
        std::cerr << msg << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    double avgComps = comps / t;

    int target = TARGETS[k];

    double relExtra = avgComps / target - 1;

    double score = 1;
    score -= 3.75 * std::min(std::max(relExtra, 0.0), 0.15);
    score -= 1.00 * std::max(relExtra - 0.15, 0.0);
    score = std::max(score, 0.1);

    std::cerr << "Score: " << score << std::endl;
    std::cerr << "Relative extra: " << relExtra << std::endl;
    std::cerr << "Average number of comparisons: " << avgComps << std::endl;
    std::cout << score << std::endl;

    return 0;
}
