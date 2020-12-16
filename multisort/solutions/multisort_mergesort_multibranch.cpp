#include "multisort.h"
#include <algorithm>
#include <numeric>

std::vector<int> compTemp;
std::vector<int> storeTemp;
std::vector<int> splits;
std::vector<int> idxs;
std::vector<int> srcs;

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

    int prev = 0;
    for (int i = 1; i <= k; ++i)
    {
        int curr = (n * i + k / 2) / k;
        mergeSort(data + prev, curr - prev, k);
        prev = curr;
    }
    storeTemp.clear();
    for (int i = 0; i <= k; ++i)
    {
        int curr = (n * i + k / 2) / k;
        if (i > 0) splits[i - 1] = curr;
        if (i < k) idxs[i] = curr;
    }
    while (true)
    {
        compTemp.clear();
        for (int i = 0; i < k; ++i)
        {
            if (idxs[i] < splits[i])
            {
                compTemp.push_back(data[idxs[i]]);
                srcs[data[idxs[i]]] = i;
            }
        }
        if (compTemp.empty()) break;
        if (compTemp.size() > 1) compare(compTemp);
        storeTemp.push_back(compTemp[0]);
        ++idxs[srcs[compTemp[0]]];
    }
    std::copy(storeTemp.begin(), storeTemp.end(), data);
}

std::vector<int> multisort(int n, int k)
{
    std::vector<int> res(n);
    std::iota(res.begin(), res.end(), 0);
    splits.resize(k);
    idxs.resize(k);
    srcs.resize(n);
    mergeSort(res.data(), n, k);
    return res;
}
