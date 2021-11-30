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
    bool infRun;

    NimHeap(int big, int small):
        big(big),
        small(small),
        infRun(false) {}
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

std::vector<NimHeap> getMexes(std::vector<NimHeap>& nimSet)
{
    std::sort(nimSet.begin(), nimSet.end());
    std::vector<NimHeap> mexes;
    NimHeap next(0, 0);
    for (const NimHeap& nimHeap : nimSet)
    {
        if (nimHeap < next) continue;
        while (next < nimHeap)
        {
            mexes.push_back(next);
            ++next.small;
        }
        if (nimHeap.infRun)
        {
            ++next.big;
            next.small = 0;
        }
        else ++next.small;
    }
    return mexes;
}

bool foundNimEquivs[MAX_B + 1];
std::vector<NimHeap> nimEquivs[MAX_B + 1];

NimHeap getNimEquiv(const CustomHeap& heap);

void findNimEquivs(int heapBig)
{
    foundNimEquivs[heapBig] = true;

    std::vector<NimHeap> nimSet;
    for (int big = 0; big < heapBig; ++big)
    {
        int minSmall = (heapBig - big) * k;
        int maxSmall = std::max(minSmall, big * k);
        for (int small = minSmall; small <= maxSmall; ++small)
        {
            NimHeap nimHeap = getNimEquiv(CustomHeap(big, small));
            if (small == maxSmall) nimHeap.infRun = true;
            nimSet.push_back(nimHeap);
        }
    }

    std::vector<NimHeap> mexes = getMexes(nimSet);
    for (int small = 0; small < heapBig * k; ++small)
    {
        nimEquivs[heapBig].push_back(mexes[small]);
    }
}

NimHeap getNimEquiv(const CustomHeap& heap)
{
    if (heap.small >= heap.big * k)
    {
        return NimHeap(heap.big, heap.small - heap.big * k);
    }

    if (!foundNimEquivs[heap.big]) findNimEquivs(heap.big);
    return nimEquivs[heap.big][heap.small];
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
