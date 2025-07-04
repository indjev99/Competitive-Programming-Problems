#include <vector>
#include <utility>
#include <functional>
#include <cassert>
#include <random>
#include <iostream>

using namespace std;

#define all(x) (x).begin(), (x).end()

const int N = 200;
const int LEN_HASH = 22;

const int MAX_NODE_ITERS = 2;

vector<bool> random_transform(vector<bool> data) {
    mt19937 mt(3461);
    for (int i = 0; i < data.size(); ++i) {
        data[i] = data[i] ^ (mt() % 2);
    }
    return data;
}

std::vector<std::pair<int, int>> encode(int n, std::vector<bool> data) {
    std::vector<std::pair<int, int>> edges;

    data = random_transform(data);

    int start = 0;
    for (int i = 1; i <= 5; ++i) {
        edges.push_back({start, i});
    }

    int end = edges.size() + 1;
    for(int i = 1; i <= 4; ++i) {
        edges.push_back({end, end + i});
    }

    auto edges_save = edges;

    for(int add = 100; add >= 0; --add) {
        edges = edges_save;

        int node_ptr = edges.back().second + 1; // 12
        int len_hash = LEN_HASH;
        int len_guess = (n - node_ptr - 2) - len_hash; // n - 14 - len_hash
        len_guess += add;

        vector<bool> data_with_hash;
        for (int i = 0; i < len_guess; ++i) {
            data_with_hash.push_back(data[i]);
        }

        mt19937 mt(3);
        for(int i = 0; i < len_hash; ++i) {
            int xor_bit = 0;
            for(int j = 0; j < len_guess; ++j) {
                if (mt() % 2) {
                    xor_bit ^= data[j];
                }
            }
            data_with_hash.push_back(xor_bit);
        }

        edges.push_back({start, node_ptr});

        int last_node[2]{};
        last_node[0] = last_node[1] = node_ptr;
        ++node_ptr;

        vector<int> run_1;
        for (int i = 0; i < data_with_hash.size(); ++i) {
            if (data_with_hash[i]) {
                edges.push_back({last_node[1], node_ptr});
                last_node[1] = node_ptr;

                run_1.push_back(node_ptr);
            }
            else {
                run_1.insert(run_1.begin(), last_node[0]);
                run_1.pop_back();
                for(int node_idx = 0; node_idx < min(MAX_NODE_ITERS, (int)run_1.size()); ++node_idx) {
                    if (i + 1 < data_with_hash.size()) {
                        if (!data_with_hash[i + 1]) {
                            edges.push_back({run_1[node_idx], node_ptr++});
                        }
                        i += 1;
                    }
                }

                edges.push_back({last_node[0], node_ptr});
                last_node[0] = last_node[1] = node_ptr;

                run_1.clear();
            }

            ++node_ptr;
        }
        edges.push_back({last_node[0], end});

        if(node_ptr > n) {
            continue;
        }

        for(; node_ptr < n; ++node_ptr) {
            edges.push_back({start, node_ptr});
        }

        cerr << "add " << add << endl;
        return edges;
    }

    while (true);
    return {};
}

vector<int> get_path(vector<vector<int>> adj, int start, int end){
    vector<bool> vis(adj.size());
    vector<int> path;

    function<bool(int)> dfs = [&](int u) {
        vis[u] = true;

        if (u == end) {
            path.push_back(end);
            return true;
        }

        for (int to: adj[u]) {
            if (vis[to]) {
                continue;
            }

            if (dfs(to)) {
                path.push_back(u);
                return true;
            }
        }

        return false;
    };

    dfs(start);
    reverse(all(path));

    return path;
}

bool verify(vector<bool> data) {
    if (data.size() <= LEN_HASH + N / 2) {
        return false;
    }

    mt19937 mt(3);
    for(int i = 0; i < LEN_HASH; ++i) {
        int xor_bit = 0;
        for(int j = 0; j < (int)data.size() - LEN_HASH; ++j) {
            if (mt() % 2) {
                xor_bit ^= data[j];
            }
        }
        if (data[(int)data.size() - LEN_HASH + i] != xor_bit) {
            return false;
        }
    }
    return true;
}

vector<bool> get_data(const vector<int> &path, const vector<vector<int>> &adj) {
    vector<bool> data;

    for (int i = 1; i < (int)path.size() - 1; ++i) {
        int x = path[i];
        int prv = path[i - 1], nxt = path[i + 1];

        function<vector<int>(int, int)> get_nodes = [&](int curr, int par) {
            vector<int> nodes{curr};

            for(int to: adj[curr]) {
                if (to == par) {
                    continue;
                }

                auto cand_nodes = get_nodes(to, curr);
                if (cand_nodes.size() + 1 > nodes.size()) {
                    nodes = cand_nodes;
                    nodes.push_back(curr);
                }
            }

            return nodes;
        };

        vector<int> lens;
        vector<vector<int>> nodes;
        for (int to: adj[x]) {
            if (to == prv || to == nxt) {
                continue;
            }

            nodes.push_back(get_nodes(to, x));
            nodes.back().push_back(x);
            lens.push_back(nodes.back().size());
        }
        
        if (!lens.empty()) {
            vector<int> max_nodes = *std::max_element(nodes.begin(), nodes.end(), [&](const auto &l, const auto &r){
                return l.size() < r.size();
            });
            for (int _ = 0; _ < (int)max_nodes.size() - 1; ++_) {
                data.push_back(true);
            }
            data.push_back(false);

            // max_nodes.pop_back();

            int iter = MAX_NODE_ITERS;
            bool first = true;
            while (max_nodes.size() >= 1 && iter--) {
                if (first) {
                    data.push_back(adj[max_nodes.back()].size() < 4);
                    max_nodes.pop_back();

                    first = false;
                }
                else {
                    data.push_back(adj[max_nodes.back()].size() < 3);
                    max_nodes.pop_back();
                }
            }

            // if (lens.size() == 3) {
                // return {};
            // }
        }
        else {
            data.push_back(false);
        }
    }

    if (data.empty()) {
        return {};
    }

    data.pop_back();

    return data;
}

pair<bool, vector<bool>> try_decode(int n, vector<vector<int>> adj) {
    int start = -1, end = -1;
    for (int i = 0; i < n; ++i) {
        if(adj[i].size() >= 6) {
            start = i;
        }
        else if(adj[i].size() >= 5) {
            end = i;
        }
    }

    if (start == -1 || end == -1) {
        return {false, {}};
    }

    vector<int> path = get_path(adj, start, end);
    vector<bool> data = get_data(path, adj);

    if (verify(data)) {
        return {true, data};
    }
    
    int remove_from_back = MAX_NODE_ITERS;
    while (remove_from_back--) {
        if (data.size() >= 1) {
            data.pop_back();
            if (verify(data)) {
                return {true, data};
            }
        }
    }

    return {false, {}};
}

std::vector<bool> decode(int n, std::vector<std::pair<int, int>> tree) {
    std::vector<std::vector<int>> adj(n + 1);
    for(const auto& [u, v]: tree) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    for (int fake = 0; fake <= n; ++fake) {
        if (adj[fake].size() != 1) {
            continue;
        }

        vector<vector<int>> new_adj = adj;
        for (int i = 0; i <= n; ++i){
            if (i == fake){
                new_adj[i].clear();
            }
            else {
                auto iter = find(new_adj[i].begin(), new_adj[i].end(), fake);
                if (iter != new_adj[i].end()) {
                    new_adj[i].erase(iter);
                }
            }
        }

        if (fake != n) {
            swap(new_adj[fake], new_adj[n]);

            for (int i = 0; i < n; ++i) {
                for (int &to: new_adj[i]) {
                    if(to == n){
                        to = fake;
                    }
                }
            }
        }
        new_adj.pop_back();

        auto [successful, data] = try_decode(n, new_adj);
        if (successful) {
            return random_transform(data);
        }
    }

    assert(false);
    return {}; 
}