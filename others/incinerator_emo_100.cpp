#include <iostream>
#include <iomanip>
#include <math.h>

const int ITERS = 64;
const double EPS = 1e-10;

struct Circle
{
    double x, y, r;
};

const int MAX_N = 100;
const double BORDER = 1000;

int n;
Circle cs[MAX_N];
double ans;

void input()
{
    std::cin >> n;
    for (int i = 0; i < n; ++i)
    {
        std::cin >> cs[i].x >> cs[i].y >> cs[i].r;
    }
}

void output()
{
    std::cout << std::fixed;
    std::cout << std::setprecision(9);
    std::cout << ans << "\n";
}

// square of number
double sq(double x)
{
    return x * x;
}

// squared distance between points
double sqDist(double x1, double y1, double x2, double y2)
{
    return sq(x1 - x2) + sq(y1 - y2);
}

// checks if point is in circle
bool isIn(double x, double y, const Circle& c, double r2)
{
    return sqDist(x, y, c.x, c.y) < sq(c.r + r2) - EPS;
}

// solves quadratic equation
bool solveQuad(double a, double b, double c, double& x1, double& x2)
{
    double d = sq(b)- 4 * a * c;
    if (d < 0) return false;
    d = sqrt(d);
    x1 = (-b + d) / (2 * a);
    x2 = (-b - d) / (2 * a);
    return true;
}

// checks if point is a solution
bool checkPoint(double x, double y, double r2)
{
    if (x < 0 || x > BORDER || y < 0 || y > BORDER) return false;

    for (int i = 0; i < n; ++i)
    {
        if (isIn(x, y, cs[i], r2)) return false;
    }

    return true;
}

// intersects borders with circle and checks points
bool interBC(const Circle& c, double r2)
{
    double rSq = sq(c.r + r2);

    bool bx1, bx2;
    double x11, x12, x21, x22;

    bx1 = solveQuad(1, -2 * c.x, sq(c.x) + sq(c.y) - rSq, x11, x12);
    bx2 = solveQuad(1, -2 * c.x, sq(c.x) + sq(BORDER) - 2 * BORDER * c.y + sq(c.y) - rSq, x21, x22);

    bool by1, by2;
    double y11, y12, y21, y22;

    by1 = solveQuad(1, -2 * c.y, sq(c.y) + sq(c.x) - rSq, y11, y12);
    by2 = solveQuad(1, -2 * c.y, sq(c.y) + sq(BORDER) - 2 * BORDER * c.x + sq(c.x) - rSq, y21, y22);

    if (bx1 && checkPoint(x11, 0, r2)) return true;
    if (bx1 && checkPoint(x12, 0, r2)) return true;
    if (bx2 && checkPoint(x21, BORDER, r2)) return true;
    if (bx2 && checkPoint(x22, BORDER, r2)) return true;
    if (by1 && checkPoint(0, y11, r2)) return true;
    if (by1 && checkPoint(0, y12, r2)) return true;
    if (by2 && checkPoint(BORDER, y21, r2)) return true;
    if (by2 && checkPoint(BORDER, y22, r2)) return true;

    return false;
}

// intersects circle with circle and checks points
bool interCC(const Circle& c1, const Circle& c2, double r2)
{
    double dSq = sqDist(c1.x, c1.y, c2.x, c2.y);
    double rSq = sq(c1.r + c2.r + 2 * r2);

    if (dSq > rSq) return false;

    double d = sqrt(dSq);

    double a = (sq(c1.r + r2) - sq(c2.r + r2) + dSq) / (2 * d);
    double h = sqrt(sq(c1.r + r2) - sq(a));

    double x3 = c1.x + a * (c2.x - c1.x) / d;
    double y3 = c1.y + a * (c2.y - c1.y) / d;

    double x41 = x3 + h * (c2.y - c1.y) / d;
    double x42 = x3 - h * (c2.y - c1.y) / d;
    double y41 = y3 - h * (c2.x - c1.x) / d;
    double y42 = y3 + h * (c2.x - c1.x) / d;

    if (checkPoint(x41, y41, r2)) return true;
    if (checkPoint(x42, y42, r2)) return true;

    return false;
}

// checks if there is a solution with r2
bool check(double r2)
{
    //corners
    if (checkPoint(0, 0, r2)) return true;
    if (checkPoint(0, BORDER, r2)) return true;
    if (checkPoint(BORDER, 0, r2)) return true;
    if (checkPoint(BORDER, BORDER, r2)) return true;

    //circle with border
    for (int i = 0; i < n; ++i)
    {
        if (interBC(cs[i], r2)) return true;
    }

    //circle with circle
    for (int i = 0; i < n; ++i)
    {
        for (int j = i + 1; j < n; ++j)
        {
           if (interCC(cs[i], cs[j], r2)) return true;
        }
    }

    return false;
}

// the main solve function
void solve()
{
    double l = 0;
    double r = 2 * BORDER;

    for (int i = 0; i < ITERS; ++i)
    {
        double m = (l + r) / 2;
        if (check(m)) l = m;
        else r = m;
    }

    ans = l;
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
