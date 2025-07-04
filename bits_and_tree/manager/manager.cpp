#include "manager.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include <utility>
#include <functional>

const std::string PASSWORD = "bfb2kjrb30ybk234m2b3";
const std::string PASSWORD2 = "inv";

const int N = 200;
const int PROCESSES = N + 1;

std::mt19937 random_gen(9489452);

using namespace std;

char** fds;

void finish()
{
    for (int i = 0; i < 2 * PROCESSES; i++)
    {
        close(atoi(fds[i]));
    }

    exit(0);
}

void myAssert(bool cond, std::string msg)
{
    if (cond) return;

    std::cout << -1 << std::endl;
    std::cout << msg << std::endl;

    finish();
}

int runDecoder(int index_process, int node, std::vector<std::pair<int, int>> edges, const std::string &message) {
    int n = N;

    edges.push_back({node, n});

    std::vector<int> permutation(n + 1);
    std::iota(permutation.begin(), permutation.end(), 0);
    std::shuffle(permutation.begin(), permutation.end(), random_gen);

    for (auto &[u, v]: edges) {
        if (random_gen() % 2) {
            swap(u, v);
        }
    }

    std::shuffle(edges.begin(), edges.end(), random_gen);

    out(index_process) << PASSWORD2 << "\n";

    out(index_process) << n << "\n";
    for (auto [u, v]: edges){
        out(index_process) << permutation[u] << " " << permutation[v] << "\n";
    }
    out(index_process) << std::flush;

    std::string pass;
    in(index_process) >> pass;
    myAssert(pass == PASSWORD, "Your program wrote to stdout."); 

    int num_decoded_bits;
    in(index_process) >> num_decoded_bits;

    std::string decoded_message = "";
    
    if (num_decoded_bits > 0) {
        in(index_process) >> decoded_message;
    }

    int correctly_guessed_bits = 0;
    for (int i = 0; i < std::min(decoded_message.size(), message.size()); ++i) {
        if (decoded_message[i] != message[i]) {
            break;
        }
        ++correctly_guessed_bits;
    }

    return correctly_guessed_bits;
}

bool is_tree(const std::vector<std::pair<int, int>> edges) {
    int n = N;
    if ((int)edges.size() != n - 1) {
        return false;
    }

    std::vector<bool> visited(n, false);
    std::vector<std::vector<int>> adj(n);

    for (const auto &[u, v]: edges) {
        if (u < 0 || v < 0 || u >= n || v >= n) {
            return false;
        }
        if (u == v) { 
            return false; 
        }

        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    function<void(int)> dfs = [&](int node) {
        visited[node] = true;
        for (int to: adj[node]) {
            if (visited[to]) { continue; }
            dfs(to);
        }
    };

    dfs(0);

    for(int node = 0; node < n; ++node) {
        if (!visited[node]) {
            return false;
        }
    }

    return true;
}

int runTest()
{
    int n;
    std::cin >> n;

    std::string bits;
    std::cin >> bits;

    out(0) << PASSWORD2 << "\n";

    out(0) << n << "\n";
    out(0) << bits << std::endl;

    std::string pass;
    in(0) >> pass;
    myAssert(pass == PASSWORD, "Your program wrote to stdout.");

    std::vector<std::pair<int, int>> edges;
    int len_edges;
    in(0) >> len_edges;
    for (int edge = 0; edge < len_edges; ++edge) {
        int u, v;
        in(0) >> u >> v;
        edges.push_back({u, v});
    }

    myAssert(is_tree(edges), "The given edges don't form a valid tree with N nodes.");

    int worst_score = 10 * N;

    vector<int> perm(n);
    iota(perm.begin(), perm.end(), 0);
    shuffle(perm.begin(), perm.end(), random_gen);

    for (int node = 0; node < n; ++node) {
        worst_score = std::min(worst_score, runDecoder(perm[node] + 1, node, edges, bits));
    }
    return worst_score;
}

int main(int argc, char* argv[])
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    fds = argv + 1;

    fd_to_stream(fds, 2 * PROCESSES);

    out(0) << 0 << std::endl;
    for (int node = 0; node < N; ++node) {
        out(node + 1) << 1 << std::endl;
    }

    int t, RANDOM_SEED;
    std::cin >> t >> RANDOM_SEED;
    random_gen = std::mt19937(RANDOM_SEED);
    for (int i = 0; i < N + 1; ++i) {
        out(i) << t << std::endl;
    }

    int min_bits = 10 * N;
    for (int test = 1; test <= t; ++test) {
        int correctly_guessed_bits = runTest();
        min_bits = std::min(min_bits, correctly_guessed_bits);
    }
    
    std::cout << min_bits  << std::endl;

    finish();

    return 0;
}
