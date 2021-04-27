#include <bits/stdc++.h>

using namespace std;
const int INF = 2e9;

bool Solve(vector<int>& x, array<int, 3> r) {
  int n = x.size();
  vector<vector<vector<bool>>> dp(n + 1, vector<vector<bool>>(
        n + 1, vector<bool>(n + 1, false)));  
  dp[0][0][0] = true;
  for (int i = 0; i <= n; ++i)
  for (int j = 0; j <= n; ++j)
  for (int k = 0; k <= n; ++k) {
    if (!dp[i][j][k]) continue;
    int pos = max({i, j, k});
    if (pos == n) return true;
    if (i == 0 || x[pos] - x[i - 1] >= r[0]) dp[pos + 1][j][k] = true;
    if (j == 0 || x[pos] - x[j - 1] >= r[1]) dp[i][pos + 1][k] = true;
    if (k == 0 || x[pos] - x[k - 1] >= r[2]) dp[i][j][pos + 1] = true;
  }
  return false;
}

int main() {
  int t; cin >> t;
  for (int j = 0; j < t; ++j) {
    int n, c; cin >> n >> c;
    assert(n > c);
    array<int, 3> r = {INF, INF, INF};
    for (int i = 0; i < c; ++i)
      cin >> r[i];
    vector<int> x(n);
    for (int i = 0; i < n; ++i)
      cin >> x[i];
    assert(is_sorted(x.begin(), x.end()));
    cout << Solve(x, r) << endl;
  }
  return 0;
}
