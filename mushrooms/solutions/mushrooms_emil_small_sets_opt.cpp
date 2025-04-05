#include <iostream>
#include <vector>
#include <unordered_map>
#include <cassert>

const int MAX_N = 1e5;
const int MAX_M = 1e5;
const int MIN_K = 2;
const int MAX_K = 100;
const int MAX_K_SUM = 1e5;
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

std::unordered_map<int, int> adjSum[MAX_N];

void init()
{
    for (int i = 0; i < m; i++)
    {
        for (int j1 = 0; j1 < (int) sets[i].size(); j1++)
        {
            for (int j2 = j1 + 1; j2 < (int) sets[i].size(); j2++)
            {
                adjSum[sets[i][j1]][sets[i][j2]] += i + 1;
            }
        }
    }
}

int solve(int a, int b)
{
    int ans = 0;

    auto it1 = adjSum[a].find(b);
    auto it2 = adjSum[b].find(a);

    if (it1 != adjSum[a].end())
    {
        ans += it1->second;
    }

    if (it2 != adjSum[b].end())
    {
        ans += it2->second;
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
