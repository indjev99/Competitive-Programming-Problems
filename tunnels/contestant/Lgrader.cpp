#include "tunnels.h"

#include <iostream>

static int n, m, k;
static std::vector<std::vector<bool>> blocked;

static void input()
{
    std::cin >> n >> m >> k;

    blocked.resize(n);
    for (int i = 0; i < n; i++)
    {
        blocked[i].resize(m);
        for (int j = 0; j < m; j++)
        {
            char c;
            std::cin >> c;
            blocked[i][j] = c == '1';
        }
    }
}

static int numQueries = 0;

static int currX;
static int currY;

static void goTo(int y)
{
    std::cout << "On level " << currX << " going from " << currY << " to " << y << std::endl;

    while (currY != y)
    {
        if (currY < y) currY++;
        else currY--;

        if (blocked[currX][currY])
        {
            std::cout << "Passing through blocked chamber." << std::endl;
            exit(0);
        }
    }
}

bool investigate(int y)
{
    numQueries++;
    std::cout << "Investigating: " << y << std::endl;
    goTo(y);
    std::cout << "Enter 0/1:" << std::endl;
    bool ans;
    std::cin >> ans;
    return ans;
}

void goDeeper(int y)
{
    std::cout << "Going deeper: " << y << std::endl;

    if (currX == n)
    {
        std::cout << "Already in the treasure room." << std::endl;
        exit(0);
    }

    goTo(y);
    currX++;

    if (currX == n) return;

    if (blocked[currX][currY])
    {
        std::cout << "Went deeper into a blocked chamber." << std::endl;
        exit(0);
    }
}

int main()
{
    input();

    currX = 0;
    currY = k;

    if (blocked[currX][currY])
    {
        std::cout << "Starting chamber is blocked." << std::endl;
        exit(0);
    }

    solve(n, m, k, blocked);

    if (currX < n)
    {
        std::cout << "Did not reach the treasure room." << std::endl;
        exit(0);
    }

    std::cout << "Reached the treasure room." << std::endl;

    std::cout << "Number of investigations: " << numQueries << std::endl;

    return 0;
}
