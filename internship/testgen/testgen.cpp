#include <iostream>
#include <fstream>
#include <time.h>
#include <assert.h>
#include <algorithm>

using namespace std;

const int MAX_N=5e5;
const int MAX_M=1e7+2;

struct Interval
{
    int from;
    int to;
    long long cost;
};
Interval is[MAX_N];
int a,b;
int n,m;

long long random_number()
{
    return rand()*(RAND_MAX+1ll)+rand();
}
long long target_cost(long long from, long long to, int type)
{
    if (type==1) return (to-from+1)*(to-from+1)/m+1;
    if (type==2) return (to-from+1)*(to-from+1)/m+1;
    if (type==3) return (to-from+1)*(to-from+1)/m+1;
    if (type==4) return (to-from+1)*(to+1)/m+1;
    if (type==0) return (to-from+1)*(to+1)/m+1;
}
long long minCost;
long long maxCost;
long long gen_cost(long long target)
{
    long long ans=target-random_number()%(target/4+1);
    minCost=min(minCost,ans);
    maxCost=max(maxCost,ans);
    return ans;
}
void gen_test(ofstream& outfile, int type)
{
    minCost=1e9;
    maxCost=-1e9;
    srand(time(0));
    a=random_number()%(m/3);
    b=m-random_number()%(m/3);
    if (a>b) swap(a,b);
    for (int i=0;i<n;++i)
    {
        is[i].from=random_number()%(m+1);
        is[i].to=random_number()%(m+1);
        if (is[i].from>is[i].to) swap(is[i].from,is[i].to);
        if ((type==3 || type==0) && is[i].to-is[i].from>=m/10) --i;
        else is[i].cost=gen_cost(target_cost(is[i].from,is[i].to,type));
    }
    outfile<<n<<'\n';
    outfile<<a<<' '<<b<<'\n';
    for (int i=0;i<n;++i)
    {
        outfile<<is[i].from<<' '<<is[i].to<<' '<<is[i].cost<<'\n';
    }
    cerr<<minCost<<" "<<maxCost<<endl;
}
int main()
{
    ios::sync_with_stdio(false);
    //cin.tie(nullptr);
    int num,type;
    int on,om;
    while (1)
    {
        cout<<"Test num? ";
        cin>>num;
        cout<<"N? ";
        cin>>n;
        cout<<"M? ";
        cin>>m;
        if (n==0) n=on;
        else on=n;
        if (m==0) m=om;
        else om=m;
        type=num%5;
        string nums="";
        nums+=(num/10+'0');
        nums+=(num%10+'0');
        ofstream outfile("internship."+nums+".in");
        gen_test(outfile,type);
    }
    return 0;
}
