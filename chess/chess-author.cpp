#include<iostream>
#include<fstream>
#include<queue>
using namespace std;

#define official
#ifdef official
ifstream inF("chess.in");
ofstream outF("chess.out");
#define cin inF
#define cout outF
#endif

const int MAX_N=1000;
int n,x1,x2,y1,y2;
long long k;
bool taken[MAX_N][MAX_N];
int d[MAX_N][MAX_N];
bool ans;

void input()
{
    cin>>n>>k>>x1>>y1>>x2>>y2;
    --x1;
    --y1;
    --x2;
    --y2;
    for (int i=0;i<n;++i)
    {
        for (int j=0;j<n;++j)
        {
            cin>>taken[i][j];
        }
    }
}
void output()
{
    if (ans) cout<<"Yes"<<endl;
    else cout<<"No"<<endl;
}
queue<pair<int,int>> q;
void handle(int x, int y, int cd)
{
    if (x<=0 || x>n || y<=0 || y>n) return;
    if (taken[x][y]) return;
    if (d[x][y]!=-1) return;
    d[x][y]=cd+1;
    q.push({x,y});
}
void solve()
{
    for (int i=0;i<n;++i)
    {
        for (int j=0;j<n;++j)
        {
            d[i][j]=-1;
        }
    }
    d[x1][y1]=0;
    q.push({x1,y1});
    pair<int,int> p;
    int x,y;
    while (!q.empty())
    {
        p=q.front();
        q.pop();
        x=p.first;
        y=p.second;
        handle(x-2,y-1,d[x][y]);
        handle(x-2,y+1,d[x][y]);
        handle(x+2,y-1,d[x][y]);
        handle(x+2,y+1,d[x][y]);
        handle(x-1,y-2,d[x][y]);
        handle(x-1,y+2,d[x][y]);
        handle(x+1,y-2,d[x][y]);
        handle(x+1,y+2,d[x][y]);
    }
    cerr<<d[x2][y2]<<' '<<k<<endl;
    if (d[x2][y2]==-1)
    {
        cerr<<"No path\n";
        ans=0;
    }
    else if (d[x2][y2]>k && (k-d[x2][y2])%2)
    {
        cerr<<"Both\n";
        ans=0;
    }
    else if (d[x2][y2]>k)
    {
        cerr<<"Path too long\n";
        ans=0;
    }
    else if ((k-d[x2][y2])%2)
    {
        cerr<<"Wrong cycle\n";
        ans=0;
    }
    else
    {
        cerr<<"Correct\n";
        ans=1;
    }
}
int main()
{
    input();
    solve();
    output();
    return 0;
}
