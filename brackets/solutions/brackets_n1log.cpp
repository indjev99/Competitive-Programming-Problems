#include <iostream>
#include <cassert>

const int MAX_N = 4e6;

int n;
int vals[MAX_N];

long long ans;

void input()
{
    std::string s;

    std::cin >> n;
    std::cin >> s;

    for (int i = 0; i < n; ++i)
    {
        vals[i] = s[i] == '(' ? 1 : -1;
    }
}

void output()
{
    std::cout << ans << std::endl;
}

int currQueryPos;
int currQueryAns;
int counts[MAX_N + 1];

int query(int min, int max, int pos)
{
    if (pos < min) return 0;
    if (pos > max) pos = max;

    pos -= min - 1;

    assert(std::abs(pos - currQueryPos) <= 1);

    if (pos == currQueryPos - 1)
    {
        currQueryAns -= counts[currQueryPos];
        --currQueryPos;
    }

    if (pos == currQueryPos + 1)
    {
        ++currQueryPos;
        currQueryAns += counts[currQueryPos];
    }

    return currQueryAns;
}

void update(int min, int max, int pos)
{
    assert(pos >= min);
    assert(pos <= max);

    pos -= min - 1;

    ++counts[pos];

    if (pos <= currQueryPos) ++currQueryAns;
}

void init(int min, int max, int firstPos)
{
    if (firstPos < min) firstPos = min;
    if (firstPos > max) firstPos = max;

    firstPos -= min - 1;

    currQueryAns = 0;
    currQueryPos = firstPos;
}

void clear(int min, int max)
{
    for (int pos = 1; pos <= max - min + 1; ++pos)
    {
        counts[pos] = 0;
    }
}

int prefSums[MAX_N + 1];
int maxRPrefSums[MAX_N + 1];
int rSwitches[MAX_N];

void solveRec(int cl, int cr)
{
    if (cr <= cl) return;

    int m = (cl + cr) / 2;

    int min = prefSums[m + 1];
    int max = prefSums[m + 1];

    maxRPrefSums[m] = 0;
    for (int r = m + 1; r <= cr; ++r)
    {
        min = std::min(prefSums[r], min);
        max = std::max(prefSums[r], max);

        maxRPrefSums[r] = std::max(prefSums[r], maxRPrefSums[r - 1]);
    }

    init(min, max, -1);

    // first r s.t. maxPrefSums[r] > maxLPrefSum
    int rSwitch = m + 1;

    int maxLPrefSum = 0;
    for (int l = m; l >= cl; --l)
    {
        maxLPrefSum = std::max(prefSums[l], maxLPrefSum);

        while (rSwitch <= cr && maxRPrefSums[rSwitch] <= maxLPrefSum)
        {
            update(min, max, prefSums[rSwitch]);

            ++rSwitch;
        }

        ans += rSwitch - (m + 1) - query(min, max, maxLPrefSum - prefSums[l] - 1);

        rSwitches[l] = rSwitch;
    }

    clear(min, max);

    max = max - min;
    min = 0;

    init(min, max, prefSums[cl]);

    int rDone = cr + 1;

    for (int l = cl; l <= m; ++l)
    {
        rSwitch = rSwitches[l];

        while (rDone > rSwitch)
        {
            --rDone;
            update(min, max, maxRPrefSums[rDone] - prefSums[rDone]);
        }

        ans += query(min, max, prefSums[l]);
    }

    clear(min, max);

    solveRec(cl, m);
    solveRec(m + 1, cr);
}

void solve()
{
    ans = 0;

    prefSums[0] = 0;
    for (int i = 0; i <= n; ++i)
    {
        prefSums[i] = vals[i - 1] + prefSums[i - 1];
    }

    solveRec(0, n);
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
