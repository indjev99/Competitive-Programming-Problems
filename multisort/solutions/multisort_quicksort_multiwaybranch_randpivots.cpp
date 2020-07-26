#include "multisort.h"
#include <algorithm>
#include <numeric>
#include <unordered_set>
#include <random>

std::mt19937 generator;

std::vector<int> compTemp;
std::vector<std::vector<int>> branchTemps;
std::vector<int> pivots;
std::unordered_set<int> pivotSet;

void quickSort(int* data, int n, int k, int s)
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
    pivotSet.clear();
    std::for_each(branchTemps.begin(), branchTemps.end(), [](auto& v){ v.clear(); });
    std::shuffle(data, data + n, generator);
    std::copy(data, data + s, std::inserter(pivotSet, pivotSet.end()));
    for (int i = s; i < n; i += k - s)
    {
        compTemp.clear();
        std::copy(data, data + s, std::back_inserter(compTemp));
        std::copy(data + i, data + std::min(n, i + k - s), std::back_inserter(compTemp));
        compare(compTemp);
        if (i == s)
        {
            for (int num : compTemp)
            {
                if (pivotSet.find(num) != pivotSet.end()) pivots.push_back(num);
            }
        }
        int currBranch = 0;
        for (int num : compTemp)
        {
            if (currBranch < s && pivots[currBranch] == num) ++currBranch;
            else branchTemps[currBranch].push_back(num);
        }
    }
    int m = 0;
    std::vector<int> starts;
    std::vector<int> lens;
    for (int i = 0; i < s + 1; ++i)
    {
        starts.push_back(m);
        lens.push_back(branchTemps[i].size());
        std::copy(branchTemps[i].begin(), branchTemps[i].end(), data + m);
        m += branchTemps[i].size();
        if (i < s) data[m++] = pivots[i];
    }
    for (int i = 0; i < s + 1; ++i)
    {
        quickSort(data + starts[i], lens[i], k, s);
    }
}

std::vector<int> multisort(int n, int k)
{
    std::vector<int> res(n);
    std::iota(res.begin(), res.end(), 0);
    int s = 1 + (k > 3) + (k > 7) + (k > 9) + (k > 20);
    branchTemps.resize(s + 1);
    quickSort(res.data(), n, k, s);
    return res;
}
