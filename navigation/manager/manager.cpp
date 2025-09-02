#include "manager.h"

#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <random>

constexpr int INIT_COLOR = 0;
constexpr int MAX_COLORS = 1500;
constexpr int TERMINATE_CODE = -1;
constexpr int MAX_ITERS = 3000;

constexpr int NUM_PROCS = 100;

const std::string PASSWORD_CHECKER = "fdi84fj29slfh20fd5jkd";
const std::string PASSWORD_GRADER_AUTH = "9ds82uiwe9tusd342a";
const std::string PASSWORD_MANAGER_AUTH = "kjdsgdfihe09tsdfuei";

char** fds;

void finish()
{
    for (int i = 0; i < 2 * NUM_PROCS; i++)
    {
        close(atoi(fds[i]));
    }

    exit(0);
}

void myAssert(bool cond, const std::string& message)
{
    if (cond) return;

    std::cout << PASSWORD_CHECKER << std::endl;
    std::cout << -1 << std::endl;
    std::cout << message << std::endl;

    finish();
}

std::mt19937 generator;
int randInt(int lb, int ub)
{
    std::uniform_int_distribution<int> distr(lb, ub - 1);
    return distr(generator);
}

template <typename InType>
void checkGraderAuthAndAlive(InType& in)
{
    std::string pass;
    in >> pass;

    myAssert(pass == PASSWORD_GRADER_AUTH, "Violation of the protocol for communication.");
}

std::pair<int, int> callNavigate(int currColor, const std::vector<int>& adjColors)
{
    int proc = randInt(0, NUM_PROCS);

    out(proc) << PASSWORD_MANAGER_AUTH << std::endl;

    out(proc) << currColor << " " << adjColors.size();
    for (int col : adjColors)
    {
        out(proc) << " " << col;
    }
    out(proc) << std::endl;

    checkGraderAuthAndAlive(in(proc));

    int newCol, newIdx;
    in(proc) >> newCol >> newIdx;

    return {newCol, newIdx};
}

struct Graph
{
    int idx;
    int n;
    int start;

    std::vector<std::vector<int>> adj;

    int curr;
    bool terminated;
    int numIters;
    std::vector<int> colors;
    std::vector<bool> visited;
};

Graph inputGraph(int idx)
{
    Graph graph;

    graph.idx = idx;

    std::cin >> graph.n;
    std::cin >> graph.start;

    graph.adj.assign(graph.n, {});

    for (int i = 0; i < graph.n; i++)
    {
        int numAdj;
        std::cin >> numAdj;

        for (int j = 0; j < numAdj; j++)
        {
            int to;
            std::cin >> to;
            graph.adj[i].push_back(to);
        }
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

std::pair<int, int> getAction(int currColor, const std::vector<int>& adjColors)
{
    std::vector<int> key;
    std::unordered_map<std::vector<int>, std::pair<int, int>, VectorHasher<int>> actionCache;

    key = adjColors;
    key.push_back(currColor);

    auto it = actionCache.find(key);
    if (it != actionCache.end())
    {
        return it->second;
    }

    std::pair<int, int> action = callNavigate(currColor, adjColors);

    actionCache.insert({key, action});

    return action;
}

bool colorUsed[MAX_COLORS];

bool oneIter(Graph& graph)
{
    if (graph.terminated)
    {
        return false;
    }

    std::vector<int> adjColors;

    int currColor = graph.colors[graph.curr];

    adjColors.clear();
    for (int other : graph.adj[graph.curr])
    {
        adjColors.push_back(graph.colors[other]);
    }

    std::pair<int, int> action = getAction(currColor, adjColors);

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

int main(int argc, char* argv[])
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    fds = argv + 1;
    fd_to_stream(fds, 2 * NUM_PROCS);

    int seed;
    std::cin >> seed;

    generator.seed(seed);

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

    std::cout << PASSWORD_CHECKER << std::endl;
    std::cout << numColorsUsed << std::endl;
    std::cout << maxIters << std::endl;

    finish();

    return 0;
};
