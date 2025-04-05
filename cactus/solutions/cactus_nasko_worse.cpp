#include <bits/stdc++.h>
#include <vector>

#define NEXT ((curr >= buffer.size()) ? 1 : buffer[curr ++])

const int MAX_N = 2e4 + 10;
std::vector<int> g[MAX_N];
int n;
int d[MAX_N];

int numBits(const int mn, const int mx) {
	int ret = 0;
	while ((1ll << ret) < (mx - mn + 1)) {
		ret ++;
	}
	return ret;
}

int numBits(const int mx) {
	return numBits(0, mx);
}

void encode(std::vector<bool> &buffer, const int bits, const int num) {
	assert(num < 1 << bits && num >= 0);
	for (int i = 0; i < bits; i ++) {
		buffer.push_back(num & (1 << i));
	}
}

int decode(std::vector<bool> &buffer, const int start, const int bits) {
	int ret = 0;
	for (int i = 0; i < bits; i ++) {
		ret += ((int)buffer[start + i]) << i;
	}
	return ret;
}

int heavy[MAX_N], parent[MAX_N], back[MAX_N];

int cnt = 0;

void dfs(const int x, const int p, const int depth) {
	d[x] = depth;
	parent[x] = x;
	for (const auto it : g[x]) {
		if (it == p) { continue; }
		if (d[it] == -1) {
			dfs(it, x, depth + 1);
			if (d[parent[it]] < d[x]) {
				parent[x] = parent[it];
			}
		} else if (d[it] < d[x]) {
			parent[x] = it;
		}
	}

	std::sort(g[x].begin(), g[x].end(), [&] (const auto a, const auto b) {
		return d[parent[a]] < d[parent[b]];
	});
}

int ending = 0;

void dfsEncode(
	const int x, 
	const int p, 
	const bool up,
	std::vector<bool> &buffer,
	std::vector<int> &perm
) {
	//std::cerr << "encoding " << x << " " << parent[x] << " " << p << " " << up << std::endl;
	//std::cerr << " --------- ";
	for (const auto it : g[x]) {
		//std::cerr << it << " ";
	}
	//std::cerr << std::endl;
	perm.push_back(x);

	bool switched = false;

	for (const auto it : g[x]) {
		if (it == p) { continue; }
		ending = 0;
		buffer.push_back(0);
		if (d[it] != d[x] + 1) {
			if (d[it] < d[x]) {
				buffer.push_back(1);
				buffer.push_back(0);
			} else {
				buffer.pop_back();
			}
		} else if (d[parent[it]] < d[x]) {
			buffer.push_back(1);
			buffer.push_back(1);
			dfsEncode(it, x, up, buffer, perm);
		} else if (parent[it] == x) {
			buffer.push_back(0);
			buffer.push_back(1);
			dfsEncode(it, x, true, buffer, perm);
		} else if (switched) {
			dfsEncode(it, x, false, buffer, perm);
		} else {
			buffer.push_back(0);
			buffer.push_back(0);
			switched = true;
			dfsEncode(it, x, false, buffer, perm);
		}
	}
	buffer.push_back(1);
	ending ++;
}

int dfsDecode(
	const int p, 
	const int up,
	std::vector<bool> &buffer,
	int &curr,
	std::vector<int> &nodes,
	std::vector<int> &perm,
	std::vector<std::pair<int, int>> &ret
) {
	const int x = perm.back();
	//std::cerr << x << " " << p << " " << up << std::endl;
	nodes.push_back(perm.back());
	perm.pop_back();

	bool switched = false;

	while (NEXT != 1) {
		if (switched) {
			const auto it = dfsDecode(x, -1, buffer, curr, nodes, perm, ret);
			ret.push_back({x, it});
			//std::cerr << "got sw " << ret.back().first << " " << ret.back().second << std::endl;
			//std::cerr << x << " switched " << std::endl;
		} else if (NEXT == 0) {
			if (NEXT == 0) {
				//std::cerr << x << " 00 " << std::endl;
				const auto it = 
					dfsDecode(x, -1, buffer, curr, nodes, perm, ret);
				ret.push_back({x, it});
				//std::cerr << "got 00" << ret.back().first << " " << ret.back().second << std::endl;
				switched = true;
			} else {
				//std::cerr << x << " 01 " << std::endl;
				const auto it = 
					dfsDecode(x, x, buffer, curr, nodes, perm, ret);
				ret.push_back({x, it});
				//std::cerr << "got 01" << ret.back().first << " " << ret.back().second << std::endl;
			}
		} else {
			if (NEXT == 0) {
				//std::cerr << x << " 10 " << std::endl;
				ret.push_back({x, up});
				//std::cerr << "got 10" << ret.back().first << " " << ret.back().second << std::endl;
			} else {
				//std::cerr << x << " 11 " << std::endl;
				const auto it = 
					dfsDecode(x, up, buffer, curr, nodes, perm, ret);
				ret.push_back({x, it});
				//std::cerr << "got 11" << ret.back().first << " " << ret.back().second << std::endl;
			}
		}
	}

	return x;
}


void clear() {
	for (int i = 0; i <= n; i ++) {
		d[i] = -1;
		g[i].resize(0);
		heavy[i] = -1;
		parent[i] = -1;
	}
	cnt = 0;
}

std::vector<bool> encode(int n_, std::vector<std::pair<int, int>> edges) {
	n = n_;
	clear();

	for (const auto &edg : edges) {
		g[edg.first].push_back(edg.second);
		g[edg.second].push_back(edg.first);
	}

	dfs(0, -1, 0);

	std::vector<int> perm;

	std::vector<bool> ret_; 
	dfsEncode(0, -1, 0, ret_, perm);
	assert(perm[0] == 0);
	perm.erase(perm.begin());

	std::vector<bool> ret;
	std::vector<int> real = perm;
	std::sort(real.begin(), real.end());
	
	for (const auto it : real) {
		//std::cerr << it << " ";
	}
	//std::cerr << std::endl;
	assert(real.size() == n - 1);

	for (const auto it : perm) {
		const auto ind = std::find(real.begin(), real.end(), it);
		encode(ret, numBits(real.size()), ind - real.begin());
		real.erase(ind);
	}

	for (const auto it : ret_) {
		ret.push_back(it);
	}

	//std::cerr << "finished encoding -----------" << std::endl;
	for (const auto it : perm) {
		//std::cerr << it << " ";
	}
	//std::cerr << std::endl;
	//std::cerr << "cnt is " << cnt << std::endl;
	while (ending) {
		ret.pop_back();
		ending --;
	}
	return ret;
}

std::vector<std::pair<int, int>> decode(int n_, std::vector<bool> data) {
	n = n_;
	clear();

	int curr = 0;

	std::vector<int> perm;
	std::vector<int> real(n - 1);
	std::iota(real.begin(), real.end(), 1);
	
	for (int i = 1; i < n; i ++) {
		const auto ind = decode(data, curr, numBits(real.size()));
		curr += numBits(real.size());
		perm.push_back(real[ind]);
		real.erase(real.begin() + ind);
	}

	//std::cerr << "decoding -----------------------" << std::endl;
	for (const auto it : data) {
		//std::cerr << (bool)it << " ";
	}
	//std::cerr << std::endl;
	//std::cerr << curr << std::endl;
	for (const auto it : perm) {
		//std::cerr << it << " ";
	}
	//std::cerr << std::endl;

	std::reverse(perm.begin(), perm.end());
	perm.push_back(0);

	std::vector<int> nodes;
	std::vector<std::pair<int, int>> ret = {};
	const auto root = dfsDecode(-1, -1, data, curr, nodes, perm, ret);
	assert(root == 0);

	//std::cerr << "Ended decoding with " << n << " " << cnt << " " << ret.size() << std::endl;

	return ret;
}
