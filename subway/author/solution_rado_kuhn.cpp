#include <bits/stdc++.h>
#include "subway.h"
#include <vector>
// #include "grader.cpp"
// #include <coding_library/graph/hopcroft_karp.hpp>
// #include <coding_library/graph/bipartite_matching.hpp>

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

class BipartiteMatching {
  private:
    int n, m, visited_timer;
    vector<int> visited;

    bool try_kuhn(int u) {
        if(visited[u] == visited_timer) {
            return false;
        }

        visited[u] = visited_timer;
        for(int v: adj[u]) {
            if(match[v] == -1 || try_kuhn(match[v])) {
                match[v] = u;
                inv_match[u] = v;
                return true;
            }
        }
        return false;
    }

    int pre_match() {
        int matching_size = 0;
        for(int u = 0; u < n; u++) {
            if(inv_match[u] == -1) {
                for(int v: adj[u]) {
                    if(match[v] == -1) {
                        matching_size++;
                        match[v] = u;
                        inv_match[u] = v;
                        break;
                    }
                }
            }
        }

        return matching_size;
    }

  public:
    vector<int> match, inv_match;
    vector<vector<int>> adj;

    BipartiteMatching(int _n, int _m = -1) : n(_n), m(_m == -1 ? _n : _m) {
        adj.assign(n, vector<int>());
        clear(false);
    }

    void clear(bool clear_adj = true) {
        match.assign(m, -1);
        inv_match.assign(n, -1);

        visited_timer = 0;
        visited.assign(n, 0);
        if(clear_adj) {
            adj.assign(n, vector<int>());
        }
    }

    void add_edge(int u, int v) { adj[u].push_back(v); }

    bool match_vertex(int u) {
        if(inv_match[u] != -1) {
            return false;
        }

        visited_timer++;
        return try_kuhn(u);
    }

    int max_matching(bool shuffle_edges = false, bool pre_matching = false) {
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
        if(pre_matching) {
            ans += pre_match();
        }

        for(int i = 0; i < n; i++) {
            ans += match_vertex(i);
        }

        return ans;
    }

    vector<pair<int, int>> get_matching() {
        vector<pair<int, int>> res;
        for(int i = 0; i < m; i++) {
            if(match[i] != -1) {
                res.emplace_back(match[i], i);
            }
        }
        return res;
    }
};

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
