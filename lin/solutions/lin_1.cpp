#include <iostream>

const int MAX_N = 5e3;
const int MAX_M = 1e4;
const long long MOD = 1e9 + 7;

int n;
long long m;

long long a[MAX_M];

long long ans;

void input()
{
    std::cin >> n >> m;

    for (int i = 0; i < n; i++)
    {
        std::cin >> a[i];
    }
}

long long pascal[MAX_N][MAX_N];
long long coeffs[MAX_N];

void solve()
{
    for (int i = 0; i <= n; i++)
    {
        pascal[i][0] = 1;
        pascal[i][i] = 1;
        for (int j = 1; j < i; j++)
        {
            pascal[i][j] = pascal[i - 1][j - 1] + pascal[i - 1][j];
            pascal[i][j] %= MOD;
        }
    }

    for (int i = 0; i < n; i++)
    {
        if ((n - i) % 2 == 0) coeffs[i] = MOD - pascal[n][i];
        else coeffs[i] = pascal[n][i];
    }

    for (int i = n; i <= m; i++)
    {
        a[i] = 0;
        for (int j = 0; j < n; j++)
        {
            a[i] += coeffs[j] * a[i - n + j];
            a[i] %= MOD;
        }
    }

    ans = a[m];
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
