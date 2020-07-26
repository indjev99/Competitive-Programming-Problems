#include "multisort.h"
#include <algorithm>
#include <numeric>
#include <random>

std::mt19937 generator;

std::vector<int> compTemp;
std::vector<std::vector<int>> branchTemps;
std::vector<int> pivots;

std::vector<int> bestS;
std::vector<int> bestQs;

int currT;
std::vector<int> lastUsed;

void genSample(int n, int k)
{
    ++currT;
    compTemp.clear();
    std::uniform_int_distribution<int> distribution(0, n - 1);
    for (int i = 0; i < k; ++i)
    {
        int num = distribution(generator);
        if (lastUsed[num] < currT) lastUsed[num] = currT;
        else --i;
    }
    for (int i = 0; i < n; ++i)
    {
        if (lastUsed[i] == currT) compTemp.push_back(i);
    }
}

int attempt(int n, int k, int s)
{
    int offset = (k - 1 + s / 2) / s / 2;
    int qs = (n + k - 2 * s + 1) / (k - s);
    int pos = 0;
    int prev = 0;
    for (int j = 0; j < s; ++j)
    {
        pos += (k - 1 - pos + (s - j) / 2) / (s - j);
        int pivot = compTemp[pos - offset];
        qs += bestQs[pivot - prev];
        prev = pivot + 1;
    }
    qs += bestQs[n - prev];
    return qs;
}

void findBestS(int n, int k)
{
    if (n <= k)
    {
        bestS[n] = 1;
        bestQs[n] = n > 1;
        return;
    }

    genSample(n, k);
    int l = std::max(1, bestS[n - 1] - 2);
    int r = std::min(k / 2, bestS[n - 1] + 2);
    for (int s = l; s <= r; ++s)
    {
        int currQs = attempt(n, k, s);
        if (currQs < bestQs[n] || s == l)
        {
            bestQs[n] = currQs;
            bestS[n] = s;
        }
    }
}

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

    int s = bestS[n];
    pivots.clear();
    branchTemps.resize(s + 1);
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
        quickSort(data + starts[i], lens[i], k);
    }
}

std::vector<int> multisort(int n, int k)
{
    bestS.resize(n + 1);
    bestQs.resize(n + 1);
    lastUsed.resize(n);
    for (int i = 0; i <= n; ++i)
    {
        findBestS(i, k);
    }

    std::vector<int> res(n);
    std::iota(res.begin(), res.end(), 0);
    quickSort(res.data(), n, k);
    return res;
}
