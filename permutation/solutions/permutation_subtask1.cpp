#include <iostream>
#include <algorithm>
#include <numeric>

const int MAX_N = 1e6;
const int MOD = 1e9 + 7;

int n;
int q[MAX_N];

int p[MAX_N];
long long ans;

void input()
{
    std::cin >> n;
    for (int i = 0; i < n; ++i)
    {
        std::cin >> q[i];
    }
}

void output()
{
    std::cout << ans << std::endl;
}

bool isValid()
{
    for (int i = 0; i < n; ++i)
    {
        if (!(i > 0 && p[i - 1] == q[i]) && !(i < n - 1 && p[i + 1] == q[i])) return false;
    }
    return true;
}

void solve()
{
    std::iota(p, p + n, 1);

    do
    {
        if (isValid()) ans = (ans + 1) % MOD;
    }
    while (std::next_permutation(p, p + n));
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    input();
    solve();
    output();

    return 0;
}
