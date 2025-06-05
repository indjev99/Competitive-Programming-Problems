#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>

const std::string PASSWORD = "4a89cneic0qkd0gd";
const std::string CORRECT = "CORRECT";
const std::string WRONG = "WRONG";

int main(int argc, char *argv[])
{
    std::ifstream out(argv[3]);

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
        std::cout << 0 << std::endl;
        return 0;
    }

    std::string verdict;
    out >> verdict;

    if (verdict != WRONG && verdict != CORRECT)
    {
        std::cout << 0 << std::endl;
        return 0;
    }

    if (verdict != CORRECT)
    {
        std::cout << 0 << std::endl;
        return 0;
    }

    std::cout << 1 << std::endl;

    return 0;
}
