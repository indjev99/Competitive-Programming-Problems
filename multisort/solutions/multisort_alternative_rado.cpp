#include <bits/stdc++.h>
#include "multisort.h"

#define SZ(x) ((int)x.size())
#define ALL(V) V.begin(), V.end()
#define L_B lower_bound
#define U_B upper_bound
#define pb push_back

using namespace std;
template<class T, class T1> int chkmin(T &x, const T1 &y) { return x > y ? x = y, 1 : 0; }
template<class T, class T1> int chkmax(T &x, const T1 &y) { return x < y ? x = y, 1 : 0; }
const int MAXN = (1 << 20);
const int STD = 2;
const int TOLERANCE = 100000;

int dp[MAXN];
int opt[MAXN];

random_device rd;
mt19937 mt(rd());

int HELPER, used[MAXN];

void solve(vector<int> &a, int k) {
	int n = SZ(a);
	if(n == 1 || n == 0) return;
	if(n <= k) {
		compare(a);
		return;
	}

	shuffle(ALL(a), mt);

	int s = opt[n], q = k - s;

	bool pivots_order_flag = false;
	vector<int> pivots(a.begin(), a.begin() + s);

	vector<vector<int> > buckets(s + 1);
	vector<int> bucket_cnt(s + 1, 0);

	for(int l = s; l < n; l += q) {
		int r = min(n, l + q);
		vector<int> query(ALL(pivots));
		query.insert(query.end(), a.begin() + l, a.begin() + r);

		compare(query);

		if(!pivots_order_flag) {
			HELPER++;

			int delta = k / s;
			for(int i = 0, j = 0; j < s; j++, i += delta) {
				int l = i;
				int r = min(n, i + delta);
				used[query[(l + r) / 2]] = HELPER;
			}

			int pos = 0;
			for(int x: query) {
				if(used[x] == HELPER) {
					pivots[pos++] = x;
				}
			}

			pivots_order_flag = true;
		}

		int i = 0;

		vector<bool> bucket_flag(s + 1, false);
		for(int x: query) {
			if(i < s && x == pivots[i]) i++;
			else {
				if(!bucket_flag[i]) bucket_cnt[i]++;
				bucket_flag[i] = true;
				buckets[i].pb(x);
			}
		}
	}

	int pos = 0;
	for(int i = 0; i < s + 1; i++) {
		if(bucket_cnt[i] > 1) solve(buckets[i], k);
		for(int x: buckets[i]) a[pos++] = x;
		if(i != s) {
			a[pos++] = pivots[i];
		}
	}
}

void precompute(int n, int k) {
	for(int len = 2; len <= k; len++) {
		dp[len] = 1;
		opt[len] = 0;
	}

	int BEST = (k + 0) / 2, cnt_best = 0;
	for(int len = k + 1; len <= n; len++) {
		dp[len] = (int)1e9;
		for(int cnt_pivots = max(opt[len - 1] - STD, 1); cnt_pivots <= min(opt[len - 1] + STD, BEST); cnt_pivots++) {
			int current_dp = 0, total_free = len - cnt_pivots, info_per_query = k - cnt_pivots;
			current_dp += total_free / info_per_query + !!(total_free % info_per_query);

			int cnt_in_one = total_free / (cnt_pivots + 1);
			int cnt_full = total_free - cnt_in_one * (cnt_pivots + 1);
			current_dp += (cnt_pivots + 1 - cnt_full) * dp[cnt_in_one] + cnt_full * dp[cnt_in_one + 1];

			if(chkmin(dp[len], current_dp)) {
				opt[len] = cnt_pivots;
			}
		}

		if(opt[len] == BEST) cnt_best++;

		if(cnt_best == TOLERANCE) {
			for(int i = len + 1; i <= n; i++) {
				opt[i] = BEST;
			}

			return;
		}
	}
}

vector<int> multisort(int n, int k) {
	precompute(n, k);
	vector<int> perm(n);
	iota(ALL(perm), 0);
	solve(perm, k);
	return perm;
}
