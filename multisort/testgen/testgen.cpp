#include <iostream>
#include <fstream>
#include <random>
using namespace std;

mt19937 generator(1337);

int randNum(int lb, int ub)
{
    uniform_int_distribution<int> distribution(lb, ub);
    return distribution(generator);
}

std::vector<int> ks = {
    2, 3, 4,
    6, 7, 9,
    12, 15, 19,
    23, 30, 39, 47, 52, 65, 71, 84, 96,
    120, 181, 263, 359, 440, 500, 666, 777, 825, 942, 999, 1000
};

int main()
{
    ofstream infile;
    for (int i = 0; i < ks.size(); ++i)
    {
        string nums = "";
        nums += ((i + 1) / 10 + '0');
        nums += ((i + 1) % 10 + '0');
        infile.open("multisort." + nums + ".in");
        infile << ks[i] << " " << randNum(0, 1e6) << "\n";
        infile.close();

    }
    return 0;
}
