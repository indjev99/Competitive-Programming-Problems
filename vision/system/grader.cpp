#include "vision.h"

#include <iostream>
#include <algorithm>
#include <queue>
#include <map>
#include <set>
#include <fstream>
#include <chrono>

static std::ofstream graderTimeFile;

static std::chrono::steady_clock::time_point startT;
static std::chrono::steady_clock::time_point endT;

static bool timing;
static double totalGraderTime = 0;

static void startTime()
{
    startT = std::chrono::steady_clock::now();

    timing = true;
}

static void writeTime()
{
    graderTimeFile << "io-time: " << totalGraderTime << std::endl;
}

static void endTime(bool write = true, bool dontEnd = false)
{
    if (!timing) return;

    endT = std::chrono::steady_clock::now();

    double time = std::chrono::duration_cast<std::chrono::duration<double>>(endT - startT).count();

    totalGraderTime += time;

    if (write)
    {
        writeTime();
    }

    if (dontEnd)
    {
        startT = endT;
    }
    else
    {
        timing = false;
    }
}

static void logTime()
{
    endTime(true, true);
}

static const std::string PASSWORD = "sh339fj348d-2hc2hcw";

static void myAssert(bool cond, const char* msg)
{
    if (cond) return;

    std::cout << PASSWORD << std::endl;
    std::cout << -1 << std::endl;
    std::cout << msg << std::endl;
    std::cout << totalGraderTime << std::endl;
    exit(0);
}

static const int MAX_NMV = 60;

static int d;
static int n;
static int m;
static std::vector<std::vector<int>> pattern;

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

static std::map<std::vector<std::vector<int>>, std::pair<int, int>> cache;

static std::pair<int, int> cachedQuery(const std::vector<std::vector<int>>& v)
{
    auto it = cache.find(v);
    if (it != cache.end()) return it->second;

    endTime();

    auto [i, j] = getMove(v);

    startTime();

    int b = v.size();
    int a = v[0].size();
    int r = (a - 1) / 2;

    myAssert(i >= 0 && i < b && j >= 0 && j < a, "Invalid move");

    int dx = d == 2 ? i - r : 0;
    int dy = j - r;

    std::pair<int, int> move = {dx, dy};

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

struct Edge
{
    int t;
    int dc[2];
};

static int getId(int x, int y)
{
    return x * m + y;
}

static Edge toEdge(int x, int y, int dx, int dy)
{
    int tx = ((x + dx) % n + n) % n;
    int ty = ((y + dy) % m + m) % m;
    int t = getId(tx, ty);

    int dcx = (x + dx - tx) / n;
    int dcy = (y + dy - ty) / m;

    return {t, {dcx, dcy}};
}

static std::vector<Edge> getEdges(int x, int y)
{
    std::vector<Edge> edges;

    std::vector<std::vector<int>> v = getSeen(x, y);

    std::set<std::pair<int, int>> moves;

    {
        auto [dx, dy] = cachedQuery(v);
        moves.insert({dx, dy});
    }

    if (d == 2)
    {
        std::reverse(v.begin(), v.end());

        auto [dx, dy] = cachedQuery(v);
        moves.insert({-dx, dy});

        std::reverse(v.begin(), v.end());
    }

    for (auto& row : v)
    {
        std::reverse(row.begin(), row.end());
    }

    {
        auto [dx, dy] = cachedQuery(v);
        moves.insert({dx, -dy});
    }

    if (d == 2)
    {
        std::reverse(v.begin(), v.end());

        auto [dx, dy] = cachedQuery(v);
        moves.insert({-dx, -dy});

        std::reverse(v.begin(), v.end());
    }

    for (auto [dx, dy] : moves)
    {
        edges.push_back(toEdge(x, y, dx, dy));
    }

    return edges;
}

static std::vector<std::vector<Edge>> edges;

static void buildGraph()
{
    edges.resize(n * m);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            int c = getId(i, j);
            edges[c] = getEdges(i, j);
        }
    }
}

static void checkGraph(int axis)
{
    int nn = edges.size();

    std::vector<int> relCnt(nn, 0);
    std::vector<int> dist(nn, 0);

    std::queue<int> q;
    for (int i = 0; i < nn; i++) q.push(i);
    std::vector<bool> inQ(nn, true);

    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        inQ[u] = 0;

        for (const auto& e : edges[u])
        {
            int v = e.t;
            int w = e.dc[axis];

            int nd = dist[u] + w;

            if (nd < dist[v])
            {
                dist[v] = nd;
                if (!inQ[v])
                {
                    q.push(v);
                    inQ[v] = true;

                    relCnt[v]++;
                    myAssert(relCnt[v] <= nn, "Incorrect solution");
                }
            }
        }
    }

    std::vector<std::vector<int>> zEdges(nn);
    std::vector<int> inDeg(nn, 0);

    for (int i = 0; i < nn; i++)
    {
        for (const auto& e : edges[i])
        {
            int w = e.dc[axis];
            if (dist[i] + w == dist[e.t])
            {
                zEdges[i].push_back(e.t);
                inDeg[e.t]++;
            }
        }
    }

    for (int i = 0; i < nn; i++)
    {
        if (inDeg[i] == 0) q.push(i);
    }

    int visCnt = 0;

    while (!q.empty())
    {
        int u = q.front();
        q.pop();

        visCnt++;

        for (int t : zEdges[u])
        {
            inDeg[t]--;
            if (inDeg[t] == 0)
            {
                q.push(t);
            }
        }
    }

    myAssert(visCnt == nn, "Incorrect solution");
}

static double evalPattern()
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

int main(int argc, char** args)
{
    graderTimeFile.open(args[1]);
    
    std::cin >> d;

    myAssert(d == 1 || d == 2, "Invalid D");

    pattern = getVisionPattern();

    startTime();

    validatePattern();

    logTime();

    buildGraph();

    checkGraph(1);
    if (d == 2) checkGraph(0);

    double score = evalPattern();

    endTime();

    std::cout << PASSWORD << std::endl;
    std::cout << score << std::endl;
    std::cout << totalGraderTime << std::endl;

    return 0;
}
