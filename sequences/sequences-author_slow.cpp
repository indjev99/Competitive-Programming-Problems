#include<iostream>
#include<fstream>
using namespace std;

#define official
#ifdef official
ifstream inF("sequences.in");
ofstream outF("sequences.out");
#define cin inF
#define cout outF
#endif

long long b1, c1, s1, b2, c2, s2, n;
int q;
long long ans;

void input()
{
    cin>>q;
}
void output()
{
    cout<<ans<<'\n';
}
long long eval1(long long x)
{
    return x*x+b1*x+c1;
}
long long eval2(long long x)
{
    return x*x+b2*x+c2;
}
long long posIn1(long long x)
{
    long long l=-1;
    long long r=1e9;
    long long m;
    while (l<r)
    {
        m=(l+r+1)/2;
        if (eval1(m+s1)<=x) l=m;
        else r=m-1;
    }
    return l+1;
}
long long posIn2(long long x)
{
    long long l=-1;
    long long r=1e9;
    long long m;
    while (l<r)
    {
        m=(l+r+1)/2;
        if (eval2(m+s2)<=x) l=m;
        else r=m-1;
    }
    return l+1;
}
void solve()
{
    long long l=0;
    long long r=8e18;
    long long m;
    long long pos;
    while (l<r)
    {
        m=(l+r)/2;
        pos=posIn1(m)+posIn2(m);
        if (pos>=n) r=m;
        else l=m+1;
    }
    ans=l;
}
int main()
{
    input();
    for (int i=0;i<q;++i)
    {
        cin>>b1>>c1>>s1>>b2>>c2>>s2>>n;
        solve();
        output();
    }
    return 0;
}
