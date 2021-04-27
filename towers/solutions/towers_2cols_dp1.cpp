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

const int NUM_STATES = 2;
const int other[NUM_STATES] = {GRAY, WHITE};

bool possible[MAX_N][NUM_STATES];
int lastOther[MAX_N][NUM_STATES];

bool check()
{
    for (int i = 0; i < NUM_STATES; ++i)
    {
        possible[0][i] = true;
        lastOther[0][i] = -INF;
    }

    for (int i = 1; i < n; ++i)
    {
        for (int j = 0; j < NUM_STATES; ++j)
        {
            possible[i][j] = false;
            lastOther[i][j] = INF;

            if (possible[i - 1][j] && pos[i] - pos[i - 1] >= r[j])
            {
                possible[i][j] = true;
                lastOther[i][j] = std::min(lastOther[i][j], lastOther[i - 1][j]);
            }

            if (possible[i - 1][other[j]] && pos[i] - lastOther[i - 1][other[j]] >= r[j])
            {
                possible[i][j] = true;
                lastOther[i][j] = std::min(lastOther[i][j], pos[i - 1]);
            }
        }
    }

    for (int i = 0; i < NUM_STATES; ++i)
    {
        if (possible[n - 1][i]) return true;
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
