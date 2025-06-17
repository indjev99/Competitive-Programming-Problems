#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cmath>

const std::string PASSWORD_TRALALEROTRALALA69 = "PASSWORD_26354";

int main(int argc, char *argv[])
{
    std::ifstream in(argv[1]);
    std::ifstream sol(argv[2]);
    std::ifstream out(argv[3]);

    std::string password;
    out >> password;

    if (password != PASSWORD_TRALALEROTRALALA69)
    {
        std::cerr << "Your program wrote to stdout." << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    bool ans_contestant, ans_real;

    sol >> ans_real;
    out >> ans_contestant;
    
    if (ans_contestant == -1)
    {
        std::string msg;
        std::getline(out, msg);
        if (msg == "") std::getline(out, msg);
        std::cerr << msg << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    if(ans_real != ans_contestant)
    {
        std::cerr << "Wrong answer." << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    if(ans_real == 0)
    {
        std::cerr << "Correct answer." << std::endl;
        std::cout << 1 << std::endl;
        return 0;
    }

    int assignment_correctness;

    out >> assignment_correctness;
    if(assignment_correctness == -1)
    {
        std::string msg;
        std::getline(out, msg);
        if (msg == "") std::getline(out, msg);
        std::cerr << msg << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }
    else
    {
        std::cerr << "Correct answer." << std::endl;
        std::cout << 1 << std::endl;
        return 0;
    }
    return 0;
}
