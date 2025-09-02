#include "prison.h"

#include <iostream>

static const std::string PASSWORD_GRADER_AUTH = "A&0k4s+h";
static const std::string PASSWORD_MANAGER_AUTH = "_d&f92Kx";

static constexpr int ENCODE = 0;
static constexpr int DECODE = 1;

static void checkManagerAuthAndAlive()
{
    std::string pass;
    std::cin >> pass;

    if (pass != PASSWORD_MANAGER_AUTH)
    {
        exit(0);
    }
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    checkManagerAuthAndAlive();

    int m;
    int cmd;
    std::cin >> m;
    std::cin >> cmd;

    int n = setup(m);

    std::cout << PASSWORD_GRADER_AUTH << "\n";
    std::cout << n << std::endl;

    while (true)
    {
        checkManagerAuthAndAlive();

        if (cmd == ENCODE)
        {
            int a;
            std::cin >> a;

            std::vector<int> trip = encode(a);

            std::cout << PASSWORD_GRADER_AUTH << "\n";
            std::cout << trip.size();
            for (int elem : trip)
            {
                std::cout << " " << elem;
            }
            std::cout << std::endl;
        }
        else if (cmd == DECODE)
        {
            int x, y;
            std::cin >> x >> y;

            int b = decode(x, y);

            std::cout << PASSWORD_GRADER_AUTH << "\n";
            std::cout << b << std::endl;
        }
    }

    return 0;
};
