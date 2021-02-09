#include <iostream>

const int MAX_N = 1e6;

int n, m;
int a[MAX_N], b[MAX_N];
int ps[MAX_N * 2];
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
        ps[num++] = b[i] - m;
    }
    ps[num++] = -m;

    for (int i = 0; i < num; ++i)
    {
        int k = ps[i];
        int curr = 0;
        for (int i = 0; i < n; ++i)
        {
            if (a[i] <= k && k + m <= b[i]) ++curr;
        }
        cnts[i] = curr;
    }
    
    ans = 0;
    for (int i = 0; i < num; ++i)
    {
        for (int j = 0; j < num; ++j)
        {
            if (ps[i] + m > ps[j]) continue;
            int curr = cnts[i] + cnts[j];
            ans = std::max(ans, curr);
        }
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