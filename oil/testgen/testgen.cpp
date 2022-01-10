#include <iostream>
#include <fstream>
#include <time.h>
#include <assert.h>
#include <algorithm>
#include <numeric>
#include <vector>
#include <unordered_set>
#include <random>

const int MAX_PRICE = 1e6;

std::mt19937 generator;
int randNum(int lb, int ub)
{
    std::uniform_int_distribution<int> distribution(lb, ub);
    return distribution(generator);
}
bool oneIn(int chances)
{
    std::uniform_int_distribution<int> distribution(0, chances - 1);
    return distribution(generator) == 0;
}

#define T_UNIFORM 0
#define T_WALK 1
#define T_LONG_WALK 2 

int n;
int type;

void genTest(std::ofstream& outfile)
{
    int storageCost = randNum(1, 100);

    outfile << n << " " << storageCost << "\n";

    int price = 0;
    int lastDir = 1;
    for (int i = 0; i <= n; ++i)
    {
        if (type == T_UNIFORM) price = randNum(-1000, 1000);
        else if (type == T_WALK) price += randNum(-10 * storageCost, 10 * storageCost);
        else if (type == T_LONG_WALK)
        {
            if (oneIn(10)) lastDir *= -1;
            price += randNum(0, 3) * storageCost * lastDir;
        }

        price = std::max(price, -MAX_PRICE);
        price = std::min(price, MAX_PRICE);

        if (i > 0) outfile << " ";
        outfile << price;
    }

    outfile << "\n";
}

int main()
{
    std::ios::sync_with_stdio(false);

    int num;
    while (true)
    {
        std::cout << "Test num? ";
        std::cin >> num;
        if (num < 0) return 0;
        std::cout << "n? ";
        std::cin >> n;
        for (int i = 0; i < 6; ++i)
        {
            std::string nums = "";
            nums += (num / 10 + '0');
            nums += (num % 10 + '0');
            std::ofstream outfile("oil." + nums + ".in");

            type = i % 3;

            genTest(outfile);
            outfile.close();

            ++num;
        }
    }
    return 0;
}
