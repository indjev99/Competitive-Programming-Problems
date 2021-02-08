#include <iostream>

const long long MOD = 1e9 + 7;
const int MAX_N = 5e5;

int n, k;
int cols[MAX_N];
long long ans;

void input()
{
    std::cin >> n >> k;
    for (int i = 0; i < n; ++i)
    {
        std::cin >> cols[i];
    }
}

void output()
{
    std::cout << ans << std::endl;
}


long long dp1[MAX_N]; // one end is fixed
long long dp2[MAX_N][2]; // both ends are fixed: same/different

void solve()
{
    dp1[0] = 1;
    for (int i = 1; i < n; ++i)
    {
        dp1[i] = dp1[i - 1] * (k - 1) % MOD;
    }

    dp2[0][0] = 0;
    dp2[0][1] = 1;
    for (int i = 1; i < n; ++i)
    {
        dp2[i][0] = dp2[i - 1][1] * (k - 1) % MOD;
        dp2[i][1] = (dp2[i - 1][1] * (k - 2) + dp2[i - 1][0]) % MOD;
    }

    ans = 1;
    int last = 0;
    int len = 0;
    for (int i = 0; i < n; ++i)
    {
        if (cols[i] == 0) ++len;
        else
        {
            if (last == 0) ans = ans * dp1[len] % MOD;
            else ans = ans * dp2[len][cols[i] != last] % MOD;
            last = cols[i];
            len = 0;
        }
    }
    if (last == 0) ans = k * dp1[len - 1] % MOD;
    else ans = ans * dp1[len] % MOD;
}

int main()
{
    std::ios::sync_with_stdio(false);

    input();
    solve();
    output();

    return 0;
}