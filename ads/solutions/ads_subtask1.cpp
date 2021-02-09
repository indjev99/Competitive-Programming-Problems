#include <iostream>

const int MAX_N = 1e6;

int n, m;
int a[MAX_N], b[MAX_N];

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
    int last = 0;
    for (int i = 0; i < n; ++i)
    {
        last = std::max(last, b[i]);
    }
    
    ans = 0;
    for (int k1 = -m; k1 <= last; ++k1)
    {
        for (int k2 = k1 + m; k2 <= last; ++k2)
        {
            int curr = 0;
            for (int i = 0; i < n; ++i)
            {
                if (a[i] <= k1 && k1 + m <= b[i]) ++curr;
                if (a[i] <= k2 && k2 + m <= b[i]) ++curr;
            }
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