#include <iostream>
#ifndef __clang__
#include <bits/stdc++.h>
#endif
#define endl "\n"
const int OFFSET = 4000001;
const int MAX_N = OFFSET * 2;
std::string str;
int n;
int arr[OFFSET];
int pref[OFFSET];
int fen[MAX_N];
long long ans = 0;
int sum(int ind) {
    ind += 1 + OFFSET;
    int ret = 0;
    for (; ind > 0; ind -= ind & (-ind)) {
        ret += fen[ind];
    }
    return ret;
}
void upd(int ind, int val) {
    ind += 1 + OFFSET;
    for (; ind < MAX_N; ind += ind & (-ind)) {
        fen[ind] += val;
    }
}
int get_pref(int x) {
    if (x < 0) {
        return 0;
    } else {
        return pref[x];
    }
}
std::vector<std::pair<int, int>> prs = {};
void solve(int l, int r) {
    if (l == r) {
        ans ++;
        return;
    }
    int m = (l + r) / 2;
    prs.resize(0);
    {
        int max_dec = 0, curr_dec = 0;
        for (int i = m; i >= l; i --) {
            curr_dec += arr[i];
            max_dec = std::min(max_dec, curr_dec);
            prs.push_back({get_pref(i - 1) * 2 + 1, -max_dec});
        }
    }
    {
        int max_dec = 0, whole = 0;
        for (int i = m + 1; i <= r; i ++) {
            whole += arr[i];
            max_dec = std::min(0, max_dec + arr[i]);
            prs.push_back({-max_dec * 2, -whole});
        }
    }
    std::sort(prs.rbegin(), prs.rend());

    for (const auto &it : prs) {
        if (it.first % 2 == 0) {
            upd(it.second, 1);
        }
    }
    int cnt = 0;
    for (const auto &it : prs) {
        if (it.first % 2 == 0) {
            upd(it.second, -1);
        } else {
            ans += sum(it.first / 2 - it.second);
        }
    }
    solve(l, m);
    solve(m + 1, r);
}

signed main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cin >> n;
    std::cin >> str;
    for (int i = 0; i < n; i ++) {
        if (str[i] == '(') {
            arr[i] = 1;
        } else {
            arr[i] = -1;
        }
        if (i != 0) {
            pref[i] = pref[i - 1] + arr[i];
        } else {
            pref[i] = arr[i];
        }
    }
    solve(0, n - 1);
    std::cout << ans << endl;
}
