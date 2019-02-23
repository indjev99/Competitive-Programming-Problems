#include <bits/stdc++.h>
#define endl '\n'

//#pragma GCC optimize ("O3")
//#pragma GCC target ("sse4")

#define SZ(x) ((int)x.size())
#define ALL(V) V.begin(), V.end()
#define L_B lower_bound
#define U_B upper_bound
#define pb push_back

using namespace std;
template<class T, class T2> inline int chkmax(T &x, const T2 &y) { return x < y ? x = y, 1 : 0; }
template<class T, class T2> inline int chkmin(T &x, const T2 &y) { return x > y ? x = y, 1 : 0; }
const int MAXN = (1 << 20);
const int mod = (int)1e9 + 7;

int pw(int x, int p)
{
	int r = 1;
	while(p)
	{
		if(p & 1) r = r * 1ll * x % mod;
		x = x * 1ll * x % mod;
		p >>= 1;
	}

	return r;
}

int inv_v[MAXN];

void init(int n)
{
	for(int i = 0; i <= n; i++)
		inv_v[i] = pw(i, mod - 2);
}

struct dsu
{
	int sz[MAXN], par[MAXN], n;
	pair<int, int> ST[MAXN];
	int st_sz = 0, p = 1, cnt_comps;

	void init(int _n)
	{
		n = _n;
		for(int i = 0; i <= n; i++)
			par[i] = i, sz[i] = 1;

		st_sz = 0;
		cnt_comps = n;
		p = 1;
	}

	int answer()
	{
		if(cnt_comps == 1) return 1;
		return pw(n, cnt_comps - 2) * 1ll * p % mod;
	}

	int root(int x)
	{
		while(x != par[x])
			x = par[x];
		return x;
	}

	bool unite(int u, int v)
	{
		u = root(u), v = root(v);
		if(u == v) return false;

		p = p * 1ll * inv_v[sz[u]] % mod;
		p = p * 1ll * inv_v[sz[v]] % mod;
		p = p * 1ll * (sz[u] + sz[v]) % mod;
		cnt_comps--;

		if(sz[u] < sz[v]) swap(u, v);
		par[v] = u;
		sz[u] += sz[v];

		ST[st_sz++] = {u, v};

		return true;
	}

	void rollback()
	{
		st_sz--;
		int u = ST[st_sz].first, v = ST[st_sz].second;

		sz[u] -= sz[v];
		par[v] = v;

		p = p * 1ll * sz[u] % mod;
		p = p * 1ll * sz[v] % mod;
		p = p * 1ll * inv_v[(sz[u] + sz[v])] % mod;
		cnt_comps++;
	}
};

int n, q;
vector<pair<int, int> > li[MAXN];
map<pair<int, int>, int> ed;

void add(int ql, int qr, pair<int, int> e, int l, int r, int idx)
{
	if(ql <= l && r <= qr)
	{
		li[idx].pb(e);
		return;
	}

	int mid = (l + r) >> 1;
	if(ql <= mid) add(ql, qr, e, l, mid, 2 * idx + 1);
	if(mid < qr) add(ql, qr, e, mid + 1, r, 2 * idx + 2);
}

void read()
{
	cin >> n >> q;
	for(int i = 0; i < q; i++)
	{
		int type, u, v;
		cin >> type >> u >> v;
		if(u > v) swap(u, v);

		if(type == 1) ed[{u, v}] = i;
		else
		{
			int l = ed[{u, v}];
			add(l, i - 1, {u, v}, 0, q - 1, 0);
			ed.erase(ed.find({u, v}));
		}
	}
}

int answer[MAXN];
bool bad_flag = 0;
dsu d;

void solve(int l, int r, int idx)
{
	int cnt = 0;
	for(auto it: li[idx])
		if(d.unite(it.first, it.second))
			cnt++;
		else
		{
			bad_flag = 1;
			break;
		}

	if(!bad_flag)
	{
		if(l == r)
			answer[l] = d.answer();
		else
		{
			int mid = (l + r) >> 1;
			solve(l, mid, 2 * idx + 1);
			solve(mid + 1, r, 2 * idx + 2);
		}
	}

	bad_flag = 0;
	while(cnt--)
		d.rollback();
}

void solve()
{
	init(n);
	for(auto it: ed)
		add(it.second, q - 1, it.first, 0, q - 1, 0);

	d.init(n);
	solve(0, q - 1, 0);

	for(int i = 0; i < q; i++)
		cout << answer[i] << endl;
}

int main()
{
	freopen("trees.in", "r", stdin);
	freopen("trees.out", "w", stdout);
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}

