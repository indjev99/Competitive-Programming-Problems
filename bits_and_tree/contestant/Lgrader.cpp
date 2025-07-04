#include "bits_and_tree.h"

#include <iostream>
#include <algorithm>
#include <random>
#include <cassert>

using namespace std;

static void dfs(int u, vector<bool> &vis, const vector<vector<int>> &adj) {
    vis[u] = true;
    
    for (int to: adj[u]) {
        if (vis[to]) {
            continue;
        }

        dfs(to, vis, adj);
    }
}

static bool forms_tree(vector<pair<int, int>> edges, int n) {
    vector<vector<int>> adj(n);
    for (auto [u, v]: edges) {
    
        if (u < 0 || v < 0 || u >= n || v >= n) {
            return false;
        }
        if (u == v) {
            return false;
        }

        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    vector<bool> vis(n, false);

    dfs(0, vis, adj);

    for (int i = 0; i < n; ++i) {
        if (!vis[i]) {
            return false;
        }
    }

    return true;
}

const int RANDOM_SEED = 0;
mt19937 random_generator(RANDOM_SEED);

static vector<pair<int, int>> corrupt(vector<pair<int, int>> edges, int n, int node_to_attach_to) {
    edges.push_back({node_to_attach_to, n});

    vector<int> permutation(n + 1);
    for (int i = 0; i <= n; ++i){
        permutation[i] = i;
    }

    shuffle(permutation.begin(), permutation.end(), random_generator);
    shuffle(edges.begin(), edges.end(), random_generator);

    for (auto &[u, v]: edges) {
        u = permutation[u];
        v = permutation[v];

        if (random_generator() % 2) {
            swap(u, v);
        }
    }

    return edges;
}

static int runTest() {
    int n;
    cout << "n: ";
    cin >> n;

    int data_length;
    cout << "data_length: ";
    cin >> data_length;

    vector<bool> data(data_length);
    cout << "data: ";
    for (int i = 0; i < data_length; ++i) {
        char bit;
        cin >> bit;

        data[i] = bit == '1';
    }

    vector<pair<int, int>> tree_edges = encode(n, data);

    assert(forms_tree(tree_edges, n));

    int min_prefix = 1e9;

    for (int node_to_attach_to = 0; node_to_attach_to < n; ++node_to_attach_to) {
        vector<pair<int, int>> corrupted_edges = corrupt(tree_edges, n, node_to_attach_to);

        vector<bool> decoded_data = decode(n, corrupted_edges);

        int correct_prefix = 0;
        while (correct_prefix < (int) std::min(decoded_data.size(), data.size())
            && decoded_data[correct_prefix] == data[correct_prefix]) {
            ++correct_prefix;
        }
        min_prefix = min(min_prefix, correct_prefix);
    }

    return min_prefix;
}

int main() {
    cout << "t: ";
    int t;
    cin >> t;

    int min_bits_guessed = 1e9;
    for (int i = 0; i < t; i++) {
        int bits = runTest();
        cerr << "Guessed " << bits << " bits correctly in test #" << i << endl;

        min_bits_guessed = min(min_bits_guessed, bits);
    }
    cerr << endl;
    cerr << "Guessed a minimum of " << min_bits_guessed << " correctly" << endl;

    return 0;
}