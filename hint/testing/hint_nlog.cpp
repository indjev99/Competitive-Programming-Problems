#include "hint.h"
#include <algorithm>

int findNumBits(int maxNum)
{
    int numBits = 0;
    while ((1 << numBits) < maxNum) ++numBits;
    return numBits;
}

std::vector<bool> genHint(const std::vector<int>& a, const std::vector<int>& b, const std::vector<int>& sol)
{
    std::vector<bool> hint;

    int n = a.size();
    int m = b.size();

    int numBits = findNumBits(std::min(n, m));

    for (int num : sol)
    {
        for (int i = 0; i < numBits; ++i)
        {
            hint.push_back(num % 2);
            num /= 2;
        }
    }

    return hint;
}

std::vector<int> solve(const std::vector<int>& a, const std::vector<int>& b, const std::vector<bool>& hint)
{
    std::vector<int> sol;

    int n = a.size();
    int m = b.size();
    int k = hint.size();

    int numBits = findNumBits(std::min(n, m));

    for (int i = 0; i < k / numBits; ++i)
    {
        int num = 0;
        for (int j = 0; j < numBits; ++j)
        {
            num += hint[i * numBits + j] * (1 << j);
        }
        sol.push_back(num);
    }

    return sol;
}
