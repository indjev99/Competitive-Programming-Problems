#include <bits/stdc++.h>

using namespace std;
const int INF = 2e9;

bool Solve(vector<int>& x, array<int, 3> r) {
  sort(r.begin(), r.end());
  int n = x.size();
  vector<array<int, 3>> good = {{-1, -1, -1}}, ngood;
  for (int i = 0; i < n; ++i) {
    // If it doesn't hurt to put r[0], put it and continue.
    int dl = (i == 0 ? INF : x[i] - x[i - 1]);
    int dr = (i + 1 == n ? INF : x[i + 1] - x[i]);
    if (dl >= r[0] && dr >= r[0]) {
      for (auto& v : good)
        v[0] = i;
      continue;
    }
    // If radius doesn't matter (anymore), clear the tower.
    // This ultimately merges states.
    for (auto& v : good) 
      for (int z = 0; z < 3; ++z) 
        if (v[z] != -1 && x[i] - x[v[z]] >= r[z])
          v[z] = -1;
    // Compute next set of good triplets.
    ngood.clear();
    for (auto& v : good) {
      for (int z = 0; z < 3; ++z) {
        if (r[z] == INF || v[z] != -1) 
          continue;
        v[z] = i;
        ngood.push_back(v);
        v[z] = -1;
      }
    }
    // Remove non-dominating triplets.
    sort(ngood.begin(), ngood.end());
    ngood.erase(unique(ngood.begin(), ngood.end()), ngood.end());
    good.clear();
    for (int i = 0; i < (int)ngood.size(); ++i) {
      bool ok = true;
      auto& a = ngood[i]; 
      for (int j = 0; j < (int)good.size(); ++j) {
        if (i == j) continue;
        auto& b = good[j];
        if (b[0] <= a[0] && b[1] <= a[1] && b[2] <= a[2]) {
          ok = false; break;
        }
      }
      if (ok) good.push_back(a);
    }
  }
  return !good.empty();
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
