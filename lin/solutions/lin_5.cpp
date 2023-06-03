#include <iostream>

const int MAX_N = 1e6;
const long long MOD = 1e9 + 7;

int n;
long long m;

long long a[MAX_N];

long long ans;

void input()
{
    std::cin >> n >> m;

    for (int i = 0; i < n; i++)
    {
        std::cin >> a[i];
    }
}

long long qPow(long long base, long long exp)
{
    if (exp == 0) return 1;

    long long res = qPow(base, exp / 2);
    res = res * res % MOD;

    if (exp % 2 == 1) res = base * res % MOD;

    return res;
}

long long modInv(long long a)
{
    return qPow(a, MOD - 2);
}

void solve()
{
    m %= MOD;

    long long t = 0;
    for (t = 0; t < n; t++)
    {
        if (a[t] != 0) break;
    }

    if (t == n)
    {
        ans = 0;
        return;
    }

    long long prod = 1;
    for (int i = 0; i < n; i++)
    {
        if (i == t) continue;
        prod *= (i - t + MOD) % MOD;
        prod %= MOD;
    }

    long long prodInv = modInv(prod);

    long long D = a[t] * prodInv % MOD;

    ans = D;

    for (int i = 0; i < n; i++)
    {
        if (i == t) continue;
        ans *= (i - m + MOD) % MOD;
        ans %= MOD;
    }
}

void output()
{
    std::cout << ans << std::endl;
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
