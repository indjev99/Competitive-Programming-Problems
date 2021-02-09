#include <iostream>
#include <vector>

const int MAX_N = 1500;

struct Point
{
    char dir;

    int next;
    std::vector<int> prevs;

    bool vis;
    bool vis2;

    int enterT;
    int exitT;

    int dist;
    int cycle;
    int cycleEntry;
    bool onCycle;
};

int n, m;
Point ps[MAX_N * MAX_N + 1];
int cycleLens[MAX_N * MAX_N + 1];

int exitIdx()
{
    return n * m;
}

int currIdx(int x, int y)
{
    return m * x + y;
}

int nextIdx(int x, int y, char dir)
{
    int nextX = x;
    int nextY = y;

    switch (dir)
    {
    case '<': --nextY; break;
    case '>': ++nextY; break;
    case '^': --nextX; break;
    case 'v': ++nextX; break;
    }

    if (nextX == -1 || nextX == n || nextY == -1 || nextY == m) return exitIdx();
    else return currIdx(nextX, nextY);
}

void input()
{
    std::cin >> n >> m;

    for (int x = 0; x < n; ++x)
    {
        for (int y = 0; y < m; ++y)
        {
            int curr = currIdx(x, y);
            std::cin >> ps[curr].dir;
        }
    }
}

void addEdges()
{
    for (int x = 0; x < n; ++x)
    {
        for (int y = 0; y < m; ++y)
        {
            int curr = currIdx(x, y);
            int next = nextIdx(x, y, ps[curr].dir);
            ps[curr].next = next;
            ps[next].prevs.push_back(curr);
        }
    }
}

int currT = 0;
void revDfs(int curr, int dist, int cycle, int cycleEntry)
{
    ps[curr].vis = true;
    ps[curr].dist = dist;
    ps[curr].cycle = cycle;
    ps[curr].cycleEntry = cycleEntry;

    ps[curr].enterT = currT++;

    for (int prev : ps[curr].prevs)
    {
        if (!ps[prev].vis) revDfs(prev, dist + 1, cycle, cycleEntry);
    }

    ps[curr].exitT = currT++;
}

int findCycleStart(int curr)
{
    if (ps[curr].vis2) return curr;
    ps[curr].vis2 = true;
    return findCycleStart(ps[curr].next);
}

int processCycle(int curr, int cycle, int cycleEntry, int cycleStart)
{
    ps[curr].vis = true;
    ps[curr].onCycle = true;

    if (curr != cycleStart) revDfs(curr, 0, cycle, cycleEntry);
    else if (cycleEntry > 0)
    {
        revDfs(curr, 0, cycle, 0);
        return cycleEntry;
    }

    return processCycle(ps[curr].next, cycle, cycleEntry + 1, cycleStart);
}

void precompute()
{
    addEdges();
    revDfs(exitIdx(), 0, 0, 0);

    int cycle = 1;
    for (int curr = 0; curr < n * m; ++curr)
    {
        if (ps[curr].vis) continue;
        int cycleStart = findCycleStart(curr);
        cycleLens[cycle] = processCycle(cycleStart, cycle, 0, cycleStart);
        ++cycle;
    }
}

bool isAncestor(int curr, int ancestor)
{
    return ps[ancestor].enterT <= ps[curr].enterT && ps[curr].exitT <= ps[ancestor].exitT;
}

int distToChange(int curr, int change)
{
    if (isAncestor(curr, change)) return ps[curr].dist - ps[change].dist + 1;
    if (ps[curr].cycle == ps[change].cycle && ps[change].onCycle)
    {
        int alongCycle = ps[change].cycleEntry - ps[curr].cycleEntry;
        if (alongCycle < 0) alongCycle += cycleLens[ps[curr].cycle];
        return ps[curr].dist + alongCycle + 1;
    }
    return 0;
}

int solveQuery(int sx, int sy, int cx, int cy, int cdir)
{
    int curr = currIdx(sx, sy);
    int change = currIdx(cx, cy);
    int newNext = nextIdx(cx, cy, cdir);

    if (!ps[curr].cycle && !isAncestor(curr, change)) return ps[curr].dist;
    int dist = distToChange(curr, change);
    if (dist && !ps[newNext].cycle && !isAncestor(newNext, change)) return dist + ps[newNext].dist;
    return 0;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    input();
    precompute();

    int q;
    std::cin >> q;

    for (int i = 0; i < q; ++i)
    {
        int sx, sy, cx, cy;
        char cdir;
        std::cin >> sx >> sy >> cx >> cy >> cdir;
        std::cout << solveQuery(sx - 1, sy - 1, cx - 1, cy - 1, cdir) << "\n";
    }

    return 0;
}
