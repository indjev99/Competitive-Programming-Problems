#include "navigation.h"

#include <iostream>

static const std::string PASSWORD_GRADER_AUTH = "9ds82uiwe9tusd342a";
static const std::string PASSWORD_MANAGER_AUTH = "kjdsgdfihe09tsdfuei";

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

    while (true)
    {
        checkManagerAuthAndAlive();

        int currColor;
        int numAdj;

        std::cin >> currColor >> numAdj;

        std::vector<int> adjColors(numAdj);

        for (int& col : adjColors)
        {
            std::cin >> col;
        }

        std::pair<int, int> action = navigate(currColor, adjColors);

        std::cout << PASSWORD_GRADER_AUTH << std::endl;
        std::cout << action.first << " " << action.second << std::endl;
    }

    return 0;
};
