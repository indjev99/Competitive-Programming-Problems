#include<bits/stdc++.h>
#define endl '\n'

using namespace std;
typedef long long ll;

void speed()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
}

const int maxn = 4e6 + 10;
int n;
string s;
int pref[maxn], lc[maxn], rc[maxn];
int rev[maxn];

int root;
void build_cartesian()
{
    stack < int > st;
    for (int i = 1; i <= n; i ++)
    {
        lc[i] = -1;
        while(!st.empty() && rev[st.top()] >= rev[i])
        {
            lc[i] = st.top();
            st.pop();
        }
        st.push(i);
    }

    while(!st.empty())
    {
        root = st.top();
        st.pop();
    }

    for (int i = n; i > 0; i --)
    {
        rc[i] = -1;
        while(!st.empty() && rev[st.top()] > rev[i])
        {
            rc[i] = st.top();
            st.pop();
        }
        st.push(i);
    }
}

struct task
{
    int pivot, type, val;

    task(int _pivot = 0, int _type = 0, int _val = 0)
    {
        pivot = _pivot;
        type = _type;
        val = _val;
    }
};

ll ans = 0;
vector < task > spot[2][maxn];
void dfs(int ver, int left, int right)
{
    if (ver == -1)
        return;
    if (ver - left < right - ver)
    {
        for (int l = left; l <= ver; l ++)
        {
            spot[0][ver].push_back(task(ver, 1, rev[ver] + pref[l - 1]));
            spot[0][right + 1].push_back(task(right + 1, -1, rev[ver] + pref[l - 1]));
        }
    }
    else
    {
        for (int r = ver; r <= right; r ++)
        {
            spot[1][ver].push_back(task(ver, 1, rev[r + 1] - rev[ver]));
            spot[1][left - 1].push_back(task(left - 1, -1, rev[r + 1] - rev[ver]));
        }
    }

    dfs(lc[ver], left, ver - 1);
    dfs(rc[ver], ver + 1, right);
}

int fen[2 * maxn];

void update(int pos, int val)
{
    pos += n;
    for (int i = pos; i <= 2 * n; i += (i & -i))
        fen[i] += val;
}

int sum(int pos)
{
    pos += n;
    int s = 0;
    for (int i = pos; i > 0; i -= (i & -i))
        s += fen[i];
    return s;
}

void solve()
{
    cin >> n >> s;
    s = "#" + s;
    for (int i = 1; i <= n; i ++)
    {
        pref[i] = pref[i - 1];
        if (s[i] == '(')
            pref[i] ++;
        else
            pref[i] --;
    }
    for (int i = n; i > 0; i --)
    {
        rev[i] = rev[i + 1];
        if (s[i] == '(')
            rev[i] ++;
        else
            rev[i] --;
    }


    build_cartesian();
    dfs(root, 1, n);

    for (int i = 1; i <= n; i ++)
    {
        update(pref[i - 1], 1);
        for (task cur : spot[1][i])
        {
            ans = ans + (ll)cur.type * (ll)(sum(n) - sum(cur.val - 1));
        }

    }

    for (int i = 1; i <= 2 * n; i ++)
        fen[i] = 0;

    for (int i = n; i > 0; i --)
    {
        update(rev[i + 1], 1);
        for (task cur : spot[0][i])
        {
            ans = ans + (ll)(cur.type) * (ll)(sum(cur.val));
        }
    }

    cout << ans << endl;

}

int main()
{
    speed();
    solve();
    return 0;
}
