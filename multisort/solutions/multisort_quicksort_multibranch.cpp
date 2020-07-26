#include "multisort.h"
#include <algorithm>
#include <numeric>

std::vector<int> compTemp;
std::vector<std::vector<int>> branchTemps;

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

    compTemp.clear();
    std::for_each(branchTemps.begin(), branchTemps.end(), [](auto& v){ v.clear(); });
    std::copy(data, data + k - 1, std::back_inserter(compTemp));
    for (int i = k - 1; i < n; ++i)
    {
        compTemp.push_back(data[i]);
        compare(compTemp);
        auto iter = std::find_if(compTemp.begin(), compTemp.end(), [&](int num){ return num == data[i]; });
        branchTemps[iter - compTemp.begin()].push_back(data[i]);
        compTemp.erase(iter);
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
        if (i < k - 1) data[m++] = compTemp[i];
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
