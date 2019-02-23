#include<iostream>
#include<fstream>
#include<queue>
using namespace std;

#define official
#ifdef official
ifstream inF("space.in");
ofstream outF("space.out");
#define cin inF
#define cout outF
#endif

const int MAX_N=100000;
const int MAX_K=20;
int n,m,k,s,q;
int de[MAX_N]; //down edges
vector<int> e[MAX_N]; //regular edges
int dist[MAX_N][MAX_K+1][2];
int ans[MAX_N];
struct Loc
{
    int pos;
    int fl;
    bool th;
};
bool operator<(const Loc& a, const Loc& b)
{
    return 1;
}

void input()
{
    cin>>n>>m>>s>>k;
    --s;
    int a,b;
    for (int i=1;i<n;++i)
    {
        cin>>a;
        --a;
        de[i]=a;
    }
    for (int i=0;i<m;++i)
    {
        cin>>a>>b;
        --a;
        --b;
        if (a==b || a==0 || b==0) cerr<<"ERROR "<<a+1<<" "<<b+1<<endl;
        e[a].push_back(b);
        e[b].push_back(a);
    }
    cin>>q;
}
priority_queue<pair<int,Loc>> pq;
void handle(int pos, int fl, bool th, int newdist)
{
    if (ans[pos]==-1 || ans[pos]>newdist) ans[pos]=newdist;
    if (dist[pos][fl][th]==-1 || dist[pos][fl][th]>newdist)
    {
        dist[pos][fl][th]=newdist;
        pq.push({-newdist,{pos,fl,th}});
    }
}
void solve()
{
    for (int i=0;i<n;++i)
    {
        ans[i]=-1;
        for (int j=0;j<=k;++j)
        {
            dist[i][j][0]=-1;
            dist[i][j][1]=-1;
        }
    }
    dist[s][1][0]=0;
    ans[s]=0;
    pq.push({0,{s,1,0}});
    Loc p;
    int d;
    int newfl;
    int newpos;
    while (!pq.empty())
    {
        d=-pq.top().first;
        p=pq.top().second;
        pq.pop();
        s=p.pos;
        newfl=p.fl+1;
        if (newfl>k) newfl=k;
        handle(de[s],newfl,0,d);
        newfl=p.fl-1;
        if (newfl<0) continue;
        if (p.th==0) ++d;
        for (int i=0;i<e[s].size();++i)
        {
            handle(e[s][i],newfl,1,d);
        }
    }
}
int main()
{
    input();
    solve();
    int b;
    for (int i=0;i<q;++i)
    {
        cin>>b;
        if (i) cout<<' ';
        cout<<ans[b-1];
    }
    cout<<'\n';
    return 0;
}
