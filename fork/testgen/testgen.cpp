#include <iostream>
#include <fstream>
#include <algorithm>
#include <tuple>
#include <vector>
#include <queue>
#include <set>
#include <random>

std::mt19937 generator;
long long randNum(long long lb, long long ub)
{
    std::uniform_int_distribution<long long> distribution(lb, ub);
    return distribution(generator);
}

bool oneIn(long long chances)
{
    if (chances == 0) return false;
    return randNum(0, chances) == 0;
}

std::vector<std::tuple<int, int, int>> nlrs = {
    // subtask 1
    {1, 0, 1},
    {2, 2, 2},
    {3, 2, 0},
    {4, 1, 2},
    {5, 0, 0},
    // subtask 2
    {10, 3, 2},
    {11, 4, 4},
    {12, 3, 9},
    {13, 4, 1},
    {14, 0, 1},
    // subtask 3
    {23, 5, 6},
    {25, 8, 16},
    {31, 30, 23},
    {34, 2, 7},
    {35, 1, 1},
    // subtask 4
    {63, 12, 12},
    {75, 6, 3},
    {84, 15, 39},
    {92, 23, 23},
    {100, 2, 1},
    // subtask 5
    {185, 15, 180},
    {236, 153, 110},
    {363, 50, 51},
    {421, 51, 50},
    {450, 0, 3},
    // subtask 6
    {1026, 392, 650},
    {1101, 1052, 165},
    {1242, 0, 105},
    {1429, 983, 807},
    {1500, 1, 4},
    // subtask 7
    {120050, 10005, 10005},
    {386121, 72632, 12345},
    {694214, 544111, 95423},
    {902175, 291257, 145116},
    {1000000, 2, 5},
    // subtask 8
    {6809872, 6809870, 12054},
    {10612045, 1111111, 1222222},
    {15532530, 2762644, 12345678},
    {17332467, 3428952, 2156009},
    {20000000, 1, 6}
};

int main()
{
    generator.seed(1);

    int num = 1;
    for (auto [n, l, r] : nlrs)
    {
        double prob = randNum(100, 900) * 1.0 / 1000;

        std::string nums = "";
        nums += (num / 10 + '0');
        nums += (num % 10 + '0');
        std::string name = "fork." + nums + ".in";
        std::ofstream outfile(name);

        outfile << prob << " " << n << " " << l << " " << r << std::endl;
        std::cerr << num << ": " << prob << " " << n << " " << l << " " << r << std::endl;

        ++num;
    }

    return 0;
}