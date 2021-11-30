#include <iostream>
#include <vector>

int n, k;
std::vector<int> heaps;

bool ans;

void input()
{
    int big, small;
    std::cin >> n;
    heaps.clear();
    for (int i = 0; i < n; ++i)
    {
        std::cin >> big >> small;
        heaps.push_back(small);
    }
}

void output()
{
    if (ans) std::cout << "Win" << "\n";
    else std::cout << "Loss" << "\n";
}

bool isWin(int nimHeap)
{
    return nimHeap != 0;
}

void solve()
{
    int total = 0;
    for (int heap : heaps)
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
