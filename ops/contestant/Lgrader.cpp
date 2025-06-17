#include "ops.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <random>

static int maxN;

static int n;

static std::vector<unsigned long long> numbers;
static std::vector<unsigned long long> inputs;
static std::vector<unsigned long long> outputs;

static int regVals[MAX_NUM_REGS];

static unsigned long long numOps;
static int maxDist;

static void myAssert(bool cond, const std::string& msg)
{
    if (cond) return;

    std::cout << msg << std::endl;
    exit(0);
}

static void input()
{
    std::cout << "Max N:" << std::endl;
    std::cin >> maxN;

    std::cout << "N:" << std::endl;
    std::cin >> n;

    numbers.resize(n);

    std::cout << "Numbers:" << std::endl;
    for (int i = 0; i < n; i++)
    {
        std::cin >> numbers[i];
    }

    inputs.resize(n + 1);
    outputs.resize(n);

    inputs[0] = n;
    for (int i = 0; i < n; i++)
    {
        inputs[i + 1] = numbers[i];
    }
}

static void output()
{
    std::cout << "Sorted numbers:" << std::endl;
    for (int i = 0; i < n; i++)
    {
        if (i > 0) std::cout << " ";
        std::cout << outputs[i];
    }
    std::cout << std::endl;

    std::cout << "Number of operations:" << std::endl;
    std::cout << numOps << std::endl;

    std::cout << "Max operation distance:" << std::endl;
    std::cout << maxDist << std::endl;

    if (maxDist > MAX_OP_DIST)
    {
        std::cout << "Register distance too large." << std::endl;
    }

    std::sort(numbers.begin(), numbers.end());

    if (outputs != numbers)
    {
        std::cout << "Wrong outputs." << std::endl;
    }
}

static void encode(int regIdx, unsigned long long val)
{
    for (int i = 0; i < NUM_REGS_IN_NUMBER; i++)
    {
        regVals[regIdx + i] = val % NUM_REG_VALS;
        val /= NUM_REG_VALS;
    }
}

static unsigned long long decode(int regIdx)
{
    unsigned long long val = 0;
    for (int i = 0; i < NUM_REGS_IN_NUMBER; i++)
    {
        val *= NUM_REG_VALS;
        val += regVals[regIdx + NUM_REGS_IN_NUMBER - i - 1];
    }
    return val;
}

static void runProgram()
{
    unsigned long long seed = 0;
    for (unsigned long long val : inputs)
    {
        seed ^= val;
    }

    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> distr(0, NUM_REG_VALS - 1);

    for (int i = 0; i < MAX_NUM_REGS; i++)
    {
        regVals[i] = distr(gen);
    }

    for (int i = 0; i < (int) inputs.size(); i++)
    {
        encode(i * NUM_REGS_IN_NUMBER, inputs[i]);
    }

    sortNumbers(maxN);

    for (int i = 0; i < n; i++)
    {
        outputs[i] = decode(i * NUM_REGS_IN_NUMBER);
    }
}

void applyOp(int input1, int input2, int output, const TableT& opTable)
{
    numOps++;

    myAssert(input1 >= 0 && input1 < MAX_NUM_REGS, "Invalid register.");
    myAssert(input2 >= 0 && input2 < MAX_NUM_REGS, "Invalid register.");
    myAssert(output >= 0 && output < MAX_NUM_REGS, "Invalid register.");

    maxDist = std::max(maxDist, std::abs(input1 - input2));
    maxDist = std::max(maxDist, std::abs(input1 - output));
    maxDist = std::max(maxDist, std::abs(input2 - output));

    int inVal1 = regVals[input1];
    int inVal2 = regVals[input2];

    int outVal = opTable[inVal1][inVal2];

    myAssert(outVal >= 0 && outVal < NUM_REG_VALS, "Invalid op table.");

    regVals[output] = outVal;

    // std::cerr << "Op: " << input1 << "=" << inVal1 << " " << input2 << "=" << inVal2 << " -> " << output << "=" << outVal << std::endl;
}

int main()
{
    input();
    runProgram();
    output();

    return 0;
}
