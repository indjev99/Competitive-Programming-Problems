#include <bits/stdc++.h>

const uint32_t MAX_N = 300000 + 10;
const uint32_t MAX_Q = 300000 + 10;

std::vector<int> st[MAX_N];
std::unordered_map<int, uint64_t> light[MAX_N];
std::unordered_set<uint64_t> heavy[MAX_N];
std::vector<int> set[MAX_N];
int n, m, q;

std::map<std::pair<int, int>, int> found;

uint64_t solve(const int a, const int b) {
	if (found.find({a, b}) != found.end()) {
		return found[{a, b}];
	} else if (found.find({b, a}) != found.end()) {
		return found[{b, a}];
	}

	int ret = 0;

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

	return found[{a, b}] = ret;
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

		for (const auto it : set[i]) {
			heavy[it].insert(i);
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
