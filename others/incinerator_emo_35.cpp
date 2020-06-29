#include <iostream>
#include <iomanip>
#include <math.h>

const int ITERS = 64;
const int SPLITS = 100;
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
double dist(double x1, double y1, double x2, double y2)
{
    return sqrt(sq(x1 - x2) + sq(y1 - y2));
}

// finds the distance to the closest circle adjusted for its radius
double minDist(double x, double y)
{
    double minD = 2 * BORDER;
    for (int i = 0; i < n; ++i)
    {
        double d = dist(x, y, cs[i].x, cs[i].y) - cs[i].r;
        minD = std::min(minD, d);
    }
    return minD;
}

// finds the max min distance for a point lying on a line segment
// works only if the function is concave on the line segment
double solveSmallLine(double x1, double y1, double x2, double y2)
{
    double l = 0;
    double r = 1;
    double dx = x2 - x1;
    double dy = y2 - y1;

    for (int i = 0; i < ITERS; ++i)
    {
        double m1 = (2 * l + r) / 3;
        double m2 = (l + 2 * r) / 3;

        double d1 = minDist(x1 + m1 * dx, y1 + m1 * dy);
        double d2 = minDist(x1 + m2 * dx, y1 + m2 * dy);

        if (d1 < d2) l = m1;
        else r = m2;
    }

    return minDist(x1 + l * dx, y1 + l * dy);
}

// finds the max min distance for a point lying on a line segment
// splits the segment into smaller ones and uses the above function
// we would need split on the projections of all centers onto the line
// however for few points it is fine to just split it up into small intervals
double solveLine(double x1, double y1, double x2, double y2)
{
    double dx = x2 - x1;
    double dy = y2 - y1;
    double best = -2 * BORDER;

    for (int i = 0; i < SPLITS; ++i)
    {
        double m1 = i * 1.0 / SPLITS;
        double m2 = (i + 1) * 1.0 / SPLITS;
        double curr = solveSmallLine(x1 + m1 * dx, y1 + m1 * dy, x1 + m2 * dx, y1 + m2 * dy);
        best = std::max(best, curr);
    }

    return best;
}

// the main solve function
void solve()
{
    ans = 0;
    ans = std::max(ans, solveLine(0, 0, 0, BORDER));
    ans = std::max(ans, solveLine(0, 0, BORDER, 0));
    ans = std::max(ans, solveLine(0, BORDER, BORDER, BORDER));
    ans = std::max(ans, solveLine(BORDER, 0, BORDER, BORDER));
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
