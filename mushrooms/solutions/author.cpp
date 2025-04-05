#include <bits/stdc++.h>

const uint32_t MAX_N = 200000 + 10;
const uint32_t MAX_M = 300000 + 10;
const uint32_t MAX_Q = 300000 + 10;
const uint32_t SQRT = 300;

std::vector<int> st[MAX_M];
std::unordered_map<int, uint64_t> light[MAX_N];
std::unordered_set<uint64_t> heavy[MAX_N];
std::vector<int> set[MAX_N];
int n, m, q;

uint64_t solve(int a, int b) {
	if (a > b) { std::swap(a, b); }
	uint64_t ret = 0;

	if (light[a].find(b) != light[a].end()) {
		ret += light[a][b];
	}

	if (heavy[a].size() < heavy[b].size()) {
		for (const auto it : heavy[a]) {
			if (heavy[b].find(it) != heavy[b].end()) {
				ret += it;
			}
		}
	} else {
		for (const auto it : heavy[b]) {
			if (heavy[a].find(it) != heavy[a].end()) {
				ret += it;
			}
		}
	}

	return ret;
}

signed main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);

	std::cin >> n >> m;
	for (int i = 1; i <= m; i ++) {
		int k;
		std::cin >> k;

		set[i].resize(k);
		for (int j = 0; j < k; j ++) {
			std::cin >> set[i][j];
		}
		std::sort(set[i].begin(), set[i].end());

		if (k >= SQRT) {
			for (const auto it : set[i]) {
				heavy[it].insert(i);
			}
		} else {
			for (size_t l = 0; l < k; l ++) {
				for (size_t r = l + 1; r < k; r ++) {
					light[set[i][l]][set[i][r]] += i;
				}
			}
		}
	}

	std::cin >> q;
	while (q --) {
		int a, b;
		std::cin >> a >> b;
		std::cout << solve(a, b) << std::endl;
	}

	return 0;
}
