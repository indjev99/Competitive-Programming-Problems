#include <iostream>

const int MAX_N = 1e6;
const int MOD = 1e9 + 7;

int n;
int q[MAX_N];

int p[MAX_N];
int occurs[MAX_N];
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

void solve()
{
	for (int i = 0; i < n; ++i)
	{
		int& curr = occurs[q[i] - 1];
		if (curr == 1 && (i < 2 || q[i] != q[i - 2])) return;
		if (curr == 2) return;
		++curr;
	}

    for (int i = 1; i < n && (i < 2 || q[i - 1] != p[i - 2]); i += 2)
    {
        p[i] = q[i - 1];
    }

	if (p[n - 2] != 0 && p[n - 2] != q[n - 1]) return;

    for (int i = n - 2; i >= 0 && (i >= n - 2 || q[i + 1] != p[i + 2]); i -= 2)
    {
        p[i] = q[i + 1];
    }

    for (int i = 1; i < n - 1; ++i)
    {
        if (q[i - 1] == q[i + 1]) p[i] = q[i - 1];
    }

    int gaps = 0;
    int gapSize[2] = {0, 0};

    ans = 1;

    for (int i = 0; i < n; ++i)
    {
        if (p[i] == 0)
        {
            if (gapSize[i % 2] == 0)
            {
                ++gaps;
                ans = ans * gaps % MOD;
            }
            ++gapSize[i % 2];
        }
        else
        {
            if (gapSize[i % 2] != 0) ans = ans * gapSize[i % 2] % MOD;
            gapSize[i % 2] = 0;
        }
    }

    if (gapSize[0] != 0) ans = ans * gapSize[0] % MOD;
    if (gapSize[1] != 0) ans = ans * gapSize[1] % MOD;
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
