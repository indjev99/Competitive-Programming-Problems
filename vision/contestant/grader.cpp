#include "vision.h"

#include <iostream>
#include <algorithm>
#include <queue>
#include <map>
#include <set>

static const int MAX_NMV = 60;

static void myAssert(bool cond, const char* msg)
{
    if (cond) return;

    std::cout << msg << std::endl;
    exit(0);
}

static int d;
static int n;
static int m;
static std::vector<std::vector<int>> pattern;

static void printVision(const std::vector<std::vector<int>>& v)
{
    for (const auto& row : v)
    {
        for (int i = 0; i < (int) row.size(); i++)
        {
            if (i > 0) std::cout << " ";
            std::cout << row[i];
        }
        std::cout << std::endl;
    }
}

static std::vector<std::vector<int>> getVisionPattern()
{
    if (d == 1) return {getVisionPattern1d()};
    else return getVisionPattern2d();
}

static std::pair<int, int> getMove(const std::vector<std::vector<int>>& v)
{
    if (d == 1) return {0, getMove1d(v[0])};
    else return getMove2d(v);
}

static void validatePattern()
{
    n = pattern.size();

    myAssert(n > 0 && n <= MAX_NMV, "Invalid N");
    if (d == 1) myAssert(n == 1, "Invalid N");

    m = pattern[0].size();

    myAssert(m > 0 && m <= MAX_NMV, "Invalid M");
    for (const auto& row : pattern)
    {
        myAssert((int) row.size() == m, "Invalid rectangle");
    }

    for (const auto& row : pattern)
    {
        for (int val : row)
        {
            myAssert(val > 0 && val <= MAX_NMV, "Invalid vision");
        }
    }
}

struct Move
{
    int s;
    int t;
    int dx;
    int dy;
};

static std::map<std::vector<std::vector<int>>, Move> cache;

static Move cachedQuery(const std::vector<std::vector<int>>& v, int x, int y, int orient)
{
    auto it = cache.find(v);
    if (it != cache.end()) return it->second;

    auto [s, t] = getMove(v);

    int b = v.size();
    int a = v[0].size();
    int r = (a - 1) / 2;

    if (!(s >= 0 && s < b && t >= 0 && t < a))
    {
        std::cout << "Invalid move:" << std::endl;
        if (d == 2) std::cout << "S: " << s << std::endl;
        std::cout << "T: " << t << std::endl;
        if (d == 2) std::cout << "X: " << x << std::endl;
        std::cout << "Y: " << y << std::endl;
        std::cout << "Ori: " << orient << std::endl;
        std::cout << "Scan:" << std::endl;
        printVision(v);
        exit(0);
    }

    int dx = d == 2 ? s - r : 0;
    int dy = t - r;

    Move move = {s, t, dx, dy};

    cache[v] = move;
    return move;
}

static std::vector<std::vector<int>> getSeen(int x, int y)
{
    int r = pattern[x][y];
    int a = 2 * r + 1;

    int b = d == 2 ? a : 1;

    std::vector<std::vector<int>> v(b, std::vector<int>(a));

    for (int i = 0; i < b; i++)
    {
        for (int j = 0; j < a; j++)
        {
            int i2 = d == 2 ? ((x + i - r) % n + n) % n : x;
            int j2 = ((y + j - r) % m + m) % m;

            v[i][j] = pattern[i2][j2];
        }
    }

    return v;
}

static std::vector<Move> getMoves(int x, int y)
{
    std::vector<Move> moves;

    std::vector<std::vector<int>> v = getSeen(x, y);

    {
        auto [s, t, dx, dy] = cachedQuery(v, x, y, 0);
        moves.push_back({s, t, dx, dy});
    }

    {
        for (auto& row : v)
        {
            std::reverse(row.begin(), row.end());
        }

        auto [s, t, dx, dy] = cachedQuery(v, x, y, 1);
        moves.push_back({s, t, dx, -dy});

        for (auto& row : v)
        {
            std::reverse(row.begin(), row.end());
        }
    }

    if (d == 2)
    {
        std::reverse(v.begin(), v.end());

        auto [s, t, dx, dy] = cachedQuery(v, x, y, 2);
        moves.push_back({s, t, -dx, dy});

        std::reverse(v.begin(), v.end());
    }

    if (d == 2)
    {
        for (auto& row : v)
        {
            std::reverse(row.begin(), row.end());
        }

        std::reverse(v.begin(), v.end());

        auto [s, t, dx, dy] = cachedQuery(v, x, y, 3);
        moves.push_back({s, t, -dx, -dy});

        std::reverse(v.begin(), v.end());

        for (auto& row : v)
        {
            std::reverse(row.begin(), row.end());
        }
    }

    return moves;
}

static std::vector<std::vector<std::vector<Move>>> moves;

static void cacheMoves()
{
    moves.resize(n);
    for (int i = 0; i < n; i++)
    {
        moves[i].resize(m);
        for (int j = 0; j < m; j++)
        {
            moves[i][j] = getMoves(i, j);
        }
    }
}

double evalPattern()
{
    int sum = 0;
    for (const auto& row : pattern)
    {
        for (int a : row)
        {
            sum += a;
        }
    }

    return (double) sum / n / m;
}

int main()
{
    std::cout << "D: ";
    std::cin >> d;

    myAssert(d == 1 || d == 2, "Invalid D");

    pattern = getVisionPattern();

    std::cout << "Pattern:" << std::endl;
    printVision(pattern);

    validatePattern();

    if (d == 2) std::cout << "N: " << n << std::endl;
    std::cout << "M: " << m << std::endl;
    std::cout << "Avg vision: " << evalPattern() << std::endl;

    cacheMoves();

    int x;
    int y;

    if (d == 2)
    {
        std::cout << "X0: ";
        std::cin >> x;
    }
    else
    {
        x = 0;
    }

    std::cout << "Y0: ";
    std::cin >> y;

    while (true)
    {
        int x2 = (x % n + n) % n;
        int y2 = (y % m + m) % m;

        if (d == 2) std::cout << "X: " << x << std::endl;
        std::cout << "Y: " << y << std::endl;

        std::vector<std::vector<int>> v = getSeen(x2, y2);

        std::cout << "Base scan:" << std::endl;
        printVision(v);

        int orient;
        std::cout << "Ori: ";
        std::cin >> orient;

        if (d == 1) myAssert(orient >= 0 && orient < 2, "Invalid orientation");
        if (d == 2) myAssert(orient >= 0 && orient < 4, "Invalid orientation");

        if (orient & 1)
        {
            for (auto& row : v)
            {
                std::reverse(row.begin(), row.end());
            }
        }

        if (orient & 2)
        {
            std::reverse(v.begin(), v.end());
        }

        std::cout << "Scan:" << std::endl;
        printVision(v);

        auto [s, t, dx, dy] = moves[x2][y2][orient];

        if (d == 2) std::cout << "S: " << s << std::endl;
        std::cout << "T: " << t << std::endl;

        if (d == 2) std::cout << "DX: " << (dx > 0 ? "+" : "") << dx << std::endl;
        std::cout << "DY: " << (dy > 0 ? "+" : "") << dy << std::endl;

        x += dx;
        y += dy;
    }

    return 0;
}
