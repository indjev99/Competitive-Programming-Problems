#include<iostream>
#include<fstream>
using namespace std;

#define official
#ifdef official
ifstream inF("rome.in");
ofstream outF("rome.out");
#define cin inF
#define cout outF
#endif

const long long MOD=1e9+7;
const int MAX_N=300;
int n;

long long qpow(long long a, long long p)
{
    if (p==0) return 1;
    if (p%2) return a*qpow(a,p-1)%MOD;
    long long r=qpow(a,p/2);
    return r*r%MOD;
}
long long modulo_inverse(long long a)
{
    return qpow(a,MOD-2);
}
struct Matrix
{
    long long a[MAX_N+10][MAX_N+10];
    void swap_rows(int r1, int r2)
    {
        for (int i=0;i<n;++i)
        {
            swap(a[r1][i],a[r2][i]);
        }
    }
    void mult_row(int r1, long long s)
    {
        for (int i=0;i<n;++i)
        {
            a[r1][i]=(a[r1][i]*s)%MOD;
        }
    }
    void add_mult_row(int r1, int r2, long long s)
    {
        for (int i=0;i<n;++i)
        {
            a[r1][i]=(a[r1][i]+a[r2][i]*s)%MOD;
        }
    }
    long long determinant(int rem=-1)
    {
        long long curr=1;
        int pos=0;
        while (pos<n)
        {
            if (pos==rem) ++pos;
            if (a[pos][pos]==0)
            {
                for (int i=pos+1;i<n;++i)
                {
                    if (i==rem) continue;
                    if (a[i][pos]!=0)
                    {
                        swap_rows(pos,i);
                        curr*=-1;
                        break;
                    }
                }
                if (a[pos][pos]==0) return 0;
            }
            curr=curr*a[pos][pos]%MOD;
            mult_row(pos,modulo_inverse(a[pos][pos]));
            for (int i=pos+1;i<n;++i)
            {
                if (i==rem) continue;
                if (a[i][pos]!=0)
                {
                    add_mult_row(i,pos,-a[i][pos]);
                }
            }
            ++pos;
        }
        if (curr<0) curr+=MOD;
        return curr;
    }
};
Matrix adj;
long long ans;
void input()
{
    int m;
    int from,to;
    cin>>n>>m;
    for (int i=0;i<m;++i)
    {
        cin>>from>>to;
        if (from==to) continue;
        --from;
        --to;
        ++adj.a[from][to];
    }
}
void output()
{
    cout<<ans<<endl;
}
int hubs;
int hub1;
int hub2;
Matrix lap;
bool vis[MAX_N];
bool vis2[MAX_N];
void reset_vis()
{
    for (int i=0;i<n;++i)
    {
        vis[i]=0;
        vis2[i]=0;
    }
}
int dir_DFS(int curr)
{
    int reach=0;
    for (int i=0;i<n;++i)
    {
        if (!vis[i] && adj.a[curr][i])
        {
            vis[i]=1;
            reach+=1+dir_DFS(i);
        }
    }
    return reach;
}
int rev_DFS(int curr)
{
    int reach=0;
    for (int i=0;i<n;++i)
    {
        if (!vis2[i] && adj.a[i][curr])
        {
            vis2[i]=1;
            if (!vis[i]) ++reach;
            reach+=rev_DFS(i);
        }
    }
    return reach;
}
void find_hubs()
{
    hubs=0;
    hub1=-1;
    hub2=-1;
    int reach;
    for (int i=0;i<n;++i)
    {
        reach=1;
        reset_vis();
        vis[i]=1;
        reach+=dir_DFS(i);
        vis2[i]=1;
        reach+=rev_DFS(i);
        if (reach==n)
        {
            if (hubs==0) hub1=i;
            else hub2=i;
            ++hubs;
        }
    }
}
void lap_dir_DFS(int curr)
{
    for (int i=0;i<n;++i)
    {
        if (adj.a[curr][i])
        {
            lap.a[curr][i]=-adj.a[curr][i];
            lap.a[i][i]+=adj.a[curr][i];
            if (!vis[i])
            {
                vis[i]=1;
                lap_dir_DFS(i);
            }
        }
    }
}
void lap_rev_DFS(int curr)
{
    for (int i=0;i<n;++i)
    {
        if (adj.a[i][curr])
        {
            lap.a[curr][i]=-adj.a[i][curr];
            lap.a[i][i]+=adj.a[i][curr];
            if (!vis[i])
            {
                vis[i]=1;
                lap_rev_DFS(i);
            }
        }
    }
}
void generate_redirected_laplacian(int hub)
{
    for (int i=0;i<n;++i)
    {
        for (int j=0;j<n;++j)
        {
            lap.a[i][j]=0;
        }
    }
    reset_vis();
    vis[hub]=1;
    lap_dir_DFS(hub);
    lap_rev_DFS(hub);
}
long long solve_hub(int hub)
{
    generate_redirected_laplacian(hub);
    return lap.determinant(hub);
}
Matrix adj2;
long long solve_hubs_no_cycle(int hub1, int hub2)
{
    long long ans=0;
    ans+=solve_hub(hub1);
    ans+=solve_hub(hub2);
    ans%=MOD;
    if (ans<0) ans+=MOD;
    if (!adj.a[hub1][hub2]) swap(hub1,hub2);
    if (!adj.a[hub1][hub2]) return ans;
    adj2=adj;
    for (int i=0;i<n;++i)
    {
        if (i!=hub2) adj.a[hub1][i]=0;
    }
    ans-=solve_hub(hub1);
    adj=adj2;
    ans%=MOD;
    if (ans<0) ans+=MOD;
    return ans;
}
long long solve_hubs(int hub1, int hub2)
{
    if (!adj.a[hub1][hub2] || !adj.a[hub2][hub1]) return solve_hubs_no_cycle(hub1,hub2);
    long long ans=0;
    long long con=adj.a[hub1][hub2];
    adj.a[hub1][hub2]=0;
    ans+=solve_hubs_no_cycle(hub1,hub2);
    adj.a[hub1][hub2]=con;
    adj.a[hub2][hub1]=0;
    ans+=solve_hubs_no_cycle(hub1,hub2);
    return ans;
}
void solve()
{
    ans=0;
    find_hubs();
    if (hubs!=1 && hubs!=2)
    {
        cerr<<"ERROR "<<hubs<<" HUBS"<<endl;
        return;
    }
    else if (hubs==1) ans=solve_hub(hub1);
    else ans=solve_hubs(hub1,hub2);
}
int main()
{
    input();
    solve();
    output();
    return 0;
}
