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

const int maxn = 1.5e6 + 10;
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
    char type;
    int  val;

    task(char _type = 0, int _val = 0)
    {

        type = _type;
        val = _val;
    }
};

ll ans;
vector < task > spot[2][maxn];
vector < task > spot_again[2][maxn];
void dfs(int ver, int left, int right)
{
    if (ver == -1)
        return;
    if (ver - left < right - ver)
    {
        for (int l = left; l <= ver; l ++)
        {
            if (rev[ver] + pref[l - 1] <= -n)
            {
                ans = ans + (ll)(right - ver + 1);
            }
            else if (rev[ver] + pref[l - 1] <= n)
            {

                spot[0][ver].push_back(task(1, rev[ver] + pref[l - 1]));
                //spot_again[0][ver].push_back(task(1, rev[ver] + pref[l - 1]));
                spot[0][right + 1].push_back(task(-1, rev[ver] + pref[l - 1]));
                //spot_again[0][right + 1].push_back(task(-1, rev[ver] + pref[l - 1]));
            }
        }
    }
    else
    {
        int tl = spot[1][ver].size();
        spot[1][ver].reserve(tl + (right - ver + 1));
        //tl = spot[1][left - 1].size();
        //spot[1][left - 1].reserve(tl + (right - ver + 1));
        for (int r = ver; r <= right; r ++)
        {
            spot[1][ver].push_back(task(1, rev[r + 1] - rev[ver]));
            ///spot_again[1][ver].push_back(task(1, rev[r + 1] - rev[ver]));
            spot[1][left - 1].push_back(task(-1, rev[r + 1] - rev[ver]));
            ///spot_again[1][left - 1].push_back(task(-1, rev[r + 1] - rev[ver]));
        }
    }

    dfs(lc[ver], left, ver - 1);
    dfs(rc[ver], ver + 1, right);
}


int arr[4 * maxn];

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
    ///build_cartesian();
    dfs(root, 1, n);
    ///dfs(root, 1, n);
    /**int mx = 0;
    for (int i = 1; i <= n; i ++)
        mx = min(mx, pref[i - 1]);
    to_add = - mx + 1;*/

    int buff = 2 * n;
    for (int i = 1; i <= n; i ++)
        for (task cur : spot[1][i])
        {

            arr[buff + cur.val] += cur.type;
            ///cout << "query " << i << " " << cur.type << " " << cur.val << endl;
        }

    int cnt = 0;
    for (int i = 0; i <= buff + pref[0]; i ++)
        cnt += arr[i];

    /// pref[i - 1] needs to be more

    for (int i = 1; i <= n; i ++)
    {
        if (i != 1)
        {
            int last = pref[i - 2], cur = pref[i - 1];
            if (cur > last)
                cnt += arr[cur + buff];
            else
                cnt -= arr[last + buff];
        }
        ans = ans + (ll)(cnt);

        for (task cur : spot[1][i])
        {
            if (cur.val <= pref[i - 1])
                cnt -= cur.type;
            else
                cnt += cur.type;
            arr[cur.val + buff] -= cur.type;
        }

    }



    for (int i = 1; i <= n; i ++)
        for (task cur : spot[0][i])
        {
            arr[buff + cur.val] += cur.type;

        }

    cnt = 0;

    for (int i = buff + rev[n + 1]; i <= 2 * buff; i ++)
        cnt += arr[i];


    for (int i = n; i > 0; i --)
    {

        if (i != n)
        {
            int last = rev[i + 2], cur = rev[i + 1];

            if (cur > last)
                cnt -= arr[last + buff];
            else
                cnt += arr[cur + buff];
        }

        ans += (ll)(cnt);
        for (task cur : spot[0][i])
        {
            if (cur.val >= rev[i + 1])
                cnt -= cur.type;

            arr[cur.val + buff] -= cur.type;

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
