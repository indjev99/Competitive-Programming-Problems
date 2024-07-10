#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <cassert>
#include <vector>
#include <stack>
#include <queue>
#include <set>
#include <map>

typedef long long llong;
const int MAXN = 1500000 + 10;
const int INF  = 1e9;

int n;
struct Fenwick
{   
    int tree[2 * MAXN];
    void reset()
    {
        std::fill(tree, tree + 2 * n + 2, 0);
    }

    void update(int idx)
    {
        idx += n + 1;
        for (; idx <= 2 * n + 1 ; idx += idx & (-idx))
        {
            tree[idx]++;
        }
    }

    int query(int idx)
    {
        idx += n + 1;
        int res = 0;
        for (; idx ; idx -= idx & (-idx))
        {
            res += tree[idx];
        }

        return res;
    }
};

Fenwick tree;
std::vector <std::pair <int,int>> queries[MAXN];
int left[MAXN];
int right[MAXN];
int sz[MAXN];
char s[MAXN];
int p[MAXN];

void dfs(int node)
{
    sz[node] = 1;
    if (left[node] != 0)
    {
        dfs(left[node]);
        sz[node] += sz[left[node]];
    }

    if (right[node] != 0)
    {
        dfs(right[node]);
        sz[node] += sz[right[node]];
    }
}

int perm[MAXN];
void solve()
{
    for (int i = 1 ; i <= n ; ++i)
    {
        p[i] = p[i - 1] + (s[i] == '(' ? 1 : -1);
    }

    int root;
    std::stack <int> st;
    for (int i = 1 ; i <= n ; ++i)
    {
        int last = -1;
        while (st.size() && p[i] > p[st.top()])
        {
            last = st.top();
            st.pop();
        }

        if (last != -1)
        {
            left[i] = last;
        }

        if (st.size())
        {
            right[st.top()] = i;
        } else
        {
            root = i;
        }

        st.push(i);
    }

    dfs(root);
    llong ans = 0;
    int max = 0;
    for (int i = 1 ; i <= n ; ++i)
    {
        max = std::max(max, p[i]);
        ans += (max > p[i]);
    }

    for (int i = 1 ; i <= n ; ++i)
    {
        if (sz[left[i]] <= sz[right[i]])
        {
            for (int j = i - sz[left[i]]; j < i ; ++j)
            {
                queries[p[i] - p[j]].push_back({i, i + sz[right[i]]});
            }
        } else
        {
            for (int j = i + 1 ; j <= i + sz[right[i]] ; ++j)
            {
                queries[p[i] - p[j]].push_back({i - sz[left[i]] - 1, i - 1});
            }
        }
    }

    std::iota(perm + 1, perm + 1 + n, 1);
    std::sort(perm + 1, perm + 1 + n, [&](int x, int y)
    {
        return p[x] < p[y];
    });

    int ptr = 1;
    for (int value = 0 ; value <= n ; ++value)
    {
        while (ptr <= n && p[perm[ptr]] < value)
        {
            tree.update(perm[ptr]);
            ptr++;
        }


        for (const auto &[l, r] : queries[value])
        {
            ans += tree.query(r) - tree.query(l);
        }
    }

    std::cout << 1LL * n * (n + 1) / 2 - ans << '\n';
}

void input()
{
    std::cin >> n;
    std::cin >> s + 1;
}

void fastIOI()
{ 
    std::ios_base :: sync_with_stdio(0);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);
}

int main()
{
    fastIOI();
    input();
    solve();

    return 0;
}
