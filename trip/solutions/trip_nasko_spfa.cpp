#include <bits/stdc++.h>

typedef long long ll;

const ll MAX_N = 1e5 + 10;
std::vector<std::pair<ll, ll>> edges[MAX_N];
ll col[MAX_N];
std::vector<ll> good[MAX_N];
ll dist[MAX_N], act[MAX_N], cnt[MAX_N];
ll n, m;

bool dfs(const int x) {
    if (col[x] == 1) {
        return true;
    }
    if (col[x] == 2) {
        return false;
    }

    col[x] = 1;

    for (const auto it : good[x]) {
        if(dfs(it)) {
            return true;
        }
    }

    col[x] = 2;
    return false;
}

signed main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::cin >> n >> m;
    for (ll i = 0; i < m; i ++) {
        ll a, b, c;
        std::cin >> a >> b >> c;
        edges[a].emplace_back(b, c);
    }

    std::queue<ll> q;
    for (ll i = 0; i < n; i ++) {
        q.push(i);
        dist[i] = 0;
        act[i] = true;
        cnt[i] = 0;
    }
    while (!q.empty()) {
        const auto curr = q.front(); q.pop();
        act[curr] = false;
        cnt[curr] ++;

        if (cnt[curr] >= n + 1) {
            std::cout << 1 << std::endl;
            return 0;
        }

        for (const auto& it : edges[curr]) {
            if (it.second + dist[curr] < dist[it.first]) {
                dist[it.first] = it.second + dist[curr];
                if (!act[it.first]) {
                    act[it.first] = true;
                    q.push(it.first);
                }
            }
        }
    }

    for (ll i = 0; i < n; i ++) {
        for (const auto& it : edges[i]) {
            if (dist[i] + it.second == dist[it.first]) {
                good[i].push_back(it.first);
            }
        }
    }

    for (ll i = 0; i < n; i ++) {
        if (col[i] == 0 && dfs(i)) {
            std::cout << 1 << std::endl;
            return 0;
        }
    }

    std::cout << 0 << std::endl;
}
