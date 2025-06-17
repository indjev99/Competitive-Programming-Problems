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

struct Edge {
    int u, v;
    int idx;
    Edge(int _u, int _v, int _idx) : u(_u), v(_v), idx(_idx) {}
};

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

int euler_color(
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

    auto [subgraphs_0, subgraphs_1] = partition_edges_euler(edges, vers, n, m);

    int color_num = 0;
    if(!subgraphs_0.empty()) {
        color_num += euler_color(subgraphs_0, n, m, answer);
    }
    if(!subgraphs_1.empty()) {
        color_num += euler_color(subgraphs_1, n, m, answer);
    }
    return color_num;
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
    euler_color(edges, n, m, ans);
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
