#include "multisort.h"
#include <algorithm>
#include <numeric>
#include <random>

std::mt19937 generator;

std::vector<int> compTemp;
std::vector<std::vector<int>> branchTemps;
std::vector<int> pivots;

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
    std::for_each(branchTemps.begin(), branchTemps.end(), [](auto& v){ v.clear(); });
    std::shuffle(data, data + n, generator);
    for (int i = s; i < n; i += k - s)
    {
        compTemp.clear();
        std::copy(data, data + s, std::back_inserter(compTemp));
        std::copy(data + i, data + std::min(n, i + k - s), std::back_inserter(compTemp));
        compare(compTemp);
        if (i == s)
        {
            int offset = (k - 1 + s / 2) / s / 2;
            int pos = 0;
            for (int j = 0; j < s; ++j)
            {
                pos += (k - 1 - pos + (s - j) / 2) / (s - j);
                pivots.push_back(compTemp[pos - offset]);
                data[j] = compTemp[pos - offset];
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
    int s = 1 + (k > 3) + (k > 7) + (k > 9) + 2 * (k > 90);
    if (k > 180) s = 12 - (k > 250) - (k > 290) - (k > 350) - (k > 410) - (k > 510) - (k > 640) - (k > 880);
    branchTemps.resize(s + 1);
    quickSort(res.data(), n, k, s);
    return res;
}
