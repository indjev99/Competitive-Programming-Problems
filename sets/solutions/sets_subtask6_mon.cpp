#include <iostream>
#include <vector>
#include <queue>

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

template<int* vals>
struct MonRmq
{
    int l, r;
    bool done;
    std::deque<int> contents;

    void setDone()
    {
        done = true;
        contents.clear();
    }

    void updateLR(int newL, int newR)
    {
        if (done) return;
        if (newR <= newL) newR = newL;
        while (r < newR) addVal(vals[r++]);
        while (l < newL) removeVal(vals[l++]);
    }

    void addVal(int val)
    {
        while (!contents.empty() && val < contents.back()) contents.pop_back();
        contents.push_back(val);
    }

    void removeVal(int val)
    {
        if (contents.front() == val) contents.pop_front();
    }

    int getMin()
    {
        if (contents.empty()) return INF;
        return contents.front();
    }
};

template<int* vals>
struct StackRmq
{
    int base, r;
    std::vector<int> contents;

    StackRmq(): contents({INF}) {}

    void setBase(int newBase)
    {
        base = newBase;
        r = newBase;
    }

    void updateR(int newR)
    {
        r = contents.size() + base - 1;
        while (r < newR) contents.push_back(std::min(contents.back(), vals[r++]));
        r = std::max(newR, base);
    }

    int getMin()
    {
        return contents[r - base];
    }
};

MonRmq<leftMin> leftMinMonRmq;
MonRmq<leftMax> leftMaxMonRmq;
MonRmq<leftMinMax> leftMinMaxMonRmq;

StackRmq<leftMin> leftMinStackRmq;
StackRmq<leftMax> leftMaxStackRmq;
StackRmq<leftMinMax> leftMinMaxStackRmq;

int minChangePos[MAX_N];
int maxChangePos[MAX_N];

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

    int minOverPos = 0, maxOverPos = 0;
    for (int r = n - 1; r >= 1; --r)
    {
        minChangePos[r] = r < n - 1 ? std::min(minChangePos[r + 1], r) : 0;
        maxChangePos[r] = r < n - 1 ? std::min(maxChangePos[r + 1], r) : 0;
        while (minChangePos[r] < r && prefMinY[minChangePos[r]] > suffMinY[r]) ++minChangePos[r];
        while (maxChangePos[r] < r && prefMaxY[maxChangePos[r]] < suffMaxY[r]) ++maxChangePos[r];
        if (!minOverPos && minChangePos[r] == r) minOverPos = r;
        if (!maxOverPos && maxChangePos[r] == r) maxOverPos = r;
    }
    
    int fstOverPos = std::max(minOverPos, maxOverPos);
    int sndOverPos = std::min(minOverPos, maxOverPos);

    leftMinStackRmq.setBase(sndOverPos);
    leftMaxStackRmq.setBase(sndOverPos);
    leftMinMaxStackRmq.setBase(fstOverPos);

    ans = INF;
    for (int r = n - 1; r >= 1; --r)
    {
        if (r == fstOverPos) leftMinMaxMonRmq.setDone();
        if (r == sndOverPos)
        {
            leftMinMonRmq.setDone();
            leftMaxMonRmq.setDone();
        }

        leftMinMonRmq.updateLR(minChangePos[r], std::min(maxChangePos[r], sndOverPos));
        leftMaxMonRmq.updateLR(maxChangePos[r], std::min(minChangePos[r], sndOverPos));
        leftMinMaxMonRmq.updateLR(std::max(minChangePos[r], maxChangePos[r]), std::min(r, fstOverPos));

        leftMinStackRmq.updateR(maxChangePos[r]);
        leftMaxStackRmq.updateR(minChangePos[r]);
        leftMinMaxStackRmq.updateR(r);

        int currAnsNone = x[r - 1] - x[std::min(minChangePos[r], maxChangePos[r]) - 1] + suffMaxY[r] - suffMinY[r];
        int currAnsMin = x[r - 1] + std::min(leftMinMonRmq.getMin(), leftMinStackRmq.getMin()) + suffMaxY[r];
        int currAnsMax = x[r - 1] + std::min(leftMaxMonRmq.getMin(), leftMaxStackRmq.getMin()) - suffMinY[r];
        int currAnsMinMax = x[r - 1] + std::min(leftMinMaxMonRmq.getMin(), leftMinMaxStackRmq.getMin());

        ans = std::min(ans, currAnsNone);
        ans = std::min(ans, currAnsMin);
        ans = std::min(ans, currAnsMax);
        ans = std::min(ans, currAnsMinMax);
        ans = std::min(ans, x[n - 1] + leftMinMax[r]);
    }
}

int main()
{
    std::ios::sync_with_stdio(false);

    input();
    solve();
    output();

    return 0;
}
