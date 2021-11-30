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
    bool infRun;

    NimHeap(int big, int small):
        big(big),
        small(small),
        infRun(false) {}
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

NimHeap getMex(std::vector<NimHeap>& nimSet)
{
    std::sort(nimSet.begin(), nimSet.end());
    NimHeap next(0, 0);
    for (const NimHeap& nimHeap : nimSet)
    {
        if (nimHeap < next) continue;
        if (next < nimHeap) return next;
        if (nimHeap.infRun)
        {
            ++next.big;
            next.small = 0;
        }
        else ++next.small;
    }
    return next;
}

std::map<CustomHeap, NimHeap> nimEquivs;

NimHeap getNimEquiv(const CustomHeap& heap)
{
    if (heap.small >= heap.big * k)
    {
        return NimHeap(heap.big, heap.small - heap.big * k);
    }

    auto iter = nimEquivs.find(heap);
    if (iter != nimEquivs.end()) return iter->second;

    std::vector<NimHeap> nimSet;
    for (int big = 0; big < heap.big; ++big)
    {
        int minSmall = (heap.big - big) * k;
        int maxSmall = std::max(minSmall, big * k);
        for (int small = minSmall; small <= maxSmall; ++small)
        {
            NimHeap nimHeap = getNimEquiv(CustomHeap(big, small));
            if (small == maxSmall) nimHeap.infRun = true;
            nimSet.push_back(nimHeap);
        }
    }
    for (int small = 0; small < heap.small; ++small)
    {
        NimHeap nimHeap = getNimEquiv(CustomHeap(heap.big, small));
        nimSet.push_back(nimHeap);
    }

    NimHeap nimHeap = getMex(nimSet);
    nimEquivs.insert({heap, nimHeap});

    return nimHeap;
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
