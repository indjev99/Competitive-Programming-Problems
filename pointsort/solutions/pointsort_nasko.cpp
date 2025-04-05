#include <bits/stdc++.h>
#include "pointsort.h"

int k;

const int MAX_K = 10;
const int MAX_N = 1e5 + 10;

std::vector<int> g[MAX_K][MAX_N];

bool find(const int ind, const int a, const int b, const int d = 1) {
	if (a == b) { return true; }
	if (d == 0) { return false; }
	for (const auto it : g[ind][a]) {
		if (find(ind, it, b, d - 1)) {
			return true;
		}
	}
	return false;
}

std::vector<bool> myCompare(const int a, const int b) {
  if (a == b) {
	return std::vector<bool>(k, false);
  } else {
	std::vector<bool> ret(k);
	bool ok = true;
	for (int i = 0; i < k; i ++) {
		if (find(i, a, b)) {
			ret[i] = true; 
		} else if (find(i, b, a)) { 
			ret[i] = false;
		} else {
			ok = false;
		}
	}
	if (ok) { return ret; }
	const auto curr = compare(a, b);
	for (int i = 0; i < k; i ++) {
	  if (curr[i]) {
		g[i][a].push_back(b);
	  } else {
		g[i][b].push_back(a);
	  }
	}
	return curr;
  }
}

auto compareOn(const int ind) {
  return [ind] (const auto a, const auto b) {
	const auto ret = myCompare(a, b);
	return ret[ind];
  };
}

std::vector<int> mergeOn(const std::vector<int> &left, const std::vector<int> &right, const int ind) {
  size_t i = 0, j = 0;
  std::vector<int> ret = {};
  while (i < left.size() && j < right.size()) {
	if (myCompare(left[i], right[j])[ind]) {
	  ret.push_back(left[i ++]);
	} else {
	  ret.push_back(right[j ++]);
	}
  }

  while (i < left.size()) { ret.push_back(left[i ++]); }
  while (j < right.size()) { ret.push_back(right[j ++]); }
  return ret;
}

std::vector<int> mergeAll(std::vector<std::vector<int>> &all, const int ind) {
  if (all.empty()) { return {}; }

  std::priority_queue<std::pair<int, int> > pq;
  for (size_t i = 0; i < all.size(); i ++) {
	pq.push({-(int)all[i].size(), i});
  }

  while (pq.size() > 1) {
	const auto a = pq.top().second; pq.pop();
	const auto b = pq.top().second; pq.pop();
	std::vector<int> ans;
	std::merge(
	  all[a].begin(), all[a].end(), 
	  all[b].begin(), all[b].end(), 
	  std::back_inserter(ans), 
	  [ind] (const auto a, const auto b) {
		if (a == b) {
		  return false;
		}
		return (bool)myCompare(a, b)[ind];
	  }
	);
	all[a] = ans;
	pq.push({-(int)all[a].size(), a});
  }

  return all[pq.top().second];
}

std::vector<std::vector<int>> sortAll(std::vector<int> &who) {
  if (who.size() == 1) {
	return std::vector(k, who);
  }

  std::map<std::vector<bool>, std::vector<int>> parts;

  const int PIVOT_CNT = 9;
  const int MIN_PIVOTTING = 100;
  assert(PIVOT_CNT <= MIN_PIVOTTING);

  if (who.size() > MIN_PIVOTTING) {
	std::sort(who.begin(), who.begin() + PIVOT_CNT, compareOn(0));
	std::swap(who[0], who[PIVOT_CNT / 2]);
  }

  const auto pivot = who[0];
  for (int i = 1; i < who.size(); i ++) {
	const auto curr = myCompare(pivot, who[i]);
	parts[curr].push_back(who[i]);
  }

  std::map<std::vector<bool>, std::vector<std::vector<int>>> partsSorted;
  for (auto &it : parts) {
	partsSorted[it.first] = sortAll(it.second);
  }

  std::vector<std::vector<int>> rets(k);
  for (int i = 0; i < k; i ++) {
	std::vector<std::vector<int>> small, big;
	for (const auto &it : partsSorted) {
	  if (it.first[i]) {
		big.push_back(it.second[i]);
	  } else {
		small.push_back(it.second[i]);
	  }
	}

	auto sortedSmall = mergeAll(small, i);
	auto sortedBig = mergeAll(big, i);

	rets[i].insert(rets[i].end(), sortedSmall.begin(), sortedSmall.end());
	rets[i].push_back(pivot);
	rets[i].insert(rets[i].end(), sortedBig.begin(), sortedBig.end());
  }

  return rets;
}

std::vector<std::vector<int>> pointSort(int n, int k_) {
  k = k_;
  for (int i = 0; i < k; i ++) {
	for (int j = 0; j < n; j ++) {
	  g[i][j].clear();
	}
  }
  std::cerr << "Starting " << k << std::endl;
  std::vector<std::vector<int>> rets(n);

  std::vector<int> inds(n);
  std::iota(inds.begin(), inds.end(), 0);
  const auto sorted = sortAll(inds);

  for (int i = 0; i < k; i ++) {
	for (int j = 0; j < n; j ++) {
	  rets[sorted[i][j]].push_back(j);
	}
  }

  return rets;
}
