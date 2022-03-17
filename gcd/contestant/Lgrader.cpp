#include "gcd.h"

#include <iostream>
#include <random>

static const size_t NUM_TRIALS = 4e4;

static ull MAX_X;
static ull MAX_AB;
static ull SEED;

static ull x;

static std::mt19937 generator;
static void generateNumber()
{
    std::uniform_int_distribution<ull> distribution(0, MAX_X - 1);
    x = distribution(generator);
}

static ull numQueries;

ull query(ull a, ull b)
{
    if (a < 0 || b <= 0 || a >= MAX_AB || b >= MAX_AB)
    {
        std::cout << "Invalid A or B." << std::endl;
        exit(0);
    }

    ++numQueries;

    a += x;
    while (b != 0)
    {
        ull temp = b;
        b = a % b;
        a = temp;
    }

    return a;
}

static void verifyGuess(ull guess)
{
    if (guess != x)
    {
        std::cout << "Wrong guess." << std::endl;
        exit(0);
    }
}

int main()
{
    std::cin >> MAX_X >> SEED;

    MAX_AB = MAX_X * 2;
    generator.seed(SEED);
    for (size_t i = 0; i < NUM_TRIALS; ++i)
    {
        generateNumber();
        ull guess = play(MAX_X);
        verifyGuess(guess);
    }

    double averageQueries = (long double) numQueries / NUM_TRIALS;
    std::cout << "Average queries: " << averageQueries << std::endl;

    return 0;
}