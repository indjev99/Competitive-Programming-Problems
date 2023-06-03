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

    ans = 0;

    long long mProd = 1;

    for (int i = 0; i < n; i++)
    {
        mProd *= (i - m + MOD) % MOD;
        mProd %= MOD;
    }

    long long prod = 1;
    for (int i = 1; i < n; i++)
    {
        prod *= i;
        prod %= MOD;
    }

    for (int t = 0; t < n; t++)
    {
        if (t > 0)
        {
            prod *= modInv(n - t);
            prod %= MOD;

            prod *= MOD - t;
            prod %= MOD;
        }

        long long prodInv = modInv(prod);

        long long D = a[t] * prodInv % MOD;

        long long excl = (t - m + MOD) % MOD;

        long long exclInv = modInv(excl);

        long long curr = D * mProd % MOD * exclInv % MOD;

        ans += curr;
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
