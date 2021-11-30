#include <iostream>
#include <algorithm>
#include <vector>
#include <map>

struct CustomHeap
{
    int big;
    int small;

    CustomHeap(int big, int small):
        big(big),
        small(small) {}
};

struct NimHeap
{
    int big;
    int small;

    NimHeap(int big, int small):
        big(big),
        small(small) {}
};

int n, k;
std::vector<CustomHeap> heaps;

bool ans;

bool operator<(const CustomHeap& left, const CustomHeap& right)
{
    if (left.big != right.big) return left.big < right.big;
    return left.small < right.small;
}

bool operator!=(const NimHeap& left, const NimHeap& right)
{
    return left.big != right.big || left.small != right.small;
}

NimHeap operator^=(NimHeap& left, const NimHeap& right)
{
    left.big ^= right.big;
    left.small ^= right.small;
    return left;
}

void input()
{
    int big, small;
    std::cin >> n;
    heaps.clear();
    for (int i = 0; i < n; ++i)
    {
        std::cin >> big >> small;
        heaps.emplace_back(big, small);
    }
}

void output()
{
    if (ans) std::cout << "Win" << "\n";
    else std::cout << "Loss" << "\n";
}

bool isWin(const NimHeap& nimHeap)
{
    return nimHeap != NimHeap(0, 0);
}

const std::map<CustomHeap, NimHeap> nimEquivs = {
    {CustomHeap(1, 0), NimHeap(0, 0)},
    {CustomHeap(2, 0), NimHeap(0, 0)},
    {CustomHeap(2, 1), NimHeap(0, 1)},
    {CustomHeap(3, 0), NimHeap(0, 0)},
    {CustomHeap(3, 1), NimHeap(0, 2)},
    {CustomHeap(3, 2), NimHeap(1, 0)},
    {CustomHeap(4, 0), NimHeap(0, 0)},
    {CustomHeap(4, 1), NimHeap(0, 1)},
    {CustomHeap(4, 2), NimHeap(0, 3)},
    {CustomHeap(4, 3), NimHeap(1, 1)},
    {CustomHeap(5, 0), NimHeap(0, 0)},
    {CustomHeap(5, 1), NimHeap(0, 2)},
    {CustomHeap(5, 2), NimHeap(0, 4)},
    {CustomHeap(5, 3), NimHeap(1, 2)},
    {CustomHeap(5, 4), NimHeap(2, 0)}
};

NimHeap getNimEquiv(const CustomHeap& heap)
{
    if (heap.small >= heap.big * k)
    {
        return NimHeap(heap.big, heap.small - heap.big * k);
    }

    auto iter = nimEquivs.find(heap);
    if (iter != nimEquivs.end()) return iter->second;

    return NimHeap(0, 0);
}

void solve()
{
    NimHeap total(0, 0);
    for (const CustomHeap& heap : heaps)
    {
        total ^= getNimEquiv(heap);
    }
    ans = isWin(total);
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int q;
    std::cin >> k >> q;

    for (int i = 0; i < q; ++i)
    {
        input();
        solve();
        output();
    }

    return 0;
}
