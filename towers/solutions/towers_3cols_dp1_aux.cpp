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

#define GW 0
#define BW 1
#define GWW 2
#define BWW 3

const int NUM_STATES = 4;
const int MAX_BETWEEN_WHITE = 4;

const int MAX_M = MAX_N + 2;

int m;
int pos2[MAX_M];

bool possible[MAX_M][NUM_STATES];
int last[MAX_M][NUM_STATES][MAX_K - 1];
int lastOtherAux[MAX_M][MAX_BETWEEN_WHITE][MAX_K - 1];

bool check()
{
    m = 0;
    for (int i = 0; i < n; ++i)
    {
        if ((i > 0 && pos[i] - pos[i - 1] < r[WHITE])
            || (i < n - 1 && pos[i + 1] - pos[i] < r[WHITE]))
            pos2[m++] = pos[i];
    }
    pos2[m++] = INF;

    possible[0][GW] = true;
    last[0][GW][GRAY - 1] = -INF;
    last[0][GW][BLACK - 1] = -INF;

    possible[0][BW] = false;
    possible[0][GWW] = false;
    possible[0][BWW] = false;

    lastOtherAux[0][0][GRAY - 1] = -INF;
    lastOtherAux[0][0][BLACK - 1] = -INF;

    for (int i = 1; i < MAX_BETWEEN_WHITE; ++i)
    {
        lastOtherAux[0][i][GRAY - 1] = INF;
        lastOtherAux[0][i][BLACK - 1] = INF;
    }

    for (int i = 1; i < m; ++i)
    {
        lastOtherAux[i][0][GRAY - 1] = INF;
        lastOtherAux[i][0][BLACK - 1] = INF;

        for (int j = 0; j < NUM_STATES; ++j)
        {
            if (!possible[i - 1][j]) continue;

            int lastGray = last[i - 1][j][GRAY - 1];
            int lastBlack = last[i - 1][j][BLACK - 1];

            if (pos2[i] - lastGray >= r[GRAY])
                lastOtherAux[i][0][GRAY - 1] = std::min(lastOtherAux[i][0][GRAY - 1], lastBlack);

            if (pos2[i] - lastBlack >= r[BLACK])
                lastOtherAux[i][0][BLACK - 1] = std::min(lastOtherAux[i][0][BLACK - 1], lastGray);
        }

        for (int j = 1; j < MAX_BETWEEN_WHITE; ++j)
        {
            lastOtherAux[i][j][GRAY - 1] = INF;
            lastOtherAux[i][j][BLACK - 1] = INF;

            int lastGray = lastOtherAux[i - 1][j - 1][BLACK - 1];
            int lastBlack = lastOtherAux[i - 1][j - 1][GRAY - 1];

            if (pos2[i] - pos2[i - 1] >= r[GRAY])
                lastOtherAux[i][j][GRAY - 1] = std::min(lastOtherAux[i][j][GRAY - 1], lastBlack);

            if (pos2[i] - lastGray >= r[GRAY])
                lastOtherAux[i][j][GRAY - 1] = std::min(lastOtherAux[i][j][GRAY - 1], pos2[i - 1]);

            if (pos2[i] - pos2[i - 1] >= r[BLACK])
                lastOtherAux[i][j][BLACK - 1] = std::min(lastOtherAux[i][j][BLACK - 1], lastGray);

            if (pos2[i] - lastBlack >= r[BLACK])
                lastOtherAux[i][j][BLACK - 1] = std::min(lastOtherAux[i][j][BLACK - 1], pos2[i - 1]);
        }

        int lastGray = INF;
        int lastBlack = INF;

        for (int j = 0; j < MAX_BETWEEN_WHITE; ++j)
        {
            if (i - j > 1 && pos2[i] - pos2[i - j - 2] < r[WHITE]) continue;

            lastGray = std::min(lastGray, lastOtherAux[i - 1][j][BLACK - 1]);
            lastBlack = std::min(lastBlack, lastOtherAux[i - 1][j][GRAY - 1]);
        }

        possible[i][GW] = lastBlack < INF;
        last[i][GW][GRAY - 1] = pos2[i - 1];
        last[i][GW][BLACK - 1] = lastBlack;

        possible[i][BW] = lastGray < INF;
        last[i][BW][GRAY - 1] = lastGray;
        last[i][BW][BLACK - 1] = pos2[i - 1];

        if (pos2[i] - pos2[i - 1] >= r[WHITE])
        {
            possible[i][GWW] = possible[i - 1][GW];
            last[i][GWW][GRAY - 1] = last[i - 1][GW][GRAY - 1];
            last[i][GWW][BLACK - 1] = last[i - 1][GW][BLACK - 1];

            possible[i][BWW] = possible[i - 1][BW];
            last[i][BWW][GRAY - 1] = last[i - 1][BW][GRAY - 1];
            last[i][BWW][BLACK - 1] = last[i - 1][BW][BLACK - 1];
        }
        else
        {
            possible[i][GWW] = false;
            possible[i][BWW] = false;
        }
    }

    for (int i = 0; i < NUM_STATES; ++i)
    {
        if (possible[m - 2][i]) return true;
        if (possible[m - 1][i]) return true;
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
