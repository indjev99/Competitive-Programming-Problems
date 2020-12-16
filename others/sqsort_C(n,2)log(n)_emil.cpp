#include "sqsort.h"
#include <vector>
#include <numeric>
#include <algorithm>
#include <queue>

std::vector<int> perm;
std::vector<std::pair<int, int>> pairs;
std::vector<std::vector<std::pair<int, int>>> buckets;
std::vector<std::pair<int, int>> temp;

bool cmpElems(int x, int y)
{
    return cmp(x, x, y, y);
}

bool cmpPairs(std::pair<int, int> x, std::pair<int, int> y)
{
    if (x.first <= y.first && x.second <= y.second) return true;
    if (x.first >= y.first && x.second >= y.second) return false;
    return cmp(perm[x.first], perm[x.second], perm[y.first], perm[y.second]);
}

void findPerm(int n)
{
	perm.resize(n);
    std::iota(perm.begin(), perm.end(), 0);
    std::stable_sort(perm.begin(), perm.end(), cmpElems);
}

void fillBuckets(int n)
{
    buckets.resize(n);
    for (int i = 0; i < n; ++i)
    {
        buckets[i].clear();
    }

    for (int i = 0; i < n; ++i)
    {
        for (int j = i; j < n; ++j)
        {
            if ((int)buckets[i].size() <= n / 2) buckets[i].push_back({i, j});
            else buckets[j].push_back({i, j});
        }
    }
}

void mergeTwoBuckets(int i, int j)
{
    auto& left = buckets[i];
    auto& right = buckets[j];
    temp.resize(left.size() + right.size());
    std::merge(left.begin(), left.end(), right.begin(), right.end(), temp.begin(), cmpPairs);
    std::swap(temp, left);
}

void mergeAllBuckets(int n)
{
    for (int dist = 1; dist < n; dist *= 2)
    {
        for (int pos = 0; pos + dist < n; pos += dist * 2)
        {
            mergeTwoBuckets(pos, pos + dist);
        }
    }
}

void sortPairs(int n)
{
    fillBuckets(n);
    mergeAllBuckets(n);
    pairs = buckets[0];
}

void renamePairs()
{
    for (auto& curr : pairs)
    {
        curr.first = perm[curr.first];
        curr.second = perm[curr.second];
    }
}

std::vector<std::pair<int, int>> solve(int n) 
{
    findPerm(n);
    sortPairs(n);
    renamePairs();
	return pairs;
}

