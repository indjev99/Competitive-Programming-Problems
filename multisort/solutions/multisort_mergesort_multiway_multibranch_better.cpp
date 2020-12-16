#include "multisort.h"
#include <algorithm>
#include <numeric>

std::vector<int> storeTemp, compTemp, splitsTemp, idxsTemp, takenTemp, srcTemp;

void mergeSort(int* data, int n, int k, int s)
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
    for (int i = 1; i <= s; ++i)
    {
        int curr = (n * i + s / 2) / s;
        mergeSort(data + prev, curr - prev, k, s);
        prev = curr;
    }
    storeTemp.clear();
    for (int i = 0; i <= s; ++i)
    {
        int curr = (n * i + s / 2) / s;
        if (i > 0) splitsTemp[i - 1] = curr;
        if (i < s) idxsTemp[i] = curr;
    }
    while (true)
    {
        compTemp.clear();
        int notDone = 0;
        for (int i = 0; i < s; ++i)
        {
            takenTemp[i] = 0;
            if (idxsTemp[i] < splitsTemp[i]) ++notDone;
        }
        if (notDone == 0) break;
        int toTake = k / notDone;
        for (int i = 0; i < s; ++i)
        {
            for (int j = 0; j < toTake && idxsTemp[i] + j < splitsTemp[i]; ++j)
            {
                compTemp.push_back(data[idxsTemp[i] + j]);
                srcTemp[data[idxsTemp[i] + j]] = i;
            }
        }
        if (notDone > 1) compare(compTemp);
        for (int elem : compTemp)
        {
            storeTemp.push_back(elem);
            ++idxsTemp[srcTemp[elem]];
            if (++takenTemp[srcTemp[elem]] == toTake) break;
        }
    }
    std::copy(storeTemp.begin(), storeTemp.end(), data);
}

std::vector<int> multisort(int n, int k)
{
    std::vector<int> res(n);
    std::iota(res.begin(), res.end(), 0);
    int s = 2 + (k > 5) + (k > 15) + (k > 30);
    splitsTemp.resize(s);
    idxsTemp.resize(s);
    takenTemp.resize(s);
    srcTemp.resize(n);
    mergeSort(res.data(), n, k, s);
    return res;
}
