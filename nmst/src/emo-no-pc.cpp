#include <iostream>
#include <vector>
#include <numeric>

const long long MOD = 1e9 + 7;

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

    edgesByCost.resize(1000001);
    for (int i = 0; i < m; i++)
    {
        int a, b, cost;
        std::cin >> a >> b >> cost;
		a--; b--;
        edgesByCost[cost].push_back({a, b});
    }
}

void output()
{
    std::cout << ans << std::endl;
}

int byLen[20];

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

    int find(int curr)
    {
		int i = 0;
		while (pars[curr] != curr) {
			curr = pars[curr];
			i++;
		}
		if(i >= 20) i = 19;
		byLen[i]++;
		return curr;
    }

    template <bool CMPR>
    bool unite(int a, int b)
    {
        a = find(a);
        b = find(b);

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
    for (currCost = 0; currCost < (int)edgesByCost.size(); currCost++)
    {
        currEdges.clear();

        for (auto [a, b] : edgesByCost[currCost])
        {
            a = dsu.find(a);
            b = dsu.find(b);

            if (a == b) continue;

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

	for(size_t i = 0; i < 20; i++) std::cerr << ' ' << byLen[i];
	std::cerr << '\n';

    return 0;
}

