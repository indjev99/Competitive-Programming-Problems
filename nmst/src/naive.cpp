#include <iostream>
#include <vector>
#include <cstdint>

struct Edge {
	size_t a, b;
	int c;
	bool picked;
};

static size_t n_vertices;
static std::vector<Edge> edges;

static size_t uf_parent[1 << 17];
static void uf_reset() {
	for (size_t i = 0; i < n_vertices; i++) {
		uf_parent[i] = i;
	}
}
static size_t uf_find(size_t i) {
	return uf_parent[i] == i ? i : uf_parent[i] = uf_find(uf_parent[i]);
}

static long long best_cost = 1LL << 62;
static uint64_t total = 0;
static void rec(size_t at, size_t n_edges) {
	if (n_edges >= n_vertices) {
		return;
	}
	if (at == edges.size()) {
		long long cost = 0;
		uf_reset();
		for (size_t i = 0; i < edges.size(); i++) {
			if (!edges[i].picked) continue;
			cost += edges[i].c;
			size_t a = uf_find(edges[i].a);
			size_t b = uf_find(edges[i].b);
			uf_parent[b] = a;
		}
		for (size_t i = 1; i < n_vertices; i++) {
			if (uf_find(i) != uf_find(0)) {
				return;
			}
		}
		if (cost > best_cost) {
			return;
		}
		if (cost < best_cost) {
			best_cost = cost;
			total = 1;
		} else {
			total++;
		}
		return;
	}

	edges[at].picked = false;
	rec(at + 1, n_edges);
	edges[at].picked = true;
	rec(at + 1, n_edges + 1);
	edges[at].picked = false;
}

int main() {
	size_t m;
	std::cin >> n_vertices >> m;

	for (size_t i = 0; i < m; i++) {
		size_t a, b;
		int c;
		std::cin >> a >> b >> c;
		a--; b--;
		edges.push_back({a, b, c, false});
	}

	rec(0, 0);
	std::cout << total << '\n';
}
