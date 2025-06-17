#include <iostream>
#include <vector>
#include <numeric>

const long long MOD = 1e9 + 7;

const int MAX_C = 1e6 + 1;

struct Edge
{
    int a, b;
};

int n, m;
std::vector<std::vector<Edge>> edgesByCost;

long long ans;

void input()
{
    std::cin >> n >> m;

    edgesByCost.resize(MAX_C);
    for (int i = 0; i < m; i++)
    {
        int a, b, cost;
        std::cin >> a >> b >> cost;
        a--;
        b--;
        edgesByCost[cost].push_back({a, b});
    }
}

void output()
{
    std::cout << ans << std::endl;
}

struct DSU
{
    std::vector<int> pars;
    std::vector<int> sizes;

    std::vector<Edge> unionsToUndo;

    void init()
    {
        pars.resize(n);
        std::iota(pars.begin(), pars.end(), 0);
        sizes.resize(n, 1);
    }

    template <bool CMPR>
    int find(int curr)
    {
        if constexpr (CMPR)
        {
            if (curr == pars[curr]) return curr;
            return pars[curr] = find<true>(pars[curr]);
        }
        else
        {
            while (pars[curr] != curr) curr = pars[curr];
            return curr;
        }
    }

    template <bool CMPR>
    bool unite(int a, int b)
    {
        a = find<CMPR>(a);
        b = find<CMPR>(b);

        if (a == b) return false;

        if (sizes[a] < sizes[b]) std::swap(a, b);

        pars[b] = a;
        sizes[a] += sizes[b];

        if constexpr (!CMPR)
        {
            unionsToUndo.push_back({a, b});
        }

        return true;
    }

    void undo()
    {
        auto [a, b] = unionsToUndo.back();
        unionsToUndo.pop_back();

        pars[b] = b;
        sizes[a] -= sizes[b];
    }

    void undoAll()
    {
        while (!unionsToUndo.empty())
        {
            undo();
        }
    }
};

int currCost;

DSU dsu;

int maxTaken;
int currTaken;

long long currAns;

std::vector<Edge> currEdges;

void solveRec(int currIdx)
{
    if (currTaken == maxTaken)
    {
        currAns++;
        return;
    }

    if (maxTaken - currTaken > (int) currEdges.size() - currIdx)
    {
        return;
    }

    solveRec(currIdx + 1);

    auto [a, b] = currEdges[currIdx];

    if (!dsu.unite<false>(a, b)) return;
    currTaken++;

    solveRec(currIdx + 1);

    dsu.undo();
    currTaken--;
}

void solve()
{
    dsu.init();
    ans = 1;
    for (currCost = 0; currCost < MAX_C; currCost++)
    {
        currEdges.clear();

        for (auto [a, b] : edgesByCost[currCost])
        {
            a = dsu.find<true>(a);
            b = dsu.find<true>(b);

            currEdges.push_back({a, b});
        }

        if (currEdges.empty()) continue;

        dsu.unionsToUndo.clear();

        maxTaken = 0;

        for (auto [a, b] : currEdges)
        {
            if (dsu.unite<false>(a, b)) maxTaken++;
        }

        if (maxTaken == 0 || maxTaken == (int) currEdges.size()) continue;

        dsu.undoAll();

        currAns = 0;

        solveRec(0);

        ans = ans * currAns % MOD;

        for (auto [a, b] : currEdges)
        {
            dsu.unite<true>(a, b);
        }
    }
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
