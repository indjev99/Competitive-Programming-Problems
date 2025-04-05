#include <bits/stdc++.h>

const int MAX_N = 2e5 + 10;
const int MAX_SUM = 1e6 + 10;

std::vector<int> st[MAX_N];
std::unordered_set<int> light[MAX_N], heavy[MAX_N];
std::vector<int> set[MAX_N];
int n, m, q;

uint32_t solve(const int a, const int b) {
	uint32_t ret = 0;
	for (int i = 1; i <= m; i ++) {
		bool has_a = false;
		bool has_b = false;
		for (const auto it : set[i]) {
			if (a == it) { has_a = true; }
			if (b == it) { has_b = true; }
		}
		if (has_a && has_b) {
			ret += i;
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
	}

	std::cin >> q;
	while (q --) {
		int a, b;
		std::cin >> a >> b;
		std::cout << solve(a, b) << "\n";
	}

	return 0;
}
