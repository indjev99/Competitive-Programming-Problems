#include "multisort.h"
#include <algorithm>
#include <numeric>

std::vector<int> leftTemp, rightTemp, compTemp;

bool cmpByAsking(int a, int b)
{
    compTemp = {a, b};
    compare(compTemp);
    return a == compTemp[0];
}

void quickSort(int* data, int n, int k)
{
    if (n == 0 || n == 1) return;

    if (n <= k)
    {
        compTemp.clear();
        std::copy(data, data + n, std::back_inserter(compTemp));
        compare(compTemp);
        std::copy(compTemp.begin(), compTemp.end(), data);
        return;
    }

    leftTemp.clear();
    rightTemp.clear();
    int pivot = data[0];
    for (int i = 1; i < n; ++i)
    {
        if (cmpByAsking(data[i], pivot)) leftTemp.push_back(data[i]);
        else rightTemp.push_back(data[i]);
    }
    int m = leftTemp.size();
    std::copy(leftTemp.begin(), leftTemp.end(), data);
    data[m] = pivot;
    std::copy(rightTemp.begin(), rightTemp.end(), data + m + 1);
    quickSort(data, m, k);
    quickSort(data + m + 1, n - m - 1, k);
}

std::vector<int> multisort(int n, int k)
{
    std::vector<int> res(n);
    std::iota(res.begin(), res.end(), 0);
    quickSort(res.data(), n, k);
    return res;
}
