#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>


struct Edge {
	unsigned tag, a, b;
};


// Neighbor set
static uint16_t gr_nmask[16];
// Set of neighbors with nedges > 1
static uint16_t gr_amask[16];
// Neighbor edge count
static uint8_t gr_nedges[16][16];

static std::pair<uint32_t, uint32_t> gr_counts[2][8945664];

[[gnu::noinline]] static uint32_t gr_solve(size_t n) {
	/*
	 * Count the number of spanning trees of the graph.
	 *
	 * With -DNDEBUG this is a leaf function.
	 */
	assert(n >= 2);
	assert(n <= 16);
	uint16_t all = (1U << n) - 1;

	/*
	 * First remove all leaves.
	 *
	 * This is done by repeatedly checking if there are vertices with exactly one
	 * neighbor. Not particularly efficient, but we don't have many vertices.
	 */
	unsigned answer = 1;
	bool progress = true;
	while (progress) {
		assert(all);
		progress = false;
		for (unsigned m = all; m; m &= m - 1) {
			size_t i = __builtin_ctz(m);
			uint32_t neighbors = gr_nmask[i] & all;
			assert(neighbors);
			if (neighbors & (neighbors - 1)) {
				continue;
			}
			// Degree-1 vertex. Remove.
			all &= ~(1U << i);
			size_t j = __builtin_ctz(neighbors);
			assert(gr_nedges[i][j]);
			answer *= gr_nedges[i][j];
			progress = true;
			if (!(all & (all - 1))) {
				// Only one vertex left.
				return answer;
			}
		}
	}

	/*
	 * Overall idea: Two players take turns building a spanning tree.
	 *
	 * First, player 1 makes the root vertex reachable. Then the two
	 * players alternate:
	 * - Player 2 picks an unopened reachable vertex.
	 * - Player 1 opens the vertex, revealing its children
	 *   (a subset of its neighbors in the original graph).
	 *
	 * Each game results in a tree. If player 2 is deterministic, then each
	 * strategy of player 1 reveals a different tree.
	 *
	 * We count the number of possible games that result in a tree covering all
	 * vertices of the graph.
	 *
	 * We store the state as a 32-bit word:
	 * - The bottom 16 bits indicate the unopened reachable vertices
	 * - The top 16 bits indicate the unreachable vertices.
	 */

	uint32_t init_state = (uint32_t)(all & -all) | ((uint32_t)(all & (all - 1)) << 16);

	auto buf_cur = gr_counts[0];
	auto p_cur = buf_cur;
	*p_cur++ = std::make_pair(init_state, answer);
	answer = 0;

	while (p_cur != buf_cur) {
		auto buf_next = buf_cur == gr_counts[0] ? gr_counts[1] : gr_counts[0];
		auto p_next = buf_next;
		for (const auto *r = buf_cur; r < p_cur; r++) {
			uint32_t r_state = r->first;
			uint32_t r_count = r->second;
			assert(r_state & 65535);
			assert(r_count > 0);

			// Player 2 picks the lowest-indexed unopened vertex.
			size_t u = __builtin_ctz(r_state);
			r_state &= r_state - 1;
			uint32_t r_next = gr_nmask[u] & (r_state >> 16);

			// Iterate over Player 1's options.
			if (r_state & 65535) {
				*p_next++ = std::make_pair(r_state, r_count);
			}
			if (r_next == 0) {
				continue;
			}
			r_next |= r_next << 16;
			for (uint32_t next = r_next; next; next = (next - 65537) & r_next) {
				uint32_t state = r_state ^ next;
				uint32_t count = r_count;
				for (unsigned m = next & gr_amask[u]; m; m &= m - 1) {
					count *= gr_nedges[u][__builtin_ctz(m)];
				}
				assert(count);

				if ((state >> 16) == 0) {
					answer += count;
					continue;
				}
				assert(state & 65535);
				*p_next++ = std::make_pair(state, count);
			}
		}
		buf_cur = buf_next;
		p_cur = p_next;
	}

	return answer;
}

inline static void gr_reset() {
	memset(gr_nmask, 0, sizeof(gr_nmask));
	memset(gr_amask, 0, sizeof(gr_amask));
	memset(gr_nedges, 0, sizeof(gr_nedges));
}

inline static void gr_edge(size_t a, size_t b) {
	assert(a != b);
	gr_nedges[a][b]++;
	gr_nedges[b][a]++;
	gr_nmask[a] |= 1U << b;
	gr_nmask[b] |= 1U << a;
	if (gr_nedges[a][b] == 2) {
		gr_amask[a] |= 1U << b;
		gr_amask[b] |= 1U << a;
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

static void uf_union(size_t a, size_t b) {
	a = uf_find(a);
	b = uf_find(b);
	if (a == b) return;
	if (uf_parent[a].second < uf_parent[b].second) {
		std::swap(a, b);
	} else if (uf_parent[a].second == uf_parent[b].second) {
		uf_parent[a].second++;
	}
	uf_parent[b].first = a;
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

static std::pair<unsigned, unsigned> part_vert[32];
static size_t part_n_vert;
static void part_mark(unsigned u) {
	for (size_t i = 0; i < part_n_vert; i++) {
		if (part_vert[i].first == u) {
			part_vert[i].second++;
			return;
		}
	}
	assert(part_n_vert < 32);
	part_vert[part_n_vert++] = std::make_pair(u, 1);
}

static size_t part_find(unsigned u) {
	for (size_t i = 0; i < part_n_vert; i++) {
		if (part_vert[i].first == u) {
			return i;
		}
	}
	abort();
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
	// Instead, apply the edges now.
	for (const Edge *e = begin; e < end; e++) {
		uf_union(e->a, e->b);
	}

	// Label edges with connected component.
	for (Edge *e = begin; e < end; e++) {
		e->tag = uf_find(e->a);
		assert(e->tag == uf_find(e->b));
	}

	// Iterate over connected components.
	std::sort(begin, end);
	unsigned answer = 1;
	while (Edge *mid = edge_group(begin, end)) {
		// Mark all relevant vertices.
		for (const Edge *e = begin; e < mid; e++) {
			part_mark(e->a);
			part_mark(e->b);
		}
		size_t n = part_n_vert;
		size_t m = mid - begin;
		assert(n >= 2);
		if (n < m + 1) {
			// Sort vertices by degree, lowest first. This should somewhat help the search.
			std::sort(part_vert, part_vert + part_n_vert, [](auto a, auto b) noexcept -> bool {
				return a.second < b.second;
			});
			for (const Edge *e = begin; e < mid; e++) {
				gr_edge(part_find(e->a), part_find(e->b));
			}
			answer *= gr_solve(n);
			gr_reset();
		} else {
			// The component is a tree. Nothing to do.
			assert(n == m + 1);
		}
		begin = mid;
		part_n_vert = 0;
	}
	return answer;
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
