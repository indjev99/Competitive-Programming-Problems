#include <iostream>
#include <vector>
#include <cassert>
#include <random>

std::mt19937 generator;
std::uniform_int_distribution<int> distr;

int setup(int m)
{
    distr = std::uniform_int_distribution<int>(0, m - 1);
    return 2;
}

std::vector<int> encode(int a)
{
    if (a == 0)
    {
        int x = distr(generator);
        return {x, x, x};
    }
    else
    {
        int x, y, z;
        do
        {
            x = distr(generator);
            y = distr(generator);
            z = distr(generator);
        }
        while (x == y || x == z || y == z);
        return {x, y, z};
    }
}

int decode(int x, int y)
{
    if (x == y) return 0;
    else return 1;
}
