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

bool check()
{
    double last[MAX_K] = {-INF, -INF, -INF};
    for (int i = 0; i < n; ++i)
    {
        int currCol = -1;
        for (int j = 0; j < k; ++j)
        {
            if (pos[i] - last[j] >= r[j])
            {
                currCol = j;
                break;
            }
        }
        if (currCol == -1) return false;
        last[currCol] = pos[i];
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
