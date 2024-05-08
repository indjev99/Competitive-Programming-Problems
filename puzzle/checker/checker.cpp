#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>

static const std::string password = "SD9G%^^H&O)UIKGH,QSAEE@R.QF42+]RW%$EGDTU&R-=1JG8HDS";
static constexpr int ALL = 0;
static constexpr int STOP = 1;
static constexpr int TIMEOUT = 2;
static constexpr int INVALID = 3;

int main(int argc, char *argv[])
{
    std::ifstream in(argv[1]);
    std::ifstream sol(argv[2]);
    std::ifstream out(argv[3]);

    if (!in.is_open())
    {
        std::cerr << "In-file " << argv[1] << " not found." << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    if (!sol.is_open())
    {
        std::cerr << "Sol-file " << argv[2] << " not found." << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    if (!out.is_open())
    {
        std::cerr << "Out-file " << argv[3] << " not found." << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    int numSubtests;
    double timeLimit;
    std::string outPass;

    in >> numSubtests >> timeLimit;

    out >> outPass;
    if (outPass != password)
    {
        std::cerr << "Your program printed to stdout. It shouldn't." << std::endl;
        std::cout << 0 << "\n";
        return 0;
    }

    int verdict;
    int lastSubtest;
    double timePassed;

    out >> verdict >> lastSubtest >> timePassed;

    if (verdict == STOP)
    {
        std::cerr << "Stopped on subtest: " << lastSubtest + 1 << " / " << numSubtests << std::endl;
    }
    if (verdict == TIMEOUT)
    {
        std::cerr << "Timed out during subtest: " << lastSubtest + 1 << " / " << numSubtests << std::endl;
    }
    if (verdict == INVALID)
    {
        std::cerr << "Invalid solution on subtest: " << lastSubtest + 1 << " / " << numSubtests << std::endl;
    }

    std::cerr << "Subtests solved: " << lastSubtest << " / " << numSubtests << std::endl;
    std::cerr << "Time taken: " << timePassed << " / " << timeLimit << std::endl;

    std::cout << std::fixed << std::setprecision(4) << (double) lastSubtest / numSubtests << std::endl;

    return 0;
}
