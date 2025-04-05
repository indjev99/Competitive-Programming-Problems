#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>

const int TARGET1 = 240000;
const int TARGET2 = 256000;
const int TARGET3 = 272000;
const int TARGET4 = 304000;
const int TARGET5 = 400000;

int main(int argc, char *argv[])
{
    std::ifstream out(argv[3]);

    if (!out)
    {
        std::cerr << "Out-file " << argv[1] << " not found." << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    int bits;
    out >> bits;

    if (bits < 0)
    {
        std::string msg;
        std::getline(out, msg);
        if (msg == "") std::getline(out, msg);
        std::cerr << msg << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    double score = 0;
    if (bits <= TARGET1) score = 1.00;
    else if (bits <= TARGET2) score = 1.00 - 0.24 * (bits - TARGET1) / (TARGET2 - TARGET1);
    else if (bits <= TARGET3) score = 0.76 - 0.16 * (bits - TARGET2) / (TARGET3 - TARGET2);
    else if (bits <= TARGET4) score = 0.60 - 0.16 * (bits - TARGET3) / (TARGET4 - TARGET3);
    else if (bits <= TARGET5) score = 0.44 - 0.30 * (bits - TARGET4) / (TARGET5 - TARGET4);
    else score = 0.14 * TARGET5 / bits;

    std::cerr << "Max bits: " << bits << std::endl;
    std::cout << score << std::endl;

    return 0;
}
