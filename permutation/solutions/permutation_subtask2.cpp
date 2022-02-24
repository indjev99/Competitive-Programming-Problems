#include <iostream>
#include <algorithm>

const int MAX_N = 1e6;
const int MOD = 1e9 + 7;

int n;
int q[MAX_N];

int p[MAX_N];
bool occurs[MAX_N];
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

bool isValid(int mask)
{
    std::fill(p, p + n, 0);
    std::fill(occurs, occurs + n, false);

    for (int i = 0; i < n; ++i)
    {
        bool bit = (mask >> i) & 1;

        if (i == 0  && !bit) return false;
        if (i == n - 1 && bit) return false;

        int offset = (bit ? 1 : -1);

        if (p[i + offset] != 0 && p[i + offset] != q[i]) return false;
        if (p[i + offset] == 0 && occurs[q[i] - 1]) return false;

        p[i + offset] = q[i];
        occurs[q[i] - 1] = true;
    }

    return true;
}

void solve()
{
    int unseen = n;
    for (int i = 0; i < n; ++i)
    {
        if (i < 2 || q[i] != q[i - 2]) --unseen;
    }

    long long unseenFact = 1;
    for (int i = 0; i < unseen; ++i)
    {
        unseenFact = unseenFact * (i + 1) % MOD;
    }

    for (int mask = 0; mask < 1 << n; ++mask)
    {
        if (isValid(mask)) ans = (ans + unseenFact) % MOD;
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
