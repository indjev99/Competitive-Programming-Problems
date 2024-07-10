#include <iostream>
#include <algorithm>

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

int valsTmp[MAX_N];

void solve()
{
    ans = 0;

    for (int l = 0; l < n; ++l)
    {
        for (int r = l + 1; r <= n; ++r)
        {
            std::copy(vals, vals + n, valsTmp);
            std::reverse(valsTmp + l, valsTmp + r);

            int sum = 0;
            bool valid = true;
            for (int i = 0; i < n; ++i)
            {
                sum += valsTmp[i];
                if (sum < 0)
                {
                    valid = false;
                    break;
                }
            }

            if (valid) ++ans;
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
