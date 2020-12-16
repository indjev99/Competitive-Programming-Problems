#include "multisort.h"
#include <algorithm>
#include <numeric>
#include <random>

std::mt19937 generator;

std::vector<int> compTemp;
std::vector<int> leftTemp, rightTemp;

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
    std::uniform_int_distribution<int> distribution(0, n - 1);
    std::swap(data[0], data[distribution(generator)]);
    int pivot = data[0];
    for (int i = 1; i < n; i += k - 1)
    {
        compTemp.clear();
        compTemp.push_back(pivot);
        std::copy(data + i, data + std::min(n, i + k - 1), std::back_inserter(compTemp));
        compare(compTemp);
        bool foundPivot = false;
        for (int num : compTemp)
        {
            if (num == pivot) foundPivot = true;
            else if (foundPivot) rightTemp.push_back(num);
            else leftTemp.push_back(num);
        }
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
