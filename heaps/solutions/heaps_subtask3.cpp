#include <iostream>
#include <vector>

struct NimHeap
{
    int big;
    int small;

    NimHeap(int big, int small):
        big(big),
        small(small) {}
};

int n, k;
std::vector<NimHeap> heaps;

bool ans;

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

void solve()
{
    NimHeap total(0, 0);
    for (const NimHeap& heap : heaps)
    {
        total ^= heap;
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
