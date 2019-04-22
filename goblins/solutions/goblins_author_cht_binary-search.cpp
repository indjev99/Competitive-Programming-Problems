#include <iostream>
#include <vector>
#include <algorithm>

const int MAX_N=2e6;
const int MAX_Q=2e6;
const int MAX_T=5e6;

struct Func
{
    int a;
    long long b;
    long long startX;
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
    int mx=-1,val;
    for (int i=0;i<n-1;++i)
    {
        sum+=a[i];
        val=a[i]+a[i+1];
        if (val>mx)
        {
            mx=val;
            funcs[1-i%2][fs]=Func(val,sum-((long long)i+1)/2*val);
            funcs[i%2][fs]=Func(val,sum+a[i+1]-((long long)i+2)/2*val);
            ++fs;
        }
    }
}
long long cross_at(const Func& f1, const Func& f2)
{
    long long da=f1.a-f2.a;
    long long db=f2.b-f1.b;
    return (db+da-1)/da;
}
struct CHT
{
    Func hull[MAX_N];
    int hs;
    void add_func(const Func& f)
    {
        while (hs && cross_at(f,hull[hs-1])<=hull[hs-1].startX)
        {
            --hs;
        }
        hull[hs]=f;
        if (hs) hull[hs].startX=cross_at(f,hull[hs-1]);
        else hull[hs].startX=0;
        ++hs;
    }
    long long query(int x)
    {
        int l=0;
        int r=hs;
        int m;
        while (r-l>1)
        {
            m=(l+r)/2;
            if (hull[m].startX<=x/2) l=m;
            else r=m;
        }
        return hull[l].eval(x);
    }
} cht[2];
void solve()
{
    generate_funcs();
    for (int i=0;i<fs;++i)
    {
        cht[0].add_func(funcs[0][i]);
        cht[1].add_func(funcs[1][i]);
    }
    for (int i=0;i<q;++i)
    {
        ans[qs[i].num]=cht[qs[i].t%2].query(qs[i].t);
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
