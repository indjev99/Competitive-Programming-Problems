#include <iostream>
#include <vector>
#include <unordered_set>

struct Graph
{
    int n;
    int start;

    std::vector<std::vector<int>> adj;
};

Graph inputGraph()
{
    Graph g;

    std::cin >> g.n;
    std::cin >> g.start;

    g.adj.assign(g.n, {});

    for (int i = 0; i < g.n; i++)
    {
        int numAdj;
        std::cin >> numAdj;

        for (int j = 0; j < numAdj; j++)
        {
            int to;
            std::cin >> to;
            g.adj[i].push_back(to);
        }
    }

    return g;
};

constexpr int INIT_COLOR = 0;

using Action = std::pair<int, int>;

constexpr Action UNDEF_ACT = {-2, -2};
constexpr Action TERM_ACT = {-1, -1};

int iPow(int base, int exp)
{
    int ans = 1;
    for (int i = 0; i < exp; i++)
    {
        ans *= base;
    }
    return ans;
}

struct State
{
    const Graph& g;
    int numColors;

    int curr;
    std::vector<int> cols;
    std::vector<bool> vis;

    std::unordered_set<int> seenConfigs;

    bool failCycles = false;
    bool failNonvis = false;
    bool succTerm = false;

    State(const Graph& g, int numColors):
        g(g),
        numColors(numColors),
        curr(g.start),
        cols(g.n, INIT_COLOR),
        vis(g.n, false)
    {
        vis[g.start] = true;
        updateSeenConfigs();
    }

    State(const State& other) = default;
    State(State&& other) = default;
    State& operator=(const State& other) = default;
    State& operator=(State&& other) = default;

    bool isDone() const
    {
        return failCycles || failNonvis || succTerm;
    }

    std::vector<int> getColors() const
    {
        std::vector<int> colors = {cols[curr]};
        for (int i = 0; i < (int) g.adj[curr].size(); i++)
        {
            colors.push_back(cols[g.adj[curr][i]]);
        }
        return colors;
    }

    void applyAction(Action act)
    {
        if (act == TERM_ACT)
        {
            for (bool v : vis)
            {
                if (!v)
                {
                    failNonvis = true;
                    return;
                }
            }

            succTerm = true;
            return;
        }

        cols[curr] = act.first;
        curr = g.adj[curr][act.second];
        vis[curr] = true;

        updateSeenConfigs();
    }

    int getConfig() const
    {
        int config = curr;
        for (int i = 0; i < g.n; i++)
        {
            config *= numColors;
            config += cols[i];
        }
        return config;
    }

    void updateSeenConfigs()
    {
        int config = getConfig();
        auto ret = seenConfigs.insert(config);
        failCycles = failCycles || !ret.second;
    }
};

struct Strategy
{
    int numColors;
    int maxDegree;

    int nextColor;
    std::vector<Action> actRules;

    Strategy(int numColors, int maxDegree):
        numColors(numColors),
        maxDegree(maxDegree),
        nextColor(INIT_COLOR + 1),
        actRules(iPow(numColors + 1, maxDegree + 1), UNDEF_ACT)
    {}

    Strategy(const Strategy& other) = default;
    Strategy(Strategy&& other) = default;
    Strategy& operator=(const Strategy& other) = default;
    Strategy& operator=(Strategy&& other) = default;

    int getRuleId(const std::vector<int>& colors) const
    {
        int id = 0;
        for (int i = 0; i <= maxDegree; i++)
        {
            id *= numColors + 1;
            if (i < (int) colors.size()) id += colors[i];
            else id += numColors;
        }
        return id;
    }

    std::vector<Strategy> expand(int ruleId, const std::vector<int>& colors) const
    {
        if (actRules[ruleId] != UNDEF_ACT)
        {
            return {*this};
        }

        std::vector<Strategy> strats;

        strats.push_back(*this);
        strats.back().actRules[ruleId] = TERM_ACT;


        for (int c = 0; c < std::min(numColors, nextColor + 1); c++)
        {
            for (int idx = 0; idx < (int) colors.size() - 1; idx++)
            {
                strats.push_back(*this);
                strats.back().actRules[ruleId] = {c, idx};
                if (c == nextColor) strats.back().nextColor++;
            }
        }

        return strats;
    }
};

struct StrategyWithState
{
    Strategy strat;
    State state;

    template <class StratT, class StateT>
    StrategyWithState(StratT&& strat, StateT&& state):
        strat(std::forward<StratT>(strat)),
        state(std::forward<StateT>(state))
    {}

    StrategyWithState(const StrategyWithState& other) = default;
    StrategyWithState(StrategyWithState&& other) = default;
    StrategyWithState& operator=(const StrategyWithState& other) = default;
    StrategyWithState& operator=(StrategyWithState&& other) = default;

    bool isDone() const
    {
        return state.isDone();
    }

    std::vector<StrategyWithState> expand() const
    {
        std::vector<int> colors = state.getColors();
        int ruleId = strat.getRuleId(colors);

        std::vector<Strategy> strats = strat.expand(ruleId, colors);

        std::vector<StrategyWithState> stStrats;

        for (int i = 0; i < (int) strats.size(); i++)
        {
            stStrats.emplace_back(std::move(strats[i]), state);
            stStrats.back().state.applyAction(stStrats.back().strat.actRules[ruleId]);
        }

        return stStrats;
    }
};

int main()
{
    int numColors = 2;
    int maxDegree = 2;

    std::vector<Strategy> strats;

    strats.emplace_back(numColors, maxDegree);

    while (true)
    {
        std::cout << std::endl;
        std::cout << "Num strats: " << strats.size() << std::endl;

        Graph g = inputGraph();
        if (g.n == 0) break;

        std::vector<StrategyWithState> stStrats;
        for (int i = 0; i < (int) strats.size(); i++)
        {
            stStrats.emplace_back(std::move(strats[i]), State(g, numColors));
        }
        strats.clear();

        int iters = 0;

        while (iters < 3 * g.n && !stStrats.empty())
        {
            int numFailCycle = 0;
            int numFailNonvis = 0;
            int numSuccTerm = 0;

            std::vector<StrategyWithState> nextStStrats;

            for (int i = 0; i < (int) stStrats.size(); i++)
            {
                std::vector<StrategyWithState> expStSts = stStrats[i].expand();

                for (int j = 0; j < (int) expStSts.size(); j++)
                {
                    if (!expStSts[j].isDone())
                    {
                        nextStStrats.emplace_back(std::move(expStSts[j]));
                    }
                    else if (expStSts[j].state.succTerm)
                    {
                        strats.emplace_back(std::move(expStSts[j].strat));
                        numSuccTerm++;
                    }
                    else if (expStSts[j].state.failCycles) numFailCycle++;
                    else if (expStSts[j].state.failNonvis) numFailNonvis++;
                }
            }

            stStrats = std::move(nextStStrats);

            iters++;

            std::cout << std::endl;
            std::cout << "Iter: " << iters << std::endl;
            std::cout << "Num failed cycled: " << numFailCycle << std::endl;
            std::cout << "Num failed nonvis: " << numFailNonvis << std::endl;
            std::cout << "Num succ term: " << numSuccTerm << std::endl;
            std::cout << "Num curr strats: " << stStrats.size() << std::endl;
            std::cout << "Total succ strats: " << strats.size() << std::endl;
        }
    }

    return 0;
};
