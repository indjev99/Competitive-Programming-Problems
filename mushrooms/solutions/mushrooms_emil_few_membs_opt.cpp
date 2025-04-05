#include <iostream>
#include <vector>
#include <unordered_set>
#include <cassert>

const int MAX_N = 1e5;
const int MAX_M = 1e5;
const int MIN_K = 2;
const int MAX_K = 1e5;
const int MAX_K_SUM = 1e5;
const int MAX_MEMBS = 100;
const int MAX_Q = 1e5;

int n, m, q;
std::vector<int> sets[MAX_M];

void input()
{
    std::cin >> n >> m;

    assert(n >= 0);
    assert(n <= MAX_N);

    assert(m >= 0);
    assert(m <= MAX_M);

    int kSum = 0;
    for (int i = 0; i < m; i++)
    {
        int k;
        std::cin >> k;

        assert(k >= MIN_K);
        assert(k <= MAX_K);

        kSum += k;

        assert(kSum <= MAX_K_SUM);

        for (int j = 0; j < k; j++)
        {
            int elem;
            std::cin >> elem;

            assert(elem >= 1);
            assert(elem <= n);

            sets[i].push_back(elem - 1);
        }
    }

    std::cin >> q;

    assert(q >= 0);
    assert(q <= MAX_Q);
}

std::vector<int> membs[MAX_N];
std::unordered_set<int> membsS[MAX_N];

void init()
{
    for (int i = 0; i < m; i++)
    {
        for (int e : sets[i])
        {
            membs[e].push_back(i);
            membsS[e].insert(i);
            assert((int) membs[e].size() <= MAX_MEMBS);
        }
    }
}

int solve(int a, int b)
{
    int ans = 0;

    if (membs[a].size() > membs[b].size()) std::swap(a, b);

    for (int i : membs[a])
    {
        if (!membsS[b].count(i)) continue;

        ans += i + 1;
    }

    return ans;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    input();

    init();

    for (int i = 0; i < q; i++)
    {
        int a, b;
        std::cin >> a >> b;

        assert(a >= 1);
        assert(a <= n);

        assert(b >= 1);
        assert(b <= n);

        std::cout << solve(a - 1, b - 1) << "\n";
    }

    return 0;
}
