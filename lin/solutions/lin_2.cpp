#include <iostream>

const int MAX_N = 2e2;
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

long long tmpMatrix[MAX_N][MAX_N];

void idMatrix(long long res[MAX_N][MAX_N])
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (j == i) res[i][j] = 1;
            else res[i][j] = 0;
        }
    }
}

void cpMatrix(const long long source[MAX_N][MAX_N], long long res[MAX_N][MAX_N])
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            res[i][j] = source[i][j];
        }
    }
}

void mulMatrix(const long long left[MAX_N][MAX_N], const long long right[MAX_N][MAX_N], long long res[MAX_N][MAX_N])
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            res[i][j] = 0;
        }
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < n; k++)
            {
                res[i][k] += left[i][j] * right[j][k];
                res[i][k] %= MOD;
            }
        }
    }
}

void powMatrix(const long long base[MAX_N][MAX_N], long long exp, long long res[MAX_N][MAX_N])
{
    if (exp == 0)
    {
        idMatrix(res);
        return;
    }

    powMatrix(base, exp / 2, res);

    mulMatrix(res, res, tmpMatrix);

    if (exp % 2 == 0) cpMatrix(tmpMatrix, res);
    else mulMatrix(base, tmpMatrix, res);
}

long long pascal[MAX_N][MAX_N];
long long recMatrix[MAX_N][MAX_N];
long long resMatrix[MAX_N][MAX_N];

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
        if ((n - i) % 2 == 0) recMatrix[n - 1][i] = MOD - pascal[n][i];
        else recMatrix[n - 1][i] = pascal[n][i];
    }

    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (j == i + 1) recMatrix[i][j] = 1;
            else recMatrix[i][j] = 0;
        }
    }

    powMatrix(recMatrix, m - n + 1, resMatrix);

    ans = 0;

    for (int i = 0; i < n; i++)
    {
        ans += resMatrix[n - 1][i] * a[i];
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
