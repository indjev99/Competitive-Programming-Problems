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

int possibleBruteforce(int idx, int iters, int whitePos, int firstGray, int firstBlack)
{
    if (idx < 0) return true;

    if (whitePos - pos2[idx] >= r[WHITE])
    {
        for (int j = 0; j < NUM_STATES; ++j)
        {
            if (possible[idx][j] && firstGray - last[idx][j][GRAY - 1] >= r[GRAY]
                && firstBlack - last[idx][j][BLACK - 1] >= r[BLACK]) return true;
        }
    }

    if (iters == 0) return false;

    if (firstGray - pos2[idx] >= r[GRAY]
        && possibleBruteforce(idx - 1, iters - 1, whitePos, pos2[idx], firstBlack))
        return true;

    if (firstBlack - pos2[idx] >= r[BLACK]
        && possibleBruteforce(idx - 1, iters - 1, whitePos, firstGray, pos2[idx]))
        return true;

    return false;
}

int lastBruteforce(int target, int idx, int iters, int whitePos, int firstOther)
{
    if (idx < 0) return -INF;

    int ans = INF;
    int other = target == GRAY ? BLACK : GRAY;

    if (whitePos - pos2[idx] >= r[WHITE])
    {
        for (int j = 0; j < NUM_STATES; ++j)
        {
            if (possible[idx][j] && firstOther - last[idx][j][other - 1] >= r[other])
                ans = std::min(ans, last[idx][j][target - 1]);
        }
    }

    if (iters == 0) return INF;

    if (firstOther - pos2[idx] >= r[other])
        ans = std::min(ans, lastBruteforce(target, idx - 1, iters - 1, whitePos, pos2[idx]));

    if (ans < INF) return ans;

    if ((target == GRAY && possibleBruteforce(idx - 1, iters - 1, whitePos, pos2[idx], firstOther))
        || (target == BLACK && possibleBruteforce(idx - 1, iters - 1, whitePos, firstOther, pos2[idx])))
        return pos2[idx];

    return INF;
}

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

    for (int i = 1; i < m; ++i)
    {
        int lastGray = lastBruteforce(GRAY, i - 2, MAX_BETWEEN_WHITE - 1, pos2[i], pos2[i - 1]);
        int lastBlack = lastBruteforce(BLACK, i - 2, MAX_BETWEEN_WHITE - 1, pos2[i], pos2[i - 1]);

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
