#include <bits/stdc++.h>
#include "subway.h"
// #include "grader.cpp"
// #include <coding_library/graph/hopcroft_karp.hpp>

using namespace std;

template<typename T1, typename T2>
ostream& operator<<(ostream& out, const pair<T1, T2>& x) {
    return out << x.first << ' ' << x.second;
}

template<typename T1, typename T2>
istream& operator>>(istream& in, pair<T1, T2>& x) {
    return in >> x.first >> x.second;
}

template<typename T>
istream& operator>>(istream& in, vector<T>& a) {
    for(auto& x: a) {
        in >> x;
    }
    return in;
};

template<typename T>
ostream& operator<<(ostream& out, const vector<T>& a) {
    for(auto x: a) {
        out << x << ' ';
    }
    return out;
};

class HopcroftKarp {
  private:
    int n, m;
    vector<int> dist;

    bool bfs() {
        queue<int> q;
        dist.assign(n, -1);
        for(int u = 0; u < n; u++) {
            if(inv_match[u] == -1) {
                dist[u] = 0;
                q.push(u);
            }
        }

        bool found = false;
        while(!q.empty()) {
            int u = q.front();
            q.pop();
            for(int v: adj[u]) {
                int m = match[v];
                if(m == -1) {
                    found = true;
                } else if(dist[m] == -1) {
                    dist[m] = dist[u] + 1;
                    q.push(m);
                }
            }
        }

        return found;
    }

    bool dfs(int u) {
        for(int v: adj[u]) {
            int m = match[v];
            if(m == -1 || (dist[m] == dist[u] + 1 && dfs(m))) {
                inv_match[u] = v;
                match[v] = u;
                return true;
            }
        }
        dist[u] = -1;
        return false;
    }

  public:
    vector<int> match, inv_match;
    vector<vector<int>> adj;

    HopcroftKarp(int _n, int _m = -1) : n(_n), m(_m == -1 ? _n : _m) {
        adj.assign(n, vector<int>());
        clear(false);
    }

    void clear(bool clear_adj = true) {
        match.assign(m, -1);
        inv_match.assign(n, -1);
        if(clear_adj) {
            adj.assign(n, vector<int>());
        }
    }

    void add_edge(int u, int v) { adj[u].push_back(v); }

    int max_matching(bool shuffle_edges = false) {
        if(shuffle_edges) {
            for(int i = 0; i < n; i++) {
                shuffle(
                    adj[i].begin(), adj[i].end(),
                    mt19937(
                        chrono::steady_clock::now().time_since_epoch().count()
                    )
                );
            }
        }

        int ans = 0;
        while(bfs()) {
            for(int u = 0; u < n; u++) {
                if(inv_match[u] == -1 && dfs(u)) {
                    ans++;
                }
            }
        }
        return ans;
    }

    vector<pair<int, int>> get_matching() {
        vector<pair<int, int>> matches;
        for(int u = 0; u < n; u++) {
            if(inv_match[u] != -1) {
                matches.emplace_back(u, inv_match[u]);
            }
        }
        return matches;
    }

    pair<vector<int>, vector<int>> minimum_vertex_cover() {
        vector<int> left_cover, right_cover;
        bfs();

        for(int u = 0; u < n; u++) {
            if(dist[u] == -1) {
                left_cover.push_back(u);
            }
        }

        for(int v = 0; v < m; v++) {
            if(match[v] != -1 && dist[match[v]] != -1) {
                right_cover.push_back(v);
            }
        }

        return {left_cover, right_cover};
    }
};

using BipartiteMatching = HopcroftKarp;

bool assign_roads(int n, int m, vector<int> a, vector<int> b) {
    int d = m / n;
    vector<int> covered(m, false);
    for(int i = 0; i < d; i++) {
        BipartiteMatching bm(n, n);
        for(int j = 0; j < m; j++) {
            if(!covered[j]) {
                bm.add_edge(a[j], b[j]);
            }
        }

        int max_match = bm.max_matching();
        if(max_match == n) {
            vector<pair<int, int>> matches = bm.get_matching();
            set<pair<int, int>> to_remove;
            for(auto [u, v]: matches) {
                to_remove.insert({u, v});
            }

            vector<int> assigned;
            for(int j = 0; j < m; j++) {
                if(covered[j]) {
                    continue;
                }

                auto it = to_remove.find({a[j], b[j]});
                if(it != to_remove.end()) {
                    covered[j] = true;
                    to_remove.erase(it);
                    assigned.push_back(j);
                }
            }
            answer(i, assigned);

            assert(to_remove.empty());
        } else {
            return false;
        }
    }

    return true;
}
