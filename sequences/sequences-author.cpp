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
void solve()
{
    long long l=0;
    long long r=n;
    long long m;
    while (l<r)
    {
        m=(l+r)/2;
        if (eval1(m+s1)<eval2(n-m+s2-1)) l=m+1;
        else r=m;
    }
    if (l==0) ans=eval2(n+s2-1);
    else if (l==n) ans=eval1(n+s1-1);
    else ans=max(eval1(l+s1-1),eval2(n-l+s2-1));
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
