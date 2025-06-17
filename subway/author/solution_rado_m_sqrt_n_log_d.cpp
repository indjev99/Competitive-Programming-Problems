#include <bits/stdc++.h>
#include <vector>
// #include "grader.cpp"
#include "subway.h"

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

template<class T>
void make_larger_if_needed(vector<T>& v, int size) {
    if(v.size() < size) {
        v.resize(size);
    }
}

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

struct Edge {
    int u, v;
    int idx;
    Edge(int _u, int _v, int _idx) : u(_u), v(_v), idx(_idx) {}
};

vector<Edge> slow_one_colour(
    const vector<Edge>& edges, int n, int m, vector<vector<int>>& answer
) {
    BipartiteMatching bm(n, n);
    for(const auto& e: edges) {
        bm.add_edge(e.u, e.v);
    }

    int max_match = bm.max_matching();
    assert(max_match == n);

    vector<pair<int, int>> matches = bm.get_matching();
    set<pair<int, int>> to_remove;

    for(auto [u, v]: matches) {
        to_remove.insert({u, v});
    }

    vector<int> assigned;
    vector<Edge> new_edges;
    for(auto e: edges) {
        auto it = to_remove.find({e.u, e.v});
        if(it != to_remove.end()) {
            to_remove.erase(it);
            assigned.push_back(e.idx);
        } else {
            new_edges.push_back(e);
        }
    }

    answer.push_back(assigned);
    return new_edges;
}

pair<vector<Edge>, vector<Edge>> partition_edges_euler(
    const vector<Edge>& edges, const vector<int>& vers, int n, int m

) {
    static vector<vector<pair<int, int>>> adj;
    static vector<int> memory, memory_m;

    make_larger_if_needed(adj, 2 * n);
    make_larger_if_needed(memory_m, m);

    for(int v: vers) {
        adj[v].clear();
        adj[v + n].clear();
    }

    for(int ei = 0; ei < (int)edges.size(); ei++) {
        auto e = edges[ei];
        adj[e.u].push_back({e.v + n, ei});
        adj[e.v + n].push_back({e.u, ei});
        memory_m[ei] = 0;
    }

    function<void(int, vector<Edge>&, vector<Edge>&)> dfs =
        [&](int v, vector<Edge>& subgraph_0, vector<Edge>& subgraph_1) {
            while(!adj[v].empty()) {
                auto [u, ei] = adj[v].back();
                adj[v].pop_back();
                if(memory_m[ei] == 1) {
                    continue;
                }
                memory_m[ei] = 1;
                dfs(u, subgraph_0, subgraph_1);
                if(v < n) {
                    subgraph_0.push_back(edges[ei]);
                } else {
                    subgraph_1.push_back(edges[ei]);
                }
                break;
            }
        };

    vector<Edge> subgraph_0, subgraph_1;
    for(int v: vers) {
        while(!adj[v].empty()) {
            dfs(v, subgraph_0, subgraph_1);
        }
    }

    return {subgraph_0, subgraph_1};
}

int euler_colour(
    const vector<Edge>& edges, int n, int m, vector<vector<int>>& answer
) {
    static vector<int> memory;
    make_larger_if_needed(memory, n);

    vector<int> vers, _vers;
    for(auto e: edges) {
        _vers.push_back(e.u);
        _vers.push_back(e.v);
    }

    int max_degree = 0;
    for(int v: _vers) {
        memory[v] = -1;
    }

    for(int v: _vers) {
        if(memory[v] == -1) {
            vers.push_back(v);
            memory[v] = 0;
        }
    }

    for(auto e: edges) {
        memory[e.u]++;
        max_degree = max(max_degree, memory[e.u]);
    }

    if(max_degree == 0) {
        return 0;
    }

    if(max_degree == 1) {
        answer.push_back({});
        for(auto e: edges) {
            answer.back().push_back(e.idx);
        }
        return 1;
    }

    if(max_degree % 2 == 1) {
        auto subgraph = slow_one_colour(edges, n, m, answer);
        return 1 + euler_colour(subgraph, n, m, answer);
    }

    auto [subgraphs_0, subgraphs_1] = partition_edges_euler(edges, vers, n, m);

    int colour_num = 0;
    if(!subgraphs_0.empty()) {
        colour_num += euler_colour(subgraphs_0, n, m, answer);
    }
    if(!subgraphs_1.empty()) {
        colour_num += euler_colour(subgraphs_1, n, m, answer);
    }
    return colour_num;
}

bool assign_roads(int n, int m, vector<int> a, vector<int> b) {
    vector<Edge> edges;
    vector<int> degree_left(n, 0), degree_right(n, 0);
    for(int i = 0; i < m; i++) {
        edges.emplace_back(a[i], b[i], i);
        degree_left[a[i]]++;
        degree_right[b[i]]++;
    }

    if(m % n != 0) {
        return false;
    }

    int d = m / n;
    for(int i = 0; i < n; i++) {
        if(degree_left[i] != d) {
            return false;
        }
        if(degree_left[i] != d) {
            return false;
        }
    }

    vector<vector<int>> ans;
    euler_colour(edges, n, m, ans);
    if(ans.size() != d) {
        return false;
    }

    for(int i = 0; i < d; i++) {
        // cout << "Color " << i << ": ";
        // cout << ans[i] << endl;
        answer(i, ans[i]);
    }

    return true;
}
