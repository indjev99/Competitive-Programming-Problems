#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include <cassert>

#include "puzzle.h"

std::mt19937 generator;

int randInt(int lb, int ub)
{
    assert(ub > lb);
    std::uniform_int_distribution<int> distribution(lb, ub - 1);
    return distribution(generator);
}

int randInt(int ub)
{
    return randInt(0, ub);
}

constexpr int MAX_N = 64;

int n, m;
int arr[MAX_N][MAX_N];

struct Node
{
    int idx;
    int count;
    std::vector<int> edges;

    int avail;

    int origCount;
};

struct Edge
{
    int idx;
    int from;
    int to;
    std::vector<int> intersects;

    int cap;
    int used;
};

int numNodes = 0;
int numEdges = 0;

// state
std::vector<Node> nodes;
std::vector<Edge> edges;
std::vector<int> compPar;
std::vector<int> compCount;

std::vector<int> infStack;
std::vector<bool> isInIS;

int nodesArr[MAX_N][MAX_N];
int horEdgesArr[MAX_N][MAX_N];
int vertEdgesArr[MAX_N][MAX_N];

void printMap(bool printAll = false)
{
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            assert((nodesArr[i][j] == -1) || (horEdgesArr[i][j] == -1 && vertEdgesArr[i][j] == -1));

            int uh = horEdgesArr[i][j] == -1 ? 0 : (printAll ? 1 : edges[horEdgesArr[i][j]].used);
            int uv = vertEdgesArr[i][j] == -1 ? 0 : (printAll ? 1 : edges[vertEdgesArr[i][j]].used);

            if (nodesArr[i][j] != -1) std::cerr << arr[i][j];
            else if (uh > 0 && uv > 0) std::cerr << "+";
            else if (uh == 1) std::cerr << "-";
            else if (uh == 2) std::cerr << "=";
            else if (uv == 1) std::cerr << "|";
            else if (uv == 2) std::cerr << "$";
            else std::cerr << " ";
        }
        std::cerr << std::endl;
    }
    std::cerr << std::endl;
}

void buildGraph()
{
    numNodes = 0;
    numEdges = 0;

    nodes.clear();
    edges.clear();

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            nodesArr[i][j] = -1;
            horEdgesArr[i][j] = -1;
            vertEdgesArr[i][j] = -1;
        }
    }

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            if (arr[i][j] == 0) continue;

            nodes.push_back({numNodes, arr[i][j]});
            nodesArr[i][j] = numNodes++;
        }
    }

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            if (nodesArr[i][j] == -1) continue;

            int found = -1;
            for (int k = j + 1; k < m; ++k)
            {
                if (nodesArr[i][k] != -1)
                {
                    found = nodesArr[i][k];
                    break;
                }
            }

            if (found == -1) continue;

            nodes[nodesArr[i][j]].edges.push_back(numEdges);
            nodes[found].edges.push_back(numEdges);

            edges.push_back({numEdges, nodesArr[i][j], found});

            for (int k = j + 1; nodesArr[i][k] == -1; ++k)
            {
                horEdgesArr[i][k] = numEdges;
            }

            ++numEdges;
        }
    }

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            if (nodesArr[i][j] == -1) continue;

            int found = -1;
            for (int k = i + 1; k < n; ++k)
            {
                if (nodesArr[k][j] != -1)
                {
                    found = nodesArr[k][j];
                    break;
                }
            }

            if (found == -1) continue;

            nodes[nodesArr[i][j]].edges.push_back(numEdges);
            nodes[found].edges.push_back(numEdges);

            edges.push_back({numEdges, nodesArr[i][j], found});

            for (int k = i + 1; nodesArr[k][j] == -1; ++k)
            {
                vertEdgesArr[k][j] = numEdges;

                if (horEdgesArr[k][j] != -1)
                {
                    edges[horEdgesArr[k][j]].intersects.push_back(numEdges);
                    edges[numEdges].intersects.push_back(horEdgesArr[k][j]);
                }
            }

            ++numEdges;
        }
    }

    // printMap(true);
}

void initConstraints()
{
    for (auto& edge : edges)
    {
        edge.used = 0;
        edge.cap = 2;
        for (int ni : {edge.from, edge.to})
        {
            edge.cap = std::min(edge.cap, nodes[ni].count);
        }

        if (nodes[edge.from].count == 1 && nodes[edge.to].count == 1)
        {
            edge.cap = 0;
        }

        if (nodes[edge.from].count == 2 && nodes[edge.to].count == 2)
        {
            edge.cap = 1;
        }
    }

    compPar.clear();
    compPar.resize(numNodes);

    compCount.clear();
    compCount.resize(numNodes);

    for (auto& node : nodes)
    {
        compPar[node.idx] = node.idx;
        compCount[node.idx] = node.count;

        node.avail = 0;
        for (int ei : node.edges)
        {
            node.avail += edges[ei].cap;
        }

        node.origCount = node.count;
    }
}

constexpr int IMPOSSIBLE_EXC = -1;

int findRoot(int a)
{
    if (compPar[a] == a) return a;
    return compPar[a] = findRoot(compPar[a]);
}

void unite(int a, int b)
{
    a = findRoot(a);
    b = findRoot(b);
    if (a == b) return;
    compPar[b] = a;
    compCount[a] += compCount[b];
}

void queueInferNode(int ni)
{
    if (isInIS[ni]) return;
    isInIS[ni] = true;
    infStack.push_back(ni);
}

void delEdge(Edge& edge, bool full = true)
{
    if (edge.cap == 0) return;

    int sub = full ? edge.cap : 1;

    for (int ni : {edge.from, edge.to})
    {
        nodes[ni].avail -= sub;
        queueInferNode(ni);
    }

    edge.cap -= sub;
}

void useEdge(Edge& edge)
{
    assert(edge.cap > 0);

    edge.used++;

    edge.cap--;

    for (int ni : {edge.from, edge.to})
    {
        assert(edge.cap + 1 <= nodes[ni].count);

        int root = findRoot(ni);
        assert(compCount[root] >= nodes[ni].count);

        --nodes[ni].count;
        --nodes[ni].avail;
        --compCount[root];

        assert(nodes[ni].count >= 0);
        assert(nodes[ni].avail >= 0);
        assert(compCount[root] >= 0);

        queueInferNode(ni);

        for (int ei : nodes[ni].edges)
        {
            while (edges[ei].cap > nodes[ni].count)
            {
                delEdge(edges[ei], false);
            }
        }
    }

    if (edge.used == 1)
    {
        unite(edge.from, edge.to);
        for (int oei : edge.intersects)
        {
            delEdge(edges[oei]);
        }
    }
}

void inferNode(Node& node)
{
    if (node.avail < node.count)
    {
        throw IMPOSSIBLE_EXC;
    }

    if (node.count > 0 && node.avail == node.count)
    {
        for (int ei : node.edges)
        {
            Edge& edge = edges[ei];
            if (edge.cap == 2)
            {
                useEdge(edge);
                useEdge(edge);
            }
            else if (edge.cap == 1)
            {
                useEdge(edge);
            }
        }
    }
    else if (node.count > 0 && node.avail == node.count + 1)
    {
        for (int ei : node.edges)
        {
            Edge& edge = edges[ei];
            if (edge.cap == 2)
            {
                useEdge(edge);
            }
        }
    }

    if (node.count == 0)
    {
        for (int ei : node.edges)
        {
            Edge& edge = edges[ei];
            delEdge(edge);
        }
    }
}

void initInfers()
{
    infStack.clear();

    isInIS.clear();
    isInIS.resize(numNodes, false);

    for (int i = 0; i < numNodes; ++i)
    {
        queueInferNode(i);
    }
}

void doInnerInfs()
{
    while (!infStack.empty())
    {
        int ni = infStack.back();
        infStack.pop_back();
        isInIS[ni] = false;

        inferNode(nodes[ni]);
    }
}

void doComps()
{
    int numComps = 0;
    bool hasDone = false;
    for (int i = 0; i < numNodes; ++i)
    {
        if (compPar[i] != i) continue;
        ++numComps;
        if (compCount[i] == 0) hasDone = true;
    }
    if (numComps > 1 && hasDone) throw IMPOSSIBLE_EXC;
}

void doInfs()
{
    // std::cerr << "Inferences" << std::endl;
    doInnerInfs();
    doComps();
}

bool check()
{
    for (Edge& edge : edges)
    {
        if (edge.cap > 0) return false;
    }

    return true;
}

bool probe()
{
    if (check()) return true;

    std::vector<Node> nodesBkp;
    std::vector<Edge> edgesBkp;
    std::vector<int> compParBkp;
    std::vector<int> compCountBkp;

    std::vector<Node> nodesRun1;
    std::vector<Edge> edgesRun1;
    std::vector<int> compParRun1;
    std::vector<int> compCountRun1;

    std::vector<Node> nodesRun2;
    std::vector<Edge> edgesRun2;
    std::vector<int> compParRun2;
    std::vector<int> compCountRun2;

    // std::cerr << "Probing" << std::endl;

    for (int ei = 0; ei < numEdges; ++ei)
    {
        if (edges[ei].cap == 0) continue;

        // std::cerr << "Probing: " << ei << std::endl;

        nodesBkp = nodes;
        edgesBkp = edges;
        compParBkp = compPar;
        compCountBkp = compCount;

        useEdge(edges[ei]);

        bool done1 = false;
        bool imp1 = false;

        try
        {
            doInfs();
            done1 = check();
        }
        catch (int)
        {
            imp1 = true;
        }

        if (done1)
        {
            // std::cerr << "Probed " << ei  << ": Use done" << std::endl;
            return true;
        }

        nodesRun1 = std::move(nodes);
        edgesRun1 = std::move(edges);
        compParRun1 = std::move(compPar);
        compCountRun1 = std::move(compCount);

        nodes = nodesBkp;
        edges = edgesBkp;
        compPar = compParBkp;
        compCount = compCountBkp;

        delEdge(edges[ei]);

        bool done2 = false;
        bool imp2 = false;

        try
        {
            doInfs();
            done2 = check();
        }
        catch (int)
        {
            imp2 = true;
        }

        if (done2)
        {
            // std::cerr << "Probed " << ei  << ": Del done" << std::endl;
            return true;
        }

        nodesRun2 = std::move(nodes);
        edgesRun2 = std::move(edges);
        compParRun2 = std::move(compPar);
        compCountRun2 = std::move(compCount);

        if (imp1 && imp2)
        {
            // std::cerr << "Probed " << ei  << ": Impossible" << std::endl;
            throw IMPOSSIBLE_EXC;
        }

        if (imp1)
        {
            // std::cerr << "Probed " << ei  << ": Del only" << std::endl;

            nodes = std::move(nodesRun2);
            edges = std::move(edgesRun2);
            compPar = std::move(compParRun2);
            compCount = std::move(compCountRun2);
            continue;
        }

        if (imp2)
        {
            // std::cerr << "Probed " << ei  << ": Use only" << std::endl;

            nodes = std::move(nodesRun1);
            edges = std::move(edgesRun1);
            compPar = std::move(compParRun1);
            compCount = std::move(compCountRun1);
            continue;
        }

        nodes = std::move(nodesBkp);
        edges = std::move(edgesBkp);
        compPar = std::move(compParBkp);
        compCount = std::move(compCountBkp);

        for (int ei = 0; ei < numEdges; ++ei)
        {
            if (edges[ei].cap == 0) continue;

            int minUsed = std::min(edgesRun1[ei].used, edgesRun2[ei].used);
            int useMore = minUsed - edges[ei].used;

            int maxTotal = std::max(edgesRun1[ei].used + edgesRun1[ei].cap, edgesRun2[ei].used + edgesRun2[ei].cap);
            int delMore = edges[ei].used + edges[ei].cap - maxTotal;

            for (int i = 0; i < useMore; ++i)
            {
                useEdge(edges[ei]);
            }

            for (int i = 0; i < delMore; ++i)
            {
                delEdge(edges[ei], false);
            }
        }

        // std::string progress = infStack.empty() ? "No progress" : "Progress";

        // std::cerr << "Probed " << ei  << ": Both: " << progress << std::endl;

        doInfs();
    }

    return check();
}

int numExpands = 0;
int maxExpands = 100;

bool solveRec(int depth);

bool expand(int depth)
{
    std::vector<int> found;
    for (Edge& edge : edges)
    {
        if (edge.cap > 0)
        {
            found.push_back(edge.idx);
        }
    }

    if (found.empty()) return true;

    ++numExpands;

    if (numExpands >= maxExpands) return false;

    int chosen = found[randInt(found.size())];

    // std::cerr << "Depth: " << depth << " " << "Expand on: " << chosen << " Iter: " << numExpands << std::endl;

    std::vector<Node> nodesBkp = nodes;
    std::vector<Edge> edgesBkp = edges;
    std::vector<int> compParBkp = compPar;
    std::vector<int> compCountBkp = compCount;

    useEdge(edges[chosen]);

    try
    {
        bool res = solveRec(depth + 1);
        if (!res)
        {
            // std::cerr << "Depth: " << depth << " " << "Cancel: " << chosen << std::endl;

            if (depth == 0)
            {
                nodes = std::move(nodesBkp);
                edges = std::move(edgesBkp);
                compPar = std::move(compParBkp);
                compCount = std::move(compCountBkp);
            }
        }
        return res;
    }
    catch (int)
    {}

    // std::cerr << "Depth: " << depth << " " << "Backtracking on: " << chosen << std::endl;

    nodes = std::move(nodesBkp);
    edges = std::move(edgesBkp);
    compPar = std::move(compParBkp);
    compCount = std::move(compCountBkp);

    delEdge(edges[chosen]);

    return solveRec(depth);
}

bool solveRec(int depth = 0)
{
    doInfs();
    if (probe()) return true;
    return expand(depth);
}

void solveImpl()
{
    buildGraph();
    initConstraints();
    initInfers();

    doInfs();
    // std::cerr << std::endl;
    // printMap();

    probe();
    // std::cerr << std::endl;
    // printMap();

    int trials = 0;
    int totalExpands = 0;

    maxExpands = 75;

    while (true)
    {
        std::cerr << "Starting solve rec" << std::endl;
        numExpands = 0;
        bool res = solveRec();
        ++trials;
        totalExpands += numExpands;
        if (res) break;
        std::cerr << "Didn't complete" << std::endl;
    }

    std::cerr << "Done" << std::endl;
    std::cerr << std::endl;

    // printMap();

    std::cerr << trials << ": " << totalExpands << std::endl;
    std::cerr << std::endl;
}

bool solve(std::vector<std::vector<int>>& puzzle)
{
    n = puzzle.size();
    m = puzzle[0].size();

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            arr[i][j] = puzzle[i][j];
        }
    }

    solveImpl();

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            assert((nodesArr[i][j] == -1) || (horEdgesArr[i][j] == -1 && vertEdgesArr[i][j] == -1));

            int uh = horEdgesArr[i][j] == -1 ? 0 : edges[horEdgesArr[i][j]].used;
            int uv = vertEdgesArr[i][j] == -1 ? 0 : edges[vertEdgesArr[i][j]].used;

            assert(uh == 0 || uv == 0);

            if (nodesArr[i][j] != -1) puzzle[i][j] = arr[i][j];
            else if (uh == 1) puzzle[i][j] = HOR_SINGLE;
            else if (uh == 2) puzzle[i][j] = HOR_DOUBLE;
            else if (uv == 1) puzzle[i][j] = VERT_SINGLE;
            else if (uv == 2) puzzle[i][j] = VERT_DOUBLE;
            else puzzle[i][j] = EMPTY;
        }
    }

    return true;
}

void init(int numSubtests, double timeLimit) {}
