#include "multisort.h"
#include <algorithm>
#include <numeric>

std::vector<int> storeTemp, compTemp;

bool cmpByAsking(int a, int b)
{
    compTemp = {a, b};
    compare(compTemp);
    return a == compTemp[0];
}

void mergeSort(int* data, int n, int k)
{
    if (n == 1) return;

    if (n <= k)
    {
        compTemp.clear();
        std::copy(data, data + n, std::back_inserter(compTemp));
        compare(compTemp);
        std::copy(compTemp.begin(), compTemp.end(), data);
        return;
    }

    int mid = n / 2;
    if (mid > k) mid = (mid + k / 2) / k * k;
    mergeSort(data, mid, k);
    mergeSort(data + mid, n - mid, k);
    storeTemp.clear();
    int i = 0;
    int j = mid;
    while (i < mid && j < n)
    {
        if (cmpByAsking(data[i], data[j])) storeTemp.push_back(data[i++]);
        else storeTemp.push_back(data[j++]);
    }
    if (i < mid) std::copy(data + i, data + mid, std::back_inserter(storeTemp));
    else std::copy(data + j, data + n, std::back_inserter(storeTemp));
    std::copy(storeTemp.begin(), storeTemp.end(), data);
}

std::vector<int> multisort(int n, int k)
{
    std::vector<int> res(n);
    std::iota(res.begin(), res.end(), 0);
    mergeSort(res.data(), n, k);
    return res;
}
