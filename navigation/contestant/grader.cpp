#include "navigation.h"

#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <random>

static bool DEBUG = true;

static void myAssert(bool cond, const std::string& message)
{
    if (cond) return;

    std::cout << message << std::endl;
    exit(0);
}

static std::mt19937 generator;
static int randInt(int lb, int ub)
{
    std::uniform_int_distribution<int> distr(lb, ub - 1);
    return distr(generator);
}

struct Graph
{
    int idx;
    int n;
    int m;
    int start;

    std::vector<std::vector<int>> adj;

    int curr;
    bool terminated;
    int numIters;
    std::vector<int> colors;
    std::vector<bool> visited;
};

static Graph inputGraph(int idx)
{
    Graph graph;

    graph.idx = idx;

    std::cin >> graph.n >> graph.m;
    std::cin >> graph.start;

    graph.adj.assign(graph.n, {});

    for (int i = 0; i < graph.m; i++)
    {
        int from, to;
        std::cin >> from >> to;

        graph.adj[from].push_back(to);
        graph.adj[to].push_back(from);
    }

    for (int i = 0; i < graph.n; i++)
    {
        std::shuffle(graph.adj[i].begin(), graph.adj[i].end(), generator);
    }

    graph.curr = graph.start;
    graph.terminated = false;
    graph.numIters = 0;
    graph.colors.assign(graph.n, INIT_COLOR);
    graph.visited.assign(graph.n, false);
    graph.visited[graph.curr] = true;

    return graph;
};

template <class T>
struct VectorHasher
{
    std::size_t operator()(const std::vector<T>& v) const
    {
        std::size_t hash = v.size();
        for (const auto& i : v)
        {
            hash ^= std::hash<T>{}(i) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }
        return hash;
    }
};

static std::pair<int, int> getAction(int currColor, const std::vector<int>& adjColors)
{
    static std::vector<int> key;
    static std::unordered_map<std::vector<int>, std::pair<int, int>, VectorHasher<int>> actionCache;

    key = adjColors;
    key.push_back(currColor);

    auto it = actionCache.find(key);
    if (it != actionCache.end())
    {
        return it->second;
    }

    std::pair<int, int> action = navigate(currColor, adjColors);

    actionCache.insert({key, action});

    return action;
}

static bool colorUsed[MAX_COLORS];

static bool oneIter(Graph& graph)
{
    if (graph.terminated)
    {
        return false;
    }

    static std::vector<int> adjColors;

    int currColor = graph.colors[graph.curr];

    adjColors.clear();
    for (int other : graph.adj[graph.curr])
    {
        adjColors.push_back(graph.colors[other]);
    }

    if (DEBUG)
    {
        std::cerr << std::endl;
        std::cerr << "Graph: " << graph.idx << std::endl;
        std::cerr << "Iter: " << graph.numIters + 1 << std::endl;
        std::cerr << "Nodes: " << graph.curr << ", [";
        for (int i = 0; i < (int) adjColors.size(); i++)
        {
            if (i > 0) std::cerr << ", ";
            std::cerr << graph.adj[graph.curr][i];
        }
        std::cerr << "]" << std::endl;
        std::cerr << "Colors: " << currColor << ", [";
        for (int i = 0; i < (int) adjColors.size(); i++)
        {
            if (i > 0) std::cerr << ", ";
            std::cerr << adjColors[i];
        }
        std::cerr << "]" << std::endl;
    }

    std::pair<int, int> action = getAction(currColor, adjColors);

    if (DEBUG)
    {
        std::cerr << "Action: " << action.first << ", " << action.second << std::endl;
    }

    graph.numIters++;

    if (action.first == TERMINATE_CODE && action.second == TERMINATE_CODE)
    {
        for (int i = 0; i < graph.n; i++)
        {
            myAssert(graph.visited[i], "Not all nodes visited.");
        }

        graph.terminated = true;

        return true;
    }

    myAssert(action.first >= 0 && action.first < MAX_COLORS, "Invalid color.");
    myAssert(action.second >= 0 && action.second < (int) adjColors.size(), "Invalid adjacency index.");

    colorUsed[action.first] = true;

    graph.colors[graph.curr] = action.first;
    graph.curr = graph.adj[graph.curr][action.second];
    graph.visited[graph.curr] = true;

    myAssert(graph.numIters < MAX_ITERS, "Exceeded max iterations.");

    return false;
}

int main()
{
    generator.seed(0);

    int numGraphs;
    std::cin >> numGraphs;

    std::vector<Graph> graphs;
    for (int i = 0; i < numGraphs; i++)
    {
        graphs.push_back(inputGraph(i));
    }

    colorUsed[INIT_COLOR] = true;

    int numGraphsTerminated = 0;
    while (numGraphsTerminated < numGraphs)
    {
        int graphIdx = randInt(0, numGraphs);
        bool terminated = oneIter(graphs[graphIdx]);
        if (terminated) numGraphsTerminated++;
    }

    int numColorsUsed = 0;
    for (int i = 0; i < MAX_COLORS; i++)
    {
        if (colorUsed[i]) numColorsUsed++;
    }

    int maxIters = 0;
    for (int i = 0; i < numGraphs; i++)
    {
        maxIters = std::max(maxIters, graphs[i].numIters);
    }

    if (DEBUG)
    {
        std::cerr << std::endl;
    }

    std::cout << "Number of colors used: " << numColorsUsed << std::endl;
    std::cout << "Max number of iterations: " << maxIters << std::endl;

    return 0;
};
