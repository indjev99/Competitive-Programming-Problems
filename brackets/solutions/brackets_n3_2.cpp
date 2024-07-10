#include <iostream>

const int MAX_N = 4e6;

int n;
int vals[MAX_N];

long long ans;

void input()
{
    std::string s;

    std::cin >> n;
    std::cin >> s;

    for (int i = 0; i < n; ++i)
    {
        vals[i] = s[i] == '(' ? 1 : -1;
    }
}

void output()
{
    std::cout << ans << std::endl;
}

int prefSums[MAX_N + 1];

void solve()
{
    ans = 0;

    prefSums[0] = 0;
    for (int i = 0; i <= n; ++i)
    {
        prefSums[i] = vals[i - 1] + prefSums[i - 1];
    }

    for (int l = n - 1; l >= 0; --l)
    {
        for (int r = l + 1; r <= n; ++r)
        {
            int maxPrefSum = 0;
            for (int i = l + 1; i <= r; ++i)
            {
                maxPrefSum = std::max(prefSums[i], maxPrefSum);
            }

            int minSuffSum = prefSums[r] - maxPrefSum;
            if (prefSums[l] + minSuffSum >= 0) ++ans;
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
