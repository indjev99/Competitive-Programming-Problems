#include <vector>
#include <utility>
#include <functional>
#include <cassert>
#include <random>

using namespace std;

#define all(x) (x).begin(), (x).end()

const int N = 200;
const int LEN_HASH = 20;

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

    for(int add = 20; add >= 0; --add) {
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

        // data_with_hash = random_transform(data_with_hash);

        edges.push_back({start, node_ptr});

        int last_node[2]{};
        last_node[0] = last_node[1] = node_ptr;
        ++node_ptr;

        int run_1_len = 0;
        int first_attached = -1;
        for (int i = 0; i < data_with_hash.size(); ++i) {
            if (data_with_hash[i]) {
                edges.push_back({last_node[1], node_ptr});
                last_node[1] = node_ptr;

                ++run_1_len;
                if (run_1_len == 1) {
                    first_attached = node_ptr;
                }
            }
            else {
                if (run_1_len && i + 1 < data_with_hash.size()) {
                    if (!data_with_hash[i + 1]) {
                        edges.push_back({last_node[0], node_ptr++});
                    }
                    i += 1;
                }
                /*if (run_1_len >= 2 && i + 2 < data_with_hash.size()) {
                    if(data_with_hash[i + 1] && data_with_hash[i + 2]) {
                        edges.push_back({first_attached, node_ptr++});
                    }
                    i += 2;
                }*/

                edges.push_back({last_node[0], node_ptr});
                last_node[0] = last_node[1] = node_ptr;

                run_1_len = 0;
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

        bool one_one = false;
        function<int(int, int)> get_len = [&](int curr, int par) {
            int len = 1;
            int cnt_real_adj = 0;

            for(int to: adj[curr]) {
                if (to == par) {
                    continue;
                }

                ++cnt_real_adj;
                len = max(len, get_len(to, curr) + 1);
            }

            if (cnt_real_adj >= 2) {
                one_one = true;
            }
            return len;
        };

        vector<int> lens;
        for (int to: adj[x]) {
            if (to == prv || to == nxt) {
                continue;
            }

            lens.push_back(get_len(to, x));
        }
        
        if (!lens.empty()) {
            int len = *std::max_element(lens.begin(), lens.end());
            while (len--) {
                data.push_back(true);
            }
            data.push_back(false);
            data.push_back(lens.size() < 2);

            if (lens.size() == 3) {
                return {};
            }
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
    if (data.size() >= 1) {
        data.pop_back();
        if (verify(data)) {
            return {true, data};
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