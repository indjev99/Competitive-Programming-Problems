#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <cstdint>


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

static size_t on_line = 1;
static size_t on_col = 1;
static bool on_nl = false;

static char read_char() {
	if (on_nl) {
		on_line++;
		on_col = 0;
		on_nl = false;
	}
	char c = std::cin.get();
	if (!std::cin) {
		std::cerr << "Read error on line " << on_line<< '\n';
		abort();
	}
	if (c == '\n') {
		on_nl = true;
	}
	on_col++;
	return c;
}

static void read_expect(char x) {
	if (read_char() == x) {
		return;
	}
	std::cerr << "Unexpected character on " << on_line << ':' << on_col << '\n';
	abort();
}

static uint64_t read_num(char term, uint64_t lbound, uint64_t ubound) {
	char c = read_char();
	if (c < '0' || c > '9') {
		std::cerr << "Expected digit on " << on_line << ':' << on_col << '\n';
		abort();
	}
	uint64_t r = c - '0';
	if (c == '0') {
		read_expect(term);
	} else {
		while (true) {
			c = read_char();
			if (c == term) break;
			if (c < '0' || c > '9') {
				std::cerr << "Expected digit on " << on_line << ':' << on_col << '\n';
				abort();
			}
			r = 10 * r + (c - '0');
			if (r > ubound) {
				std::cerr << "Number to large " << on_line << ':' << on_col << '\n';
				abort();
			}
		}
	}
	if (r > ubound) {
		std::cerr << "Number to large on " << on_line << ':' << on_col << '\n';
		abort();
	}
	if (r < lbound) {
		std::cerr << "Number to small on " << on_line << ':' << on_col << '\n';
		abort();
	}
	return r;
}

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

	size_t n, m;
	n = read_num(' ', 1, 100000);
	m = read_num('\n', n - 1, 200000);

	std::unordered_set<uint64_t> edges;
	std::unordered_map<unsigned, unsigned> costs;

	uf_init(n);
	for (size_t i = 0; i < m; i++) {
		unsigned a, b, c;
		a = read_num(' ', 1, n);
		b = read_num(' ', a + 1, n);
		c = read_num('\n', 0, 1000000);
		if(!edges.emplace((uint64_t)a * n + b).second) {
			std::cerr << "Duplicate edge " << a << ' ' << b << '\n';
			// abort();
		}
		uf_union(a-1, b-1);
		costs[c]++;
	}

	size_t c = uf_find(0);
	for (size_t i = 1; i < n; i++) {
		if (uf_find(i) != c) {
			std::cerr << "Graph not connected\n";
			abort();
		}
	}

	size_t maxr = 1;
	for (auto [c, r]: costs) {
		if (r > 16) {
			std::cerr << "Too many edges with cost " << c << '\n';
			abort();
		}
		if (r > maxr) maxr = r;
	}

	std::cin.get();
	if (!std::cin.eof()) {
		std::cerr << "Expected EOF\n";
		abort();
	}

	std::cout << "R=" << maxr << '\n';

	return 0;
}
