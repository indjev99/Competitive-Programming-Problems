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

const int MAX_M = MAX_N + 1;

int m;
int pos2[MAX_M];

bool curr;
bool possible[2][MAX_M][NUM_STATES];

bool check()
{
    m = 0;
    pos2[m++] = -INF;
    for (int i = 0; i < n; ++i)
    {
        pos2[m++] = pos[i];
    }

    possible[curr][0][WHITE] = true;
    possible[curr][0][GRAY] = false;

    for (int max = 0; max < m - 1; ++max)
    {
        for (int i = 0; i <= max + 1; ++i)
        {
            for (int j = 0; j < NUM_STATES; ++j)
            {
                possible[!curr][i][j] = false;
            }
        }

        for (int i = 0; i <= max; ++i)
        {
            for (int j = 0; j < NUM_STATES; ++j)
            {
                if (!possible[curr][i][j]) continue;

                if (pos2[max + 1] - pos2[max] >= r[j]) possible[!curr][i][j] = true;
                if (pos2[max + 1] - pos2[i] >= r[other[j]]) possible[!curr][max][other[j]] = true;
            }
        }

        curr = !curr;
    }

    for (int i = 0; i < m - 1; ++i)
    {
        for (int j = 0; j < NUM_STATES; ++j)
        {
            if (possible[curr][i][j]) return true;
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
