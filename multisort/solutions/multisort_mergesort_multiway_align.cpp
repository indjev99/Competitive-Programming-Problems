#include "multisort.h"
#include <algorithm>
#include <numeric>

std::vector<int> compTemp;
std::vector<int> storeTemp;

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
        int leftComp = std::min(mid - i, (k + (mid - i > n - j)) / 2);
        int rightComp = std::min(n - j, k - leftComp);
        int leftEnd = i + std::min(mid - i, k - rightComp);
        int rightEnd = j + rightComp;
        compTemp.clear();
        std::copy(data + i, data + leftEnd, std::back_inserter(compTemp));
        std::copy(data + j, data + rightEnd, std::back_inserter(compTemp));
        compare(compTemp);
        int curr = 0;
        while (i < leftEnd && j < rightEnd)
        {
            int elem = compTemp[curr++];
            storeTemp.push_back(elem);
            if (i < mid && elem == data[i]) ++i;
            else ++j;
        }
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
