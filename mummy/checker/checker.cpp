#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>

static const std::string PASSWORD = "du38dn50cbq01nc594";

double TARGET = 3700;

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

    std::string pass;
    out >> pass;

    if (pass != PASSWORD)
    {
        std::cerr << "Your program wrote to stdout." << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    double queries;
    out >> queries;

    if (queries < 0)
    {
        std::string msg;
        std::getline(out, msg);
        if (msg == "") std::getline(out, msg);
        std::cerr << msg << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    double score = TARGET / std::max(queries, TARGET);

    std::cerr << "Queries: " << queries << std::endl;
    std::cout << score << std::endl;

    return 0;
}
