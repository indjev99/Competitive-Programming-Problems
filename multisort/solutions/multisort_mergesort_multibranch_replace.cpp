#include "multisort.h"
#include <algorithm>
#include <numeric>

std::vector<int> storeTemp, compTemp, splitsTemp, idxsTemp, srcTemp;

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
    compTemp.clear();
    for (int i = 0; i <= k; ++i)
    {
        int curr = (n * i + k / 2) / k;
        if (i > 0) splitsTemp[i - 1] = curr;
        if (i < k)
        {
            idxsTemp[i] = curr;
            compTemp.push_back(data[idxsTemp[i]]);
            srcTemp[data[idxsTemp[i]]] = i;
        }
    }
    while (!compTemp.empty())
    {
        if (compTemp.size() > 1) compare(compTemp);
        storeTemp.push_back(compTemp[0]);
        int src = srcTemp[compTemp[0]];
        if (++idxsTemp[src] < splitsTemp[src])
        {
            compTemp[0] = data[idxsTemp[src]];
            srcTemp[data[idxsTemp[src]]] = src;
        }
        else
        {
            std::swap(compTemp[0], compTemp.back());
            compTemp.pop_back();
        }
    }
    std::copy(storeTemp.begin(), storeTemp.end(), data);
}

std::vector<int> multisort(int n, int k)
{
    std::vector<int> res(n);
    std::iota(res.begin(), res.end(), 0);
    splitsTemp.resize(k);
    idxsTemp.resize(k);
    srcTemp.resize(n);
    mergeSort(res.data(), n, k);
    return res;
}
