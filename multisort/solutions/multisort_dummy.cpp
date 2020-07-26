#include "multisort.h"
#include <algorithm>
#include <numeric>

std::vector<int> compTemp;

bool cmpByAsking(int a, int b)
{
    compTemp = {a, b};
    compare(compTemp);
    return a == compTemp[0];
}

std::vector<int> multisort(int n, int k)
{
    std::vector<int> res(n);
    std::iota(res.begin(), res.end(), 0);
    std::sort(res.begin(), res.end(), cmpByAsking);
    return res;
}
