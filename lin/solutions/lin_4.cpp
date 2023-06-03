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

    long long fact = 1;
    for (int i = 1; i < n; i++)
    {
        fact *= i;
        fact %= MOD;
    }

    long long factInv = modInv(fact);

    long long D = a[0] * factInv % MOD;

    long long E = (a[1] * factInv % MOD * (n - 1) % MOD - D + MOD) % MOD;

    ans = (D + E * m % MOD) % MOD;

    for (int i = 2; i < n; i++)
    {
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
