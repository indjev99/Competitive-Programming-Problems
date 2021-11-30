#include <iostream>
#include <algorithm>
#include <vector>

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

const int MAX_B = 3000;

int n, k;
std::vector<CustomHeap> heaps;

bool ans;

bool operator!=(const NimHeap& left, const NimHeap& right)
{
    return left.big != right.big || left.small != right.small;
}

bool operator<(const NimHeap& left, const NimHeap& right)
{
    return left.big < right.big || (left.big == right.big && left.small < right.small);
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

bool foundNimEquivs[MAX_B + 1];
std::vector<NimHeap> nimEquivs[MAX_B + 1];

NimHeap getNimEquiv(const CustomHeap& heap);

void findNimEquivs(int heapBig)
{
    foundNimEquivs[heapBig] = true;

    for (int big = 0; big < heapBig; ++big)
    {
        int minSmall = (heapBig - big) * k;
        nimEquivs[heapBig].push_back(getNimEquiv(CustomHeap(big, minSmall - k)));
    }

    std::sort(nimEquivs[heapBig].begin(), nimEquivs[heapBig].end());
}

NimHeap getNimEquiv(const CustomHeap& heap)
{
    if (heap.small >= heap.big * k)
    {
        return NimHeap(heap.big, heap.small - heap.big * k);
    }

    if (!foundNimEquivs[heap.big]) findNimEquivs(heap.big);
    NimHeap baseAns = nimEquivs[heap.big][heap.small / k];
    return NimHeap(baseAns.big, baseAns.small + heap.small % k);
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
