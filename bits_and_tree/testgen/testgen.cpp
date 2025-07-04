#include <iostream>
#include <fstream>
#include <ctime>
#include <cassert>
#include <algorithm>
#include <numeric>
#include <vector>
#include <unordered_set>
#include <random>

std::mt19937 generator(2435289);

int rand_num(int lb, int ub)
{
    std::uniform_int_distribution<int> distribution(lb, ub - 1);
    return distribution(generator);
}

double rand_real(double lb, double ub)
{
    std::uniform_real_distribution<double> distribution(lb, ub);
    return distribution(generator);
}

const int T = 2;
const int N = 200;

void gen_test(std::ofstream &outfile, double prob) {
    for (int test = 1; test <= T; ++test) {
        outfile << N << std::endl;
        for (int i = 0; i < 2 * N; ++i) {
            outfile << (rand_real(0, 1) <= prob);
        }
        outfile << std::endl;
    }
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);

    int num = 1;

    const int NUM_TESTS = 10;
    for (int i = 0; i < NUM_TESTS; ++i) {
        std::cout << std::endl;

        std::string nums = "";
        nums += (num / 10 + '0');
        nums += (num % 10 + '0');

        std::ofstream dummySol("bits_and_tree." + nums + ".sol");
        std::ofstream outfile("bits_and_tree." + nums + ".in");

        std::cout << "Test " << num << ":" << std::endl;

        outfile << T << " " << 193455 * i + 2334 << std::endl;

        double prob = 0.5;
        if (i == 0) {
            prob = 0;
        }
        else if (i == 1) {
            prob = 1;
        }
        else if (i == 2) {
            prob = 0.2;
        }
        else if (i == 3) {
            prob = 0.8;
        }

        gen_test(outfile, prob);    

        num++;
    }

    return 0;
}
