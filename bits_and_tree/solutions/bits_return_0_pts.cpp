#include <vector>
#include <utility>

std::vector<std::pair<int, int>> encode(int n, std::vector<bool> data) {
    std::vector<std::pair<int, int>> edges;
    for(int i = 1; i < n; ++i){
        edges.push_back({0, i});
    }
    return edges;
}

std::vector<bool> decode(int n, std::vector<std::pair<int, int>> tree) {
    return {false};
}