#include <iostream>
#include <fstream>
#include <time.h>
#include <assert.h>
#include <algorithm>
#include <numeric>
#include <vector>
#include <unordered_set>
#include <random>

const long long MAX_N_1 = 5e3;
const long long MAX_N_2 = 2e2;
const long long MAX_N_6 = 9e3;

const long long MAX_M_1 = 1e4;

const long long MAX_N = 1e6;
const long long MAX_M = 1e9;
const long long MOD = 1e9 + 7;

std::mt19937 generator;
long long randNum(long long lb, long long ub)
{
    std::uniform_int_distribution<long long> distribution(lb, ub - 1);
    return distribution(generator);
}

int subtask;

void genTest(std::ofstream& outfile)
{
    long long maxN = MAX_N;
    long long maxM = MAX_M;

    if (subtask == 1)
    {
        maxN = MAX_N_1;
        maxM = MAX_M_1;
    }
    else if (subtask == 2)
    {
        maxN = MAX_N_2;
    }
    else if (subtask == 6)
    {
        maxN = MAX_N_6;
    }

    long long n, m, t;

    do
    {
        n = randNum(maxN * 4 / 5, maxN);
    }
    while (n >= maxM);

    assert(1 <= n);
    assert(n < maxN);

    m = randNum(n + (maxM - n) * 4 / 5, maxM);

    assert(n <= m);
    assert(m < maxM);

    t = randNum(0, n);

    outfile << n << " " << m << "\n";

    for (int i = 0; i < n; ++i)
    {
        if (i > 0) outfile << " ";

        bool zero = false;
        if (subtask == 3 && i > 0) zero = true;
        if (subtask == 4 && i > 1) zero = true;
        if (subtask == 5 && i != t) zero = true;

        if (zero) outfile << 0;
        else outfile << randNum(0, MOD);
    }

    outfile << "\n";
}

const int NUM_SUBTASKS = 7;
const int PER_SUBTASK = 5;

int main()
{
    std::ios::sync_with_stdio(false);

    generator.seed(0);

    for (int i = 0; i < NUM_SUBTASKS * PER_SUBTASK; i++)
    {
        subtask = i / PER_SUBTASK + 1;

        int num = i + 1;

        std::string nums = "";
        nums += (num / 10 + '0');
        nums += (num % 10 + '0');

        std::ofstream outfile("lin." + nums + ".in");

        genTest(outfile);
        outfile.close();
    }

    return 0;
}
