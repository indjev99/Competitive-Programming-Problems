#include <iostream>
#include "periods.h"

static int n;
static std::vector<int> array;
static std::vector<int> retPeriods;

static void input()
{
    std::cin >> n;
    array.resize(n);
    for (int& elem : array)
    {
        std::cin >> elem;
    }
}

static void output()
{
    for (int& period : retPeriods)
    {
        std::cout << period << " ";
    }
}

static int numQs;
bool hasPeriod(int len, int period)
{
    ++numQs;

    if (len <= 0 || len > n || period <= 0)
    {
        std::cerr << "Invalid query" << std::endl;
        exit(0);
    }

    if (len % period != 0) return false;

    for (int i = period; i < len; ++i)
    {
        if (array[i] != array[i - period]) return false;
    }

    return true;
}

int main()
{
    std::ios::sync_with_stdio(false);

    input();

    numQs = 0;
    retPeriods = findPeriods(n);

    std::cerr << "Number of queries: " << numQs << std::endl;
    output();

    return 0;
}