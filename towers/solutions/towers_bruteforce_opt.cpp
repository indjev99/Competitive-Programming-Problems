#include <iostream>
#include <algorithm>

#define WHITE 0
#define GRAY 1
#define BLACK 2

const int INF = 1e9;

const int MAX_N = 5e5;
const int MAX_K = 3;

int n, k;
int r[MAX_K];
int pos[MAX_N];

bool ans;

void input()
{
    std::cin >> n >> k;
    for (int i = 0; i < k; ++i)
    {
        std::cin >> r[i];
    }
    for (int i = 0; i < n; ++i)
    {
        std::cin >> pos[i];
    }
}

void output()
{
    std::cout << ans << std::endl;
}

int last[MAX_K] = {-INF, -INF, -INF};

bool bruteforceCheck(int idx)
{
    for (int j = 0; j < k; ++j)
    {
        if (pos[idx] - last[j] >= r[j])
        {
            int prevLast = last[j];
            last[j] = pos[idx];
            bool succ = idx == n - 1 || pos[idx + 1] - pos[idx] >= r[k - 1] || bruteforceCheck(idx + 1);
            last[j] = prevLast;
            if (succ) return true;
        }
    }
    return false;
}

bool check()
{
    for (int i = 0; i < n; ++i)
    {
        if ((i == 0 || pos[i] - pos[i - 1] >= r[k - 1]) && !bruteforceCheck(i)) return false;
    }
    return true;
}

void solve()
{
    std::sort(r, r + k);
    ans = check();
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int t;
    std::cin >> t;
    for (int i = 0; i < t; ++i)
    {
        input();
        solve();
        output();
    }

    return 0;
}
