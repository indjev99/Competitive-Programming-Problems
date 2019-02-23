#include<iostream>
#include<fstream>
using namespace std;

#define official
#ifdef official
ifstream inF("yen.in");
ofstream outF("yen.out");
#define cin inF
#define cout outF
#endif

long long a,b,c,d;
long long gcdAB,xAB,yAB,gcdDC,xDC,yDC;
long long n;
bool poss;
long long ans1,ans2,ans3;

void input()
{
    cin>>a>>b>>c>>n;
}
void output()
{
    if (!poss) cout<<"No\n";
    else cout<<"Yes\n"<<ans1<<' '<<ans2<<' '<<ans3<<'\n';
}
pair<long long, pair<long long, long long>> gcd(long long a, long long b)
{
    long long x=1, y=0;
    long long u=0, v=1;
    long long q,p;
    while (b!=0)
    {
        q=a/b;
        p=a-q*b;
        a=b;
        b=p;
        p=x-q*u;
        x=u;
        u=p;
        p=y-q*v;
        y=v;
        v=p;
    }
    return {a,{x,y}};
}
void solve()
{
    auto p=gcd(a,b);
    gcdAB=p.first;
    xAB=p.second.first;
    yAB=p.second.second;
    d=gcdAB;
    p=gcd(d,c);
    gcdDC=p.first;
    xDC=p.second.first;
    yDC=p.second.second;
    if (n%gcdDC) poss=false;
    else
    {
        poss=true;
        long long p=n/gcdDC;
        ans1=xAB*xDC*p;
        ans2=yAB*xDC*p;
        ans3=yDC*p;
        //check
        cerr<<ans1*a+ans2*b+ans3*c<<' '<<n<<'\n';
    }
}
int main()
{
    input();
    solve();
    output();
    return 0;
}
