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

int col[MAX_N];

bool checkCurr()
{
    int last[MAX_K] = {-INF, -INF, -INF};
    for (int i = 0; i < n; ++i)
    {
        if (pos[i] - last[col[i]] < r[col[i]]) return false;
        last[col[i]] = pos[i];
    }
    return true;
}

bool bruteforceCheck(int idx)
{
    if (idx == n) return checkCurr();
    for (int j = 0; j < k; ++j)
    {
        col[idx] = j;
        if (bruteforceCheck(idx + 1)) return true;
    }
    return false;
}

void solve()
{
    std::sort(r, r + k);
    ans = bruteforceCheck(0);
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
