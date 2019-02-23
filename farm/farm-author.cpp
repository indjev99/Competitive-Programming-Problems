#include<iostream>
#include<algorithm>
#include<fstream>
#include<vector>
#include<deque>
using namespace std;

#define official
#ifdef official
ifstream inF("farm.in");
ofstream outF("farm.out");
#define cin inF
#define cout outF
#endif

struct Point
{
    long long x;
    long long y;
};
bool operator<(const Point& a, const Point& b)
{
    if (a.x<b.x) return 1;
    if (a.x>b.y) return 0;
    return a.y<b.y;
}

const int MAX_N=50;
int n,k;
Point p[MAX_N];
long long dp[MAX_N][MAX_N+1][MAX_N][MAX_N]; //first used prev last
long long p2[MAX_N][MAX_N+1][MAX_N][MAX_N]; //first used prev last
long long ansS,ansC,ansP;
long long ans=-1;
void input()
{
    cin>>n>>k;
    for (int i=0;i<n;++i)
    {
        cin>>p[i].x>>p[i].y;
    }
    sort(p,p+n);
    for (int i=0;i<n-1;++i)
    {
        if (p[i].x==p[i+1].x && p[i].y==p[i+1].y) cerr<<"ERROR"<<endl;
    }

}
void output()
{
    cout<<ans<<'\n';
    /*cout<<p[ansC].x<<'\t'<<p[ansC].y<<'\n';
    for (int i=k;i>=2;--i)
    {
        int temp=p2[ansS][i][ansC][ansP];
        ansC=ansP;
        ansP=temp;
        cout<<p[ansC].x<<'\t'<<p[ansC].y<<'\n';
    }*/
}
Point operator-(const Point& a, const Point& b)
{
    return {a.x-b.x,a.y-b.y};
}
long long cross(const Point& a, const Point& b)
{
    return a.x*b.y-a.y*b.x;
}
void solve()
{
    Point start,prev,prev2,curr;
    long long area;
    for (int i=0;i<n;++i)
    {
        for (int j=0;j<=k;++j)
        {
            for (int s=0;s<n;++s)
            {
                for (int t=0;t<n;++t)
                {
                    dp[i][j][s][t]=-1;
                }
            }
        }
    }
    for (int i=0;i<n;++i)
    {
        start=p[i];
        for (int j=3;j<=k;++j)
        {
            for (int s=0;s<n;++s)
            {
                curr=p[s];
                if (curr.x<start.x || (curr.x==start.x && curr.y<start.y) || s==i) continue;
                for (int t=0;t<n;++t)
                {
                    prev=p[t];
                    if (prev.x<start.x || (prev.x==start.x && prev.y<start.y) || t==i || t==s) continue;
                    if (j==3)
                    {
                        prev2=start;
                        area=cross(curr-start,prev-start);
                        if (area<=0) continue;
                        if (j==k && area>ans)
                        {
                            ansS=i;
                            ansC=s;
                            ansP=t;
                            ans=area;
                        }
                        if (area>dp[i][j][s][t])
                        {
                            dp[i][j][s][t]=area;
                            p2[i][j][s][t]=i;
                            //cerr<<"MAX: "<<j<<"  "<<area<<"   "<<curr.x<<" "<<curr.y<<"   "<<prev.x<<" "<<prev.y<<"   "<<prev2.x<<" "<<prev2.y<<"   "<<start.x<<" "<<start.y<<endl;
                        }
                    }
                    else for (int u=0;u<n;++u)
                    {
                        if (dp[i][j-1][t][u]<0) continue;
                        prev2=p[u];
                        if (cross(curr-prev2,prev-prev2)<=0) continue;
                        area=cross(curr-start,prev-start);
                        if (area<=0) continue;
                        area+=dp[i][j-1][t][u];
                        if (j==k && area>ans)
                        {
                            ansS=i;
                            ansC=s;
                            ansP=t;
                            ans=area;
                        }
                        if (area>dp[i][j][s][t])
                        {
                            dp[i][j][s][t]=area;
                            p2[i][j][s][t]=u;
                            //cerr<<"MAX: "<<j<<"  "<<area<<" , "<<dp[i][j-1][t][u]<<"   "<<curr.x<<" "<<curr.y<<"   "<<prev.x<<" "<<prev.y<<"   "<<prev2.x<<" "<<prev2.y<<"   "<<start.x<<" "<<start.y<<endl;
                        }
                    }
                }
            }
        }
    }
    cerr<<k<<" "<<ans<<endl;
}
int main()
{
    input();
    solve();
    output();
    return 0;
}
