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

#define FIRST 0
#define SECOND 1

const int NUM_STATES = 3;
const int NUM_STATES2 = 2;
const int current[NUM_STATES][NUM_STATES2] = {
    {GRAY, BLACK},
    {BLACK, WHITE},
    {WHITE, GRAY}
};

const int MAX_M = MAX_N + 1;

int m;
int pos2[MAX_M];

bool curr;
bool possible[2][MAX_M][NUM_STATES][NUM_STATES2];
int lastOther[2][MAX_M][NUM_STATES][NUM_STATES2];

bool check()
{
    m = 0;
    pos2[m++] = -INF;
    for (int i = 0; i < n; ++i)
    {
        pos2[m++] = pos[i];
    }

    possible[curr][0][WHITE][FIRST] = true;
    lastOther[curr][0][WHITE][FIRST] = -INF;

    possible[curr][0][WHITE][SECOND] = false;
    possible[curr][0][GRAY][FIRST] = false;
    possible[curr][0][GRAY][SECOND] = false;
    possible[curr][0][BLACK][FIRST] = false;
    possible[curr][0][BLACK][SECOND] = false;

    for (int max = 0; max < m - 1; ++max)
    {
        for (int i = 0; i <= max + 1; ++i)
        {
            for (int jt = 0; jt < NUM_STATES * NUM_STATES2; ++jt)
            {
                int j = jt / NUM_STATES2;
                int t = jt % NUM_STATES2;

                possible[!curr][i][j][t] = false;
                lastOther[!curr][i][j][t] = INF;
            }
        }

        for (int i = 0; i <= max; ++i)
        {
            for (int jt = 0; jt < NUM_STATES * NUM_STATES2; ++jt)
            {
                int j = jt / NUM_STATES2;
                int t = jt % NUM_STATES2;

                if (!possible[curr][i][j][t]) continue;

                if (pos2[max + 1] - pos2[max] >= r[current[j][t]])
                {
                    possible[!curr][i][j][t] = true;
                    lastOther[!curr][i][j][t] = std::min(lastOther[!curr][i][j][t], lastOther[curr][i][j][t]);
                }

                if (pos2[max + 1] - pos2[i] >= r[current[j][!t]])
                {
                    possible[!curr][max][j][!t] = true;
                    lastOther[!curr][max][j][!t] = std::min(lastOther[!curr][max][j][!t], lastOther[curr][i][j][t]);
                }

                if (pos2[max + 1] - lastOther[curr][i][j][t] >= r[j])
                {
                    possible[!curr][i][current[j][t]][!t] = true;
                    lastOther[!curr][i][current[j][t]][!t] = std::min(lastOther[!curr][i][current[j][t]][!t], pos2[max]);

                    possible[!curr][max][current[j][!t]][t] = true;
                    lastOther[!curr][max][current[j][!t]][t] = std::min(lastOther[!curr][max][current[j][!t]][t], pos2[i]);
                }
            }
        }

        curr = !curr;
    }

    for (int i = 0; i < m - 1; ++i)
    {
        for (int jt = 0; jt < NUM_STATES * NUM_STATES2; ++jt)
        {
            int j = jt / NUM_STATES2;
            int t = jt % NUM_STATES2;

            if (possible[curr][i][j][t]) return true;
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
