#include <iostream>
#include <algorithm>

const int MAX_N = 1e6;

int n, m;
int a[MAX_N], b[MAX_N];
int ps[MAX_N * 2];
int ps2[MAX_N * 2];
int cnts[MAX_N * 2];

int ans;

void input()
{
    std::cin >> n >> m;
    for (int i = 0; i < n; ++i)
    {
        std::cin >> a[i] >> b[i];
    }
}

void output()
{
    std::cout << ans << std::endl;
}

void solve()
{
    int num = 0;
    for (int i = 0; i < n; ++i)
    {
        ps[num++] = a[i];
        ps[num++] = b[i] - 1;
    }
    ps[num++] = -m;

    std::sort(a, a + n);
    std::sort(b, b + n);
    std::sort(ps, ps + num);

    int num2 = 0;
    for (int i = 0; i < num; ++i)
    {
        if (i > 0 && ps[i] == ps[i - 1]) continue;
        ps2[num2++] = ps[i];
    }

    int cnt = 0;
    int aj = 0, bj = 0;
    for (int i = 0; i < num2; ++i)
    {
        while (aj < n && a[aj] <= ps2[i])
        {
            ++cnt;
            ++aj;
        }
        while (bj < n && b[bj] <= ps2[i])
        {
            --cnt;
            ++bj;
        }
        cnts[i] = cnt;
    }

    ans = 0;
    int prefMax = 0;
    for (int i = 0; i < num2 - 1; ++i)
    {
        prefMax = std::max(prefMax, cnts[i]);
        int curr = prefMax + cnts[i + 1];
        ans = std::max(ans, curr);
    }
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