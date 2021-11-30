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

bool isWin(bool big, int nimHeap)
{
    return big || nimHeap != 0;
}

void solve()
{
    bool totalBig = false;
    int total = 0;;
    for (const NimHeap& heap : heaps)
    {
        totalBig ^= heap.big != 0;
        total ^= heap.small;
    }
    ans = isWin(totalBig, total);
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
