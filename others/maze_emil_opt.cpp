#include <iostream>
#include <vector>

const int MAX_N = 1500;
const int MAX_NODES = MAX_N * MAX_N + 1;

int n, m;
char dir[MAX_NODES];

int next[MAX_NODES];
std::vector<int> prevs[MAX_NODES];

int enterT[MAX_NODES];
int exitT[MAX_NODES];

int dist[MAX_NODES];
int cyclePoint[MAX_NODES];
int cycle[MAX_NODES];
int cycleEntry[MAX_NODES];
bool onCycle[MAX_NODES];

int cycleLens[MAX_NODES];

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
            std::cin >> dir[curr];
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
            next[curr] = nextIdx(x, y, dir[curr]);
            prevs[next[curr]].push_back(curr);
        }
    }
}

int currT = 1;
void revDfs(int curr, int currDist, int currCyclePoint)
{
    enterT[curr] = currT++;

    dist[curr] = currDist;
    cyclePoint[curr] = currCyclePoint;

    for (int prev : prevs[curr])
    {
        if (!enterT[prev]) revDfs(prev, currDist + 1, currCyclePoint);
    }

    exitT[curr] = currT++;
}

int findCycleStart(int curr)
{
    if (cyclePoint[curr]) return curr;
    cyclePoint[curr] = -1;
    return findCycleStart(next[curr]);
}

int processCycle(int curr, int currCycle, int currCycleEntry, int cycleStart)
{
    enterT[curr] = -1;

    if (currCycleEntry > 0)
    {
        revDfs(curr, 0, curr);
        if (curr == cycleStart) return currCycleEntry;
    }

    onCycle[curr] = true;
    cycle[curr] = currCycle;
    cycleEntry[curr] = currCycleEntry;

    return processCycle(next[curr], currCycle, currCycleEntry + 1, cycleStart);
}

void precompute()
{
    addEdges();
    revDfs(exitIdx(), 0, exitIdx());

    int cycle = 1;
    for (int curr = 0; curr < n * m; ++curr)
    {
        if (enterT[curr]) continue;
        int cycleStart = findCycleStart(curr);
        cycleLens[cycle] = processCycle(cycleStart, cycle, 0, cycleStart);
        ++cycle;
    }
}

bool isAncestor(int curr, int ancestor)
{
    return enterT[ancestor] <= enterT[curr] && exitT[curr] <= exitT[ancestor];
}

int distToChange(int curr, int change)
{
    if (isAncestor(curr, change)) return dist[curr] - dist[change] + 1;
    int currCP = cyclePoint[curr];
    int changeCP = cyclePoint[change];
    if (cycle[currCP] == cycle[changeCP] && onCycle[change])
    {
        int alongCycle = cycleEntry[changeCP] - cycleEntry[currCP];
        if (alongCycle < 0) alongCycle += cycleLens[cycle[currCP]];
        return dist[curr] + alongCycle + 1;
    }
    return 0;
}

int solveQuery(int sx, int sy, int cx, int cy, int cdir)
{
    int curr = currIdx(sx, sy);
    int change = currIdx(cx, cy);
    int newNext = nextIdx(cx, cy, cdir);

    if (!cycle[cyclePoint[curr]] && !isAncestor(curr, change)) return dist[curr];
    int dtc = distToChange(curr, change);
    if (dtc && !cycle[cyclePoint[newNext]] && !isAncestor(newNext, change)) return dtc + dist[newNext];
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
