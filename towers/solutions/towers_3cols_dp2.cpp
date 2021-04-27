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

const int NUM_STATES = 3;
const int current1[NUM_STATES] = {GRAY, BLACK, WHITE};
const int current2[NUM_STATES] = {BLACK, WHITE, GRAY};

const int SMALL_N = 2e3;
const int MAX_M = SMALL_N + 1;

int m;
int pos2[MAX_M];

bool possible[MAX_M][MAX_M][NUM_STATES];
int lastOther[MAX_M][MAX_M][NUM_STATES];

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
            for (int t = 0; t < NUM_STATES; ++t)
            {
                possible[i][j][t] = false;
                lastOther[i][j][t] = INF;
            }
        }
    }

    possible[0][0][WHITE] = true;
    lastOther[0][0][WHITE] = -INF;

    for (int sum = 0; sum < 2 * m; ++sum)
    {
        for (int i = std::max(0, sum - m + 2); i < std::min(m - 1, sum + 1); ++i)
        {
            int j = sum - i;
            for (int t = 0; t < NUM_STATES; ++t)
            {
                if (!possible[i][j][t]) continue;

                int next = std::max(i, j) + 1;

                if (pos2[next] - pos2[i] >= r[current1[t]])
                {
                    possible[next][j][t] = true;
                    lastOther[next][j][t] = std::min(lastOther[next][j][t], lastOther[i][j][t]);
                }

                if (pos2[next] - pos2[j] >= r[current2[t]])
                {
                    possible[i][next][t] = true;
                    lastOther[i][next][t] = std::min(lastOther[i][next][t], lastOther[i][j][t]);
                }

                if (pos2[next] - lastOther[i][j][t] >= r[t])
                {
                    possible[j][next][current1[t]] = true;
                    lastOther[j][next][current1[t]] = std::min(lastOther[j][next][current1[t]], pos2[i]);

                    possible[next][i][current2[t]] = true;
                    lastOther[next][i][current2[t]] = std::min(lastOther[next][i][current2[t]], pos2[j]);
                }
            }
        }
    }

    for (int i = 0; i < m - 1; ++i)
    {
        for (int j = 0; j < NUM_STATES; ++j)
        {
            if (possible[m - 1][i][j]) return true;
            if (possible[i][m - 1][j]) return true;
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
