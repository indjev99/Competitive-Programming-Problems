#include <iostream>
#include <algorithm>

const int MAX_N = 1e5;

struct Point
{
    int x;
    int y;
    int bot;
    int top;
};

int n;
int w, h;
Point points[MAX_N];
int ans;

void input()
{
    std::cin >> n >> w >> h;
    for (int i = 0; i < n; ++i)
    {
        std::cin >> points[i].x >> points[i].y;
    }
}

void output()
{
    std::cout << ans << std::endl;
}

int tree[MAX_N * 4];
int lazy[MAX_N * 4];

inline void addAtNode(int curr, int cl, int cr, int val)
{
    tree[curr] += val;
    if (cr > cl)
    {
        lazy[curr * 2 + 1] += val;
        lazy[curr * 2 + 2] += val;
    }
}

void updateUtil(int curr, int cl, int cr, int ul, int ur, int val)
{
    addAtNode(curr, cl, cr, lazy[curr]);
    lazy[curr] = 0;

    if (ul > cr || ur < cl) return;
    
    if (ul <= cl && cr <= ur)
    {
        addAtNode(curr, cl, cr, val);
        return;
    }

    int cm = (cl + cr) / 2;
    updateUtil(curr * 2 + 1, cl, cm, ul, ur, val);
    updateUtil(curr * 2 + 2, cm + 1, cr, ul, ur, val);
    tree[curr] = std::max(tree[curr * 2 + 1], tree[curr * 2 + 2]);
}

void update(int idx, int val)
{
    int bot = points[idx].bot;
    int top = points[idx].top;

    updateUtil(0, 0, MAX_N - 1, bot, top, val);
}

int currMax()
{
    return tree[0];
}

bool cmpY(const Point& a, const Point& b)
{
    return a.y < b.y;
}

bool cmpX(const Point& a, const Point& b)
{
    return a.x < b.x;
}

void findBotTops()
{
    std::sort(points, points + n, cmpY);

    int bot = 0;
    int top = 0;

    while (top < n)
    {
        if (top > 0 && points[top].y == points[top - 1].y)
        {
            points[top].bot = points[top - 1].bot;
        }
        else points[top].bot = top;

        while (bot < top && points[top].y - points[bot].y > h)
        {
            points[bot].top = top - 1;
            ++bot;
        }
        ++top;
    }

    while (bot < n)
    {
        points[bot].top = n - 1;
        ++bot;
    }
}

void slideX()
{
    std::sort(points, points + n, cmpX);

    int left = 0;
    int right = 0;

    while (right < n)
    {
        update(right, 1);
        while (left < right && points[right].x - points[left].x > w)
        {
            update(left, -1);
            ++left;
        }
        ans = std::max(ans, currMax());
        ++right;
    }
}

void solve()
{
    findBotTops();
    slideX();
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