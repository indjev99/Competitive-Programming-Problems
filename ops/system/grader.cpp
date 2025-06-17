#include "ops.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <random>

static const unsigned long long BASE_SEED = 8206571;

static const std::string PASSWORD = "PASSWORD_93765017535";

static const int TOO_MANY_OPS = 3 * 2150000;

struct __Test
{
    int n;

    std::vector<unsigned long long> numbers;
    std::vector<unsigned long long> inputs;
    std::vector<unsigned long long> outputs;

    int regVals[MAX_NUM_REGS];
};

static int maxN;
static int numTests;
static std::vector<__Test> tests;

static unsigned long long numOps;
static int maxDist;

static void myAssert(bool cond, const std::string& msg)
{
    if (cond) return;

    std::cout << PASSWORD << std::endl;
    std::cout << -1 << std::endl;
    std::cout << msg << std::endl;

    exit(0);
}

static void input()
{
    std::cin >> maxN;
    std::cin >> numTests;

    tests.resize(numTests);
    for (int t = 0; t < numTests; t++)
    {
        __Test& test = tests[t];

        std::cin >> test.n;

        test.numbers.resize(test.n);

        for (int i = 0; i < test.n; i++)
        {
            std::cin >> test.numbers[i];
        }

        test.inputs.resize(test.n + 1);
        test.outputs.resize(test.n);

        test.inputs[0] = test.n;
        for (int i = 0; i < test.n; i++)
        {
            test.inputs[i + 1] = test.numbers[i];
        }
    }
}

static void output()
{
    std::cout << PASSWORD << std::endl;
    std::cout << numOps << std::endl;
    std::cout << maxDist << std::endl;
}

static void encode(__Test& test, int regIdx, unsigned long long val)
{
    for (int i = 0; i < NUM_REGS_IN_NUMBER; i++)
    {
        test.regVals[regIdx + i] = val % NUM_REG_VALS;
        val /= NUM_REG_VALS;
    }
}

static unsigned long long decode(__Test& test, int regIdx)
{
    unsigned long long val = 0;
    for (int i = 0; i < NUM_REGS_IN_NUMBER; i++)
    {
        val *= NUM_REG_VALS;
        val += test.regVals[regIdx + NUM_REGS_IN_NUMBER - i - 1];
    }
    return val;
}

static void runProgram()
{
    unsigned long long seed = 0;

    for (int t = 0; t < numTests; t++)
    {
        for (unsigned long long val : tests[t].inputs)
        {
            seed ^= val;
        }
    }

    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> distr(0, NUM_REG_VALS - 1);

    for (int t = 0; t < numTests; t++)
    {
        __Test& test = tests[t];

        for (int i = 0; i < MAX_NUM_REGS; i++)
        {
            test.regVals[i] = distr(gen);
        }

        for (int i = 0; i < (int) test.inputs.size(); i++)
        {
            encode(test, i * NUM_REGS_IN_NUMBER, test.inputs[i]);
        }
    }

    sortNumbers(maxN);

    for (int t = 0; t < numTests; t++)
    {
        __Test& test = tests[t];

        for (int i = 0; i < test.n; i++)
        {
            test.outputs[i] = decode(test, i * NUM_REGS_IN_NUMBER);
        }

        std::sort(test.numbers.begin(), test.numbers.end());

        myAssert(test.outputs == test.numbers, "Wrong outputs.");
    }
}

void applyOp(int input1, int input2, int output, const TableT& opTable)
{
    numOps++;

    myAssert(numOps < TOO_MANY_OPS, "Too many operations.");

    myAssert(input1 >= 0 && input1 < MAX_NUM_REGS, "Invalid register.");
    myAssert(input2 >= 0 && input2 < MAX_NUM_REGS, "Invalid register.");
    myAssert(output >= 0 && output < MAX_NUM_REGS, "Invalid register.");

    maxDist = std::max(maxDist, std::abs(input1 - input2));
    maxDist = std::max(maxDist, std::abs(input1 - output));
    maxDist = std::max(maxDist, std::abs(input2 - output));

    myAssert(maxDist <= MAX_OP_DIST, "Register distance too large.");

    for (int t = 0; t < numTests; t++)
    {
        __Test& test = tests[t];

        int inVal1 = test.regVals[input1];
        int inVal2 = test.regVals[input2];

        int outVal = opTable[inVal1][inVal2];

        myAssert(outVal >= 0 && outVal < NUM_REG_VALS, "Invalid op table.");

        test.regVals[output] = outVal;
    }
}

int main()
{
    input();
    runProgram();
    output();

    return 0;
}
