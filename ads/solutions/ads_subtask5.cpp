#include <iostream>
#include <algorithm>

const int MAX_N = 1e6;

struct Point
{
    int pos;
    int val;
};

bool operator<(const Point& p1, const Point& p2)
{
    return p1.pos < p2.pos;
}

int n, m;
int a[MAX_N], b[MAX_N];
Point ps[MAX_N * 2];
Point psCnts[MAX_N * 2];
int suffMaxs[MAX_N * 2];

int ans;

void input()
{
    std::cin >> n >> m;
    for (int i = 0; i < n; ++i)
    {
        std::cin >> a[i] >> b[i];
    }
}

void output()
{
    std::cout << ans << std::endl;
}

void solve()
{
    int num = 0;
    for (int i = 0; i < n; ++i)
    {
        if (a[i] + m > b[i]) continue;
        ps[num++] = {a[i], 1};
        ps[num++] = {b[i] - m, -1};
    }
    ps[num++] = {-m, 0};

    std::sort(ps, ps + num);

    int num2 = 0;
    int cnt = 0;
    int nextCnt = 0;
    for (int i = 0; i < num; ++i)
    {
        if (ps[i].val == 1) ++cnt;
        else if (ps[i].val == -1) --nextCnt;
        if (i == num - 1 || ps[i].pos < ps[i + 1].pos)
        {
            psCnts[num2++] = {ps[i].pos, cnt};
            cnt += nextCnt;
            nextCnt = 0;
        }
    }

    int suffMax = 0;
    for (int i = num2 - 1; i >= 0; --i)
    {
        suffMax = std::max(suffMax, psCnts[i].val);
        suffMaxs[i] = suffMax;
    }
    
    ans = 0;
    int j = 0;
    for (int i = 0; i < num2; ++i)
    {
        while (j < num2 && psCnts[j].pos < psCnts[i].pos + m) ++j;
        if (j == num2) break;
        int curr = psCnts[i].val + suffMaxs[j];
        ans = std::max(ans, curr);
    }
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    input();
    solve();
    output();

    return 0;
}