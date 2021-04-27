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

const int SMALL_N = 5e2;
const int MAX_M = SMALL_N + 1;

int m;
int pos2[MAX_M];

bool possible[MAX_M][MAX_M][MAX_M];

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
            for (int t = 0; t < m; ++t)
            {
                possible[i][j][t] = false;
            }
        }
    }

    possible[0][0][0] = true;

    for (int sum = 0; sum < 3 * n; ++sum)
    {
        for (int i = 0; i < m - 1; ++i)
        {
            for (int j = std::max(0, sum - i - m + 2); j < std::min(m - 1, sum - i + 1); ++j)
            {
                int t = sum - i - j;
                if (!possible[i][j][t]) continue;

                int next = std::max(i, std::max(j, t)) + 1;
                if (pos2[next] - pos2[i] >= r[WHITE]) possible[next][j][t] = true;
                if (pos2[next] - pos2[j] >= r[GRAY]) possible[i][next][t] = true;
                if (pos2[next] - pos2[t] >= r[BLACK]) possible[i][j][next] = true;
            }
        }
    }

    for (int i = 0; i < m - 1; ++i)
    {
        for (int j = 0; j < m - 1; ++j)
        {
            if (possible[m - 1][i][j]) return true;
            if (possible[i][m - 1][j]) return true;
            if (possible[i][j][m - 1]) return true;
        }
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
