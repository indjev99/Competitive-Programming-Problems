#include "multisort.h"
#include <algorithm>
#include <numeric>
#include <iostream>

std::vector<int> compTemp;
std::vector<std::vector<int>> branchTemps;
std::vector<int> pivots;

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

    pivots.clear();
    compTemp.clear();
    std::for_each(branchTemps.begin(), branchTemps.end(), [](auto& v){ v.clear(); });
    std::copy(data, data + k - 1, std::back_inserter(compTemp));
    compTemp.push_back(0);
    int lastPos = k - 1;
    for (int i = k - 1; i < n; ++i)
    {
        compTemp[lastPos] = data[i];
        compare(compTemp);
        if (i == k - 1)
        {
            for (int j = 0; j < k; ++j)
            {
                if (compTemp[j] != data[i]) pivots.push_back(compTemp[j]);
                else lastPos = j;
            }
        }
        else
        {
            int l = 0, r = k - 1;
            while (r > l)
            {
                int mid = (l + r) / 2;
                if (compTemp[mid] == pivots[mid]) l = mid + 1;
                else r = mid;
            }
            lastPos = l;
        }
        branchTemps[lastPos].push_back(data[i]);
    }
    int m = 0;
    std::vector<int> starts;
    std::vector<int> lens;
    for (int i = 0; i < k; ++i)
    {
        starts.push_back(m);
        lens.push_back(branchTemps[i].size());
        std::copy(branchTemps[i].begin(), branchTemps[i].end(), data + m);
        m += branchTemps[i].size();
        if (i < k - 1) data[m++] = pivots[i];
    }
    for (int i = 0; i < k; ++i)
    {
        quickSort(data + starts[i], lens[i], k);
    }
}

std::vector<int> multisort(int n, int k)
{
    std::vector<int> res(n);
    std::iota(res.begin(), res.end(), 0);
    branchTemps.resize(k);
    quickSort(res.data(), n, k);
    return res;
}
