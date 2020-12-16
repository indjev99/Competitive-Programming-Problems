#include "multisort.h"
#include <algorithm>
#include <numeric>
#include <random>

std::mt19937 generator;

std::vector<int> compTemp;
std::vector<int> storeTemp;
std::vector<int> splits;
std::vector<int> idxs;
std::vector<int> toTake;
std::vector<int> taken;
std::vector<int> srcs;

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
        int curr = n * i / s;
        mergeSort(data + prev, curr - prev, k, s);
        prev = curr;
    }
    storeTemp.clear();
    for (int i = 0; i <= s; ++i)
    {
        int curr = n * i / s;
        if (i > 0) splits[i - 1] = curr;
        if (i < s) idxs[i] = curr;
    }
    while (true)
    {
        compTemp.clear();
        int notDone = 0;
        for (int i = 0; i < s; ++i)
        {
            if (idxs[i] < splits[i]) ++notDone;
        }
        if (notDone == 0) break;
        toTake.clear();
        int prev = 0;
        for (int i = 1; i <= notDone; ++i)
        {
            int curr = k * i / notDone;
            toTake.push_back(curr - prev);
            prev = curr;
        }
        std::shuffle(toTake.begin(), toTake.end(), generator);
        int ttPos = 0;
        for (int i = 0; i < s; ++i)
        {
            taken[i] = toTake[ttPos];
            for (int j = 0; j < toTake[ttPos] && idxs[i] + j < splits[i]; ++j)
            {
                compTemp.push_back(data[idxs[i] + j]);
                srcs[data[idxs[i] + j]] = i;
            }
            if (idxs[i] < splits[i]) ++ttPos;
        }
        if (notDone > 1) compare(compTemp);
        for (int elem : compTemp)
        {
            storeTemp.push_back(elem);
            ++idxs[srcs[elem]];
            if (--taken[srcs[elem]] == 0) break;
        }
    }
    std::copy(storeTemp.begin(), storeTemp.end(), data);
}

std::vector<int> multisort(int n, int k)
{
    std::vector<int> res(n);
    std::iota(res.begin(), res.end(), 0);
    int s = 2 + (k > 5) + (k > 18) + (k > 31);
    if (k > 58) s = 7 - (k > 91) - (k > 160) - (k > 313);
    if (k > 410) s = 7 - (k > 556) - (k > 800);
    splits.resize(s);
    idxs.resize(s);
    taken.resize(s);
    srcs.resize(n);
    mergeSort(res.data(), n, k, s);
    return res;
}
