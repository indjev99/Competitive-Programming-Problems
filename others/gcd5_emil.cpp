#include "gcd5.h"

#include <iostream>
#include <iterator>
#include <cassert>
#include <vector>
#include <map>
#include <set>

const long long MAX_X = 1e9;

struct PointData
{
    long long x;
    long long y;
};

std::map<long long, PointData> data;
std::set<std::pair<long long, long long>> ans;

void makeQuery(long long x)
{
    if (data.count(x) == 0)
    {
        data[x] = {x, query(x)};
    }
}

std::pair<long long, long long> getLine(PointData p1, PointData p2, bool fake=false)
{
    long long a = (p2.y - p1.y) / (p2.x - p1.x);
    long long b = p1.y - p1.x * a;
    if (!fake) ans.insert({a, b});
    return {a, b};
}

bool onLine(PointData p, long long a, long long b)
{
    return p.x * a + b == p.y;
}

void solveRec(long long x1, long long x2)
{
    assert(x2 > x1);

    auto it1 = data.find(x1);
    auto it0 = std::prev(it1);

    auto it2 = data.find(x2);
    auto it3 = std::next(it2);

    auto p0 = it0->second;
    auto p1 = it1->second;
    auto p2 = it2->second;
    auto p3 = it3->second;

    auto [a1, b1] = getLine(p0, p1);
    auto [a2, b2] = getLine(p2, p3);

    auto xm1 = (b2 - b1) / (a1 - a2);
    if (a2 * xm1 + b2 > a1 * xm1 + b1) xm1--;
    auto xm2 = xm1 + 1;

    makeQuery(xm1);
    makeQuery(xm2);

    auto pm1 = data[xm1];
    auto pm2 = data[xm2];

    bool on1 = onLine(pm1, a1, b1);
    bool on2 = onLine(pm2, a2, b2);

    if (on1 && on2) return;

    if (a1 * xm1 + b1 > a2 * xm1 + b2)
    {
        assert(!onLine(pm1, a2, b2));
    }
    else
    {
        assert(a1 * xm1 + b1 == a2 * xm1 + b2);
    }

    if (a2 * xm2 + b2 > a1 * xm2 + b1)
    {
        assert(!onLine(pm2, a1, b1));
    }
    else
    {
        assert(a2 * xm2 + b2 == a1 * xm2 + b1);
    }

    if (on1 && !on2)
    {
        auto xm3 = xm2 + 1;
        makeQuery(xm3);
        solveRec(xm3, x2);
        return;
    }

    if (!on1 && on2)
    {
        auto xm0 = xm1 - 1;
        makeQuery(xm0);
        solveRec(x1, xm0);
        return;
    }

    auto xm0 = xm1 - 1;
    makeQuery(xm0);
    auto pm0 = data[xm0];

    if (onLine(pm0, a1, b1))
    {
        solveRec(xm2, x2);
        return;
    }

    auto [am2, bm2] = getLine(pm1, pm2, true);

    if (onLine(pm0, am2, bm2))
    {
        solveRec(x1, xm0);
        solveRec(xm2, x2);
        return;
    }

    auto xm3 = xm2 + 1;
    makeQuery(xm3);
    auto pm3 = data[xm3];

    assert(!onLine(pm3, a2, b2));

    if (!onLine(pm3, am2, bm2))
    {
        solveRec(x1, xm0);
        solveRec(xm3, x2);
        return;
    }

    auto xm_1 = xm0 - 1;
    makeQuery(xm_1);
    auto pm_1 = data[xm_1];

    assert(!onLine(pm_1, a2, b2));

    solveRec(x1, xm_1);
    solveRec(xm3, x2);
}

void solve(int n)
{
    long long x0 = -MAX_X;
    long long x2 = MAX_X;

    makeQuery(x0);
    makeQuery(x0 + 1);

    makeQuery(x2 - 1);
    makeQuery(x2);

    solveRec(x0 + 1, x2 - 1);

    for (auto [a, b] : ans)
    {
        answer(a, b);
    }
}
