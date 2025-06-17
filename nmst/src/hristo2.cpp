#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>


struct Edge {
	unsigned tag, a, b;
};


static unsigned gr_total;
static uint8_t gr_comp[32];

static void gr_solve(const Edge *begin, const Edge *end, size_t count) {
	// How many ways are there to pick exactly `count` edges from `begin` to
	// `end` without creating any cycles?
	if (count == 0) {
		gr_total++;
		return;
	}
	count--;
	while (begin < end - count) {
		size_t a = begin->a;
		size_t b = begin->b;
		while (gr_comp[a] != a) a = gr_comp[a];
		while (gr_comp[b] != b) b = gr_comp[b];
		begin++;
		if (a == b) {
			// skip
			continue;
		}
		gr_comp[b] = a;
		gr_solve(begin, end, count);
		gr_comp[b] = b;
	}
}


// (parent, rank) pairs
static std::pair<unsigned, unsigned> uf_parent[1 << 17];

static void uf_init(size_t n) {
	for (size_t i = 0; i < n; i++) {
		uf_parent[i] = std::make_pair(i, 0);
	}
}

static size_t uf_find(size_t i) {
	// Step 1: find root
	size_t j = i;
	while (true) {
		size_t k = uf_parent[j].first;
		if (k == j) break;
		j = k;
	}
	// Step 2: update path
	while(i != j) {
		size_t k = uf_parent[i].first;
		uf_parent[i].first = j;
		i = k;
	}
	return j;
}

static bool uf_union(size_t a, size_t b) {
	a = uf_find(a);
	b = uf_find(b);
	if (a == b) return false;
	if (uf_parent[a].second < uf_parent[b].second) {
		std::swap(a, b);
	} else if (uf_parent[a].second == uf_parent[b].second) {
		uf_parent[a].second++;
	}
	uf_parent[b].first = a;
	return true;
}

inline bool operator<(Edge a, Edge b) noexcept {
	return a.tag < b.tag;
}

static Edge *edge_group(Edge *begin, Edge *end) {
	if (begin == end) {
		return nullptr;
	}
	unsigned tag = begin->tag;
	do {
		begin++;
	} while (begin < end && begin->tag == tag);
	return begin;
}


static unsigned part_vert[32];
static size_t part_n_vert;
static size_t part_mark(unsigned u) {
	for (size_t i = 0; i < part_n_vert; i++) {
		if (part_vert[i] == u) {
			return i;
		}
	}
	size_t i = part_n_vert++;
	assert(i < 32);
	part_vert[i] = u;
	return i;
}

static unsigned part_solve(Edge *begin, Edge *end) {
	// First update the edges to point to the input union-find components.
	for (Edge *e = begin; e < end; e++) {
		e->a = uf_find(e->a);
		e->b = uf_find(e->b);
		if (e->a == e->b) {
			// Loop edge. Remove.
			*e = *begin++;
		}
		// Do not merge components -- this will mess up the remaining edges.
	}

	part_n_vert = 0;
	size_t count = 0;
	// Instead, apply the edges now. Also count how many edges we need.
	for (Edge *e = begin; e < end; e++) {
		count += uf_union(e->a, e->b);
		e->a = part_mark(e->a);
		e->b = part_mark(e->b);
	}

	for (size_t i = 0; i < part_n_vert; i++) {
		gr_comp[i] = i;
	}
	gr_total = 0;
	gr_solve(begin, end, count);
	return gr_total;
}

static Edge edges[1 << 20];
int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

	size_t n, m;
	std::cin >> n >> m;
	for (size_t i = 0; i < m; i++) {
		unsigned a, b, c;
		std::cin >> a >> b >> c;
		assert(1 <= a && a <= n);
		assert(1 <= b && b <= n);
		assert(a < b);
		edges[i] = Edge { c, a - 1, b - 1 };
	}
	assert(std::cin);

	Edge *begin = edges;
	Edge *end = begin + m;
	std::sort(begin, end);

	unsigned answer = 1;

	uf_init(n);
	while (Edge *mid = edge_group(begin, end)) {
		answer = (unsigned long long)answer * part_solve(begin, mid) % 1000000007;
		begin = mid;
	}

	std::cout << answer << '\n';

	return 0;
}
