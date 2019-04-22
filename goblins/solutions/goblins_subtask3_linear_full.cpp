#include <iostream>
#include <algorithm>

const int MAX_N=1e6;
const int MAX_Q=2e6;
const int MAX_T=5e6;

struct Func
{
    int a;
    long long b;
    Func() {}
    Func(int a, long long b): a{a}, b{b} {}
    long long eval(int x) const
    {
        return ((long long)a)*(x/2)+b;
    }
};

struct Query
{
    int num;
    int t;
    bool operator<(const Query& other) const
    {
        return t<other.t;
    }
};

int n,q;
int a[MAX_N];
Query qs[MAX_Q];
Func funcs[2][MAX_N];
int fs;
long long ans[MAX_Q];

void output()
{
    for (int i=0;i<q;++i)
    {
        std::cout<<ans[i]<<'\n';
    }
}
void input()
{
    std::cin>>n>>q;
    for (int i=0;i<n;++i)
    {
        std::cin>>a[i];
    }
    for (int i=0;i<q;++i)
    {
        qs[i].num=i;
        std::cin>>qs[i].t;
    }
}
void generate_funcs()
{
    long long sum=0;
    int val;
    for (int i=0;i<n-1;++i)
    {
        sum+=a[i];
        val=a[i]+a[i+1];
        funcs[1-i%2][fs]=Func(val,sum-((long long)i+1)/2*val);
        funcs[i%2][fs]=Func(val,sum+a[i+1]-((long long)i+2)/2*val);
        ++fs;
    }
}
long long query(int t)
{
    const Func* currFuncs=funcs[t%2];
    long long mx=-1,res;
    for (int i=0;i<t && i<fs;++i)
    {
        res=currFuncs[i].eval(t);
        if (res>mx) mx=res;
    }
    return mx;
}
void solve()
{
    generate_funcs();
    for (int i=0;i<q;++i)
    {
        ans[qs[i].num]=query(qs[i].t);
    }
}
int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    input();
    solve();
    output();

    return 0;
}
