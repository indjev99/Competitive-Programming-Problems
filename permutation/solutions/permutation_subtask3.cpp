#include <iostream>
#include <algorithm>

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

void solve()
{
    if (n % 2 == 0) ans = 1;
    else ans = 0;
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
