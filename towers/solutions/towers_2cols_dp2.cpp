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

const int SMALL_N = 5e3;
const int MAX_M = SMALL_N + 1;

int m;
int pos2[MAX_M];

bool possible[MAX_M][MAX_M];

bool check()
{
    m = 0;
    pos2[m++] = -INF;
    for (int i = 0; i < n; ++i)
    {
        pos2[m++] = pos[i];
    }

    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            possible[i][j] = false;
        }
    }

    possible[0][0] = true;

    for (int sum = 0; sum < 2 * m; ++sum)
    {
        for (int i = std::max(0, sum - m + 2); i < std::min(m - 1, sum + 1); ++i)
        {
            int j = sum - i;
            if (!possible[i][j]) continue;

            int next = std::max(i, j) + 1;
            if (pos2[next] - pos2[i] >= r[WHITE]) possible[next][j] = true;
            if (pos2[next] - pos2[j] >= r[GRAY]) possible[i][next] = true;
        }
    }

    for (int i = 0; i < m - 1; ++i)
    {
        if (possible[m - 1][i]) return true;
        if (possible[i][m - 1]) return true;
    }
    return false;
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
