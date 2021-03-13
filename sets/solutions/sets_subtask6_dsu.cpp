#include <iostream>
#include <vector>
#include <stack>

const int MAX_N = 2e6;
const int INF = 2e8;

int n;
int x[MAX_N], y[MAX_N];

int ans;

void input()
{
    std::cin >> n;
    for (int i = 0; i < n; ++i)
    {
        std::cin >> x[i] >> y[i];
    }
}

void output()
{
    std::cout << ans << std::endl;
}

// prefY[i] = Y[0, i)
int prefMinY[MAX_N + 1], prefMaxY[MAX_N + 1];

// suffY[i] = Y[i, n)
int suffMinY[MAX_N + 1], suffMaxY[MAX_N + 1];

// leftMin[i] = -X[i] - min prefY[i]
// leftMax[i] = -X[i] + max prefY[i]
// leftMinMax[i] = -X[i] + max prefY[i] - min prefY[i]

int leftMin[MAX_N + 1];
int leftMax[MAX_N + 1];
int leftMinMax[MAX_N + 1];

struct Query
{
    int* id;
    int l, r;
    int base;
};

int par[MAX_N + 1];
std::vector<Query> qs[MAX_N + 1];

template<int* vals>
struct DsuRmq
{
    int maxR;

    void addQuery(int l, int r, int base)
    {
        if (r <= l) return;
        maxR = std::max(maxR, r);
        qs[r].push_back({vals, l, r, base});
    }

    int findRoot(int curr)
    {
        if (par[curr] == curr) return curr;
        return par[curr] = findRoot(par[curr]);
    }

    void answerQueries()
    {
        std::stack<int> roots;
        for (int i = 0; i <= maxR; ++i)
        {
            for (const Query& q : qs[i])
            {
                if (q.id != vals) continue;
                int currAns = vals[findRoot(q.l)] + q.base;
                ans = std::min(ans, currAns);
            }
            while (!roots.empty() && vals[roots.top()] >= vals[i])
            {
                par[roots.top()] = i;
                roots.pop();
            }
            roots.push(i);
            par[i] = i;
        }
    }
};

DsuRmq<leftMin> leftMinDsuRmq;
DsuRmq<leftMax> leftMaxDsuRmq;
DsuRmq<leftMinMax> leftMinMaxDsuRmq;

void solve()
{
    prefMinY[0] = INF;
    prefMaxY[0] = -INF;
    for (int i = 1; i <= n; ++i)
    {
        prefMinY[i] = std::min(prefMinY[i - 1], y[i - 1]);
        prefMaxY[i] = std::max(prefMaxY[i - 1], y[i - 1]);
    }

    suffMinY[n] = INF;
    suffMaxY[n] = -INF;
    for (int i = n - 1; i >= 0; --i)
    {
        suffMinY[i] = std::min(suffMinY[i + 1], y[i]);
        suffMaxY[i] = std::max(suffMaxY[i + 1], y[i]);
    }

    for (int i = 0; i <= n; ++i)
    {
        leftMin[i] = -x[i] - prefMinY[i];
        leftMax[i] = -x[i] + prefMaxY[i];
        leftMinMax[i] = -x[i] + prefMaxY[i] - prefMinY[i];
    }

    ans = INF;
    int minChangePos = 0, maxChangePos = 0;
    leftMinMaxDsuRmq.addQuery(1, n, x[n - 1]);
    for (int r = n - 1; r >= 1; --r)
    {
        minChangePos = std::min(minChangePos, r);
        maxChangePos = std::min(maxChangePos, r);
        while (minChangePos < r && prefMinY[minChangePos] > suffMinY[r]) ++minChangePos;
        while (maxChangePos < r && prefMaxY[maxChangePos] < suffMaxY[r]) ++maxChangePos;

        int currAnsNone = x[r - 1] - x[std::min(minChangePos, maxChangePos) - 1] + suffMaxY[r] - suffMinY[r];
        leftMinDsuRmq.addQuery(minChangePos, maxChangePos, x[r - 1] + suffMaxY[r]);
        leftMaxDsuRmq.addQuery(maxChangePos, minChangePos, x[r - 1] - suffMinY[r]);
        leftMinMaxDsuRmq.addQuery(std::max(minChangePos, maxChangePos), r, x[r - 1]);

        ans = std::min(ans, currAnsNone);
    }

    leftMinDsuRmq.answerQueries();
    leftMaxDsuRmq.answerQueries();
    leftMinMaxDsuRmq.answerQueries();
}

int main()
{
    std::ios::sync_with_stdio(false);

    input();
    solve();
    output();

    return 0;
}
