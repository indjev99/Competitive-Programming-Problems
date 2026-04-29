#include "triangle.h"
#include <numeric>

std::vector<int> solve(int n)
{
    std::vector<int> answer(n);
    std::iota(answer.begin(), answer.end(), 1);
    return answer;
}